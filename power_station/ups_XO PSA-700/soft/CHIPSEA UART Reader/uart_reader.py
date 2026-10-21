import serial
import time

def format_packet(packet):
    """
    Calculate the checksum as the XOR of the first 9 bytes and constant 0x55.
    Then, compare with the 10th byte.
    Format the packet as a string with columns aligned.
    """
    checksum_calc = 0
    for b in packet[:9]:
        checksum_calc ^= b
    # XOR the result with the constant 0x55
    checksum_calc ^= 0x55

    checksum_actual = packet[9]
    valid = (checksum_calc == checksum_actual)

    # Format each byte in decimal, right-aligned with a width of 4 characters
    formatted = ' '.join(f"{b:>4}" for b in packet)
    status = "OK" if valid else "FAIL"
    return f"{formatted}   {status} (calc: {checksum_calc})\n"

# Open the serial port COM3 with a baud rate of 2416 and timeout of 1 second.
with serial.Serial('COM3', baudrate=2416, timeout=1) as ser:
    with open("uart_log.txt", "w") as f:
        buffer = []
        last_time = time.time()
        while True:
            byte = ser.read(1)
            if byte:
                current_time = time.time()
                # If the gap between bytes exceeds 50ms and the buffer is incomplete,
                # discard the buffer as it is not a full packet.
                if current_time - last_time > 0.05 and len(buffer) < 10:
                    buffer = []
                last_time = current_time

                # Append the received byte (as integer) to the buffer.
                buffer.append(byte[0])

                # Once we have collected 10 bytes, process the packet.
                if len(buffer) == 10:
                    line = format_packet(buffer)
                    f.write(line)
                    f.flush()
                    print(line, end='')
                    buffer = []
