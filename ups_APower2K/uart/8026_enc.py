import argparse
import sys
import time
from datetime import datetime
from typing import Optional

import serial


PACKET_SIZE = 16
PACKET_HEADER = 0x55


FAULT_CODES = {
    0x00: "No fault",
    0x01: "Overload",
    0x02: "Short circuit",
    0x03: "Low output voltage",
    0x04: "Bus overvoltage",
    0x05: "Bus undervoltage",
    0x06: "Phase error",
    0x07: "Overtemperature",
    0x08: "Overcurrent",
    0x09: "Frequency mismatch",
    0x0A: "Motor speed too low",
    0x0C: "Output off",
}


def crc16_eg8026(data: bytes) -> int:
    """
    Calculate CRC16 for EG8026 packets.

    Polynomial from datasheet:
        x^16 + x^15 + x^2 + 1

    In hex this corresponds to 0x8005.
    This implementation uses a standard MSB-first bitwise algorithm.

    Note:
    The datasheet mentions the polynomial, but does not clearly describe
    init/xor/reflection details in the public summary. If CRC does not match
    on real hardware, the algorithm may need adjustment.
    """
    poly = 0x8005
    crc = 0x0000

    for byte in data:
        crc ^= (byte << 8)
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ poly) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF

    return crc


def to_u16_be(high_byte: int, low_byte: int) -> int:
    """
    Convert two bytes to unsigned 16-bit integer, big-endian order.
    """
    return (high_byte << 8) | low_byte


def to_i8(value: int) -> int:
    """
    Convert unsigned byte to signed 8-bit integer.
    """
    return value - 256 if value >= 128 else value


def decode_fault(code: int) -> str:
    """
    Return fault text for a fault code.
    """
    return FAULT_CODES.get(code, f"Unknown fault 0x{code:02X}")


def format_packet(packet: bytes) -> str:
    """
    Decode one valid 16-byte EG8026 status packet into readable text.
    """
    vout_raw = to_u16_be(packet[1], packet[2])
    iout_raw = to_u16_be(packet[3], packet[4])
    vin_raw = to_u16_be(packet[5], packet[6])
    period_us = to_u16_be(packet[7], packet[8])
    igbt_temp_c = to_i8(packet[9])
    fault_code = packet[10]
    inv_temp_c = to_i8(packet[11])
    power_w = to_u16_be(packet[12], packet[13])

    vout_v = vout_raw * 0.1
    iout_a = iout_raw * 0.01
    vin_v = vin_raw * 0.1

    frequency_hz: Optional[float]
    if period_us > 0:
        frequency_hz = 1_000_000.0 / period_us
    else:
        frequency_hz = None

    fault_text = decode_fault(fault_code)

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]

    return (
        f"[{timestamp}] "
        f"Vout={vout_v:.1f} V, "
        f"Iout={iout_a:.2f} A, "
        f"Vin={vin_v:.1f} V, "
        f"Period={period_us} us, "
        f"Freq={(f'{frequency_hz:.2f} Hz' if frequency_hz is not None else 'N/A')}, "
        f"IGBT Temp={igbt_temp_c} C, "
        f"Inv Temp={inv_temp_c} C, "
        f"Power={power_w} W, "
        f"Fault=0x{fault_code:02X} ({fault_text}), "
        f"Raw={packet.hex(' ')}"
    )


def try_extract_packet(buffer: bytearray) -> Optional[bytes]:
    """
    Try to extract one valid packet from the buffer.

    Strategy:
    - Find the header byte 0x55
    - Ensure there are at least 16 bytes from that position
    - Validate CRC
    - If valid, remove packet from buffer and return it
    - If invalid, discard one byte and continue later
    """
    while True:
        if len(buffer) < PACKET_SIZE:
            return None

        try:
            start_index = buffer.index(PACKET_HEADER)
        except ValueError:
            buffer.clear()
            return None

        if start_index > 0:
            del buffer[:start_index]

        if len(buffer) < PACKET_SIZE:
            return None

        candidate = bytes(buffer[:PACKET_SIZE])

        calculated_crc = crc16_eg8026(candidate[:14])
        packet_crc = to_u16_be(candidate[14], candidate[15])

        if calculated_crc == packet_crc:
            del buffer[:PACKET_SIZE]
            return candidate

        # CRC mismatch: discard the header and continue searching.
        del buffer[0]


def write_log_line(log_file, line: str) -> None:
    """
    Write a line to the log file and flush immediately.
    """
    log_file.write(line + "\n")
    log_file.flush()


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Read and decode EG8026 UART status packets."
    )
    parser.add_argument(
        "port",
        help="Serial port name, for example COM3 or /dev/ttyUSB0",
    )
    parser.add_argument(
        "--baudrate",
        type=int,
        default=9600,
        help="UART baud rate (default: 9600)",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=0.2,
        help="Serial read timeout in seconds (default: 0.2)",
    )
    parser.add_argument(
        "--raw",
        action="store_true",
        help="Print all received raw bytes before packet parsing",
    )
    parser.add_argument(
        "--log",
        help="Optional path to a log file",
    )

    args = parser.parse_args()

    log_file = None
    if args.log:
        log_file = open(args.log, "a", encoding="utf-8")

    try:
        ser = serial.Serial(
            port=args.port,
            baudrate=args.baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=args.timeout,
        )
    except serial.SerialException as ex:
        print(f"Failed to open serial port {args.port}: {ex}", file=sys.stderr)
        return 1

    print(
        f"Opened {args.port} at {args.baudrate} baud, "
        f"8N1, timeout={args.timeout}s"
    )
    print("Waiting for EG8026 status packets...")
    print("Press Ctrl+C to stop.")

    buffer = bytearray()

    try:
        while True:
            chunk = ser.read(256)
            if not chunk:
                continue

            if args.raw:
                raw_line = f"RX RAW: {chunk.hex(' ')}"
                print(raw_line)
                if log_file:
                    write_log_line(log_file, raw_line)

            buffer.extend(chunk)

            while True:
                packet = try_extract_packet(buffer)
                if packet is None:
                    break

                line = format_packet(packet)
                print(line)
                if log_file:
                    write_log_line(log_file, line)

    except KeyboardInterrupt:
        print("\nStopped by user.")
    finally:
        ser.close()
        if log_file:
            log_file.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())