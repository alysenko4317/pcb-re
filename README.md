# Reverse Engineering Schematics of Power Electronics

This repository contains reverse-engineered schematics of various power electronics devices.  
All schematics are manually traced and drawn by me during my free time, mostly as part of repair processes.

## About the Schematics

- **Work in Progress**: The schematics do not claim to be fully complete or accurate.  
  They are created as a personal effort to document and analyze the circuits for repair and educational purposes.
- **Tools Used**:
  - **Sprint Layout** – used for PCB tracing and layout.
  - **sPlan** – used for creating circuit schematics.
- **Methodology**:
  - PCBs are traced using photos as templates to assist in schematic creation.
  - For surface-mounted device (SMD) components, stitched microscope images are used for better accuracy.
  - Sprint Layout projects are **not intended for PCB fabrication**; their primary purpose is to help
    understand connectivity and assist schematic reverse-engineering.

## List of Reverse-Engineered Schematics

The repository currently contains technical information for the following devices:

- **Bluetti T200 Charger (28V)**  
- **LittoKala 3S Charger (12.6V)**  
- **Sanken SBS-10020A Power Supply (24V/48V)**  
- **HP Powerwise 1000 UPS**
- **MGE Pulsar 1500 UPS**  
- **Penxess-6 Mini-UPS (12.6V)**  
- **Powerware Prestige 1000 UPS**
- **XO PSA-700 Power Station (inverter)**
- **Millenium HOHC-2500 (Simistor Voltage Regulator)**  
- **FSP-150 DC Power Supply (15V 10A)**

## Per-Device Documentation

Each larger project has its own README with photos, schematic/layout notes, ROM dumps and Ghidra/analysis projects:

- [`ups_Powerware_Prestige/README.md`](./ups_Powerware_Prestige/README.md)  
  Reverse engineering of **Powerware / Powerware 9 Prestige series UPS** (600–1800 VA):  
  power board schematic & PCB tracing, control board panoramas, ROM dump and Ghidra project.

- [`ups_HP Powerwise 1000/README.md`](./ups_HP%20Powerwise%201000/README.md)  
  Reverse engineering of **HP Powerwise 1000 UPS**:  
  main power board schematic (sPlan), LED board, internal photos and notes about controller architecture.

- [`ups_XO PSA-700/README.md`](./ups_XO%20PSA-700/README.md)  
  Reverse engineering of **XO PSA-700 power station / inverter**:  
  topology notes, schematics and internal photos (where available).

- [`avr_Millennium HOHC-2500/README.md`](./avr_Millennium%20HOHC-2500/README.md)  
  Reverse engineering of **Millennium HOHC-2500 8-step triac AVR** (no MCU, only analog logic & op-amps):  
  schematic in sPlan, PCB traced from photo templates, and internal photos for repair and study.

As new devices are added or existing projects become more complete, their per-device READMEs will be updated with:

- current status of the reverse-engineered schematics and PCB tracing,
- controller / firmware notes (MCUs, ROM/Flash dumps, disassembly projects),
- photos and other reference material for repair and study.

## Contribution and Disclaimer

This is a personal project maintained in my spare time.  
Contributions (photos, measurements, corrections, or additional schematics) are welcome, but please keep in mind:

- The repository is primarily a **documentation and repair aid**, not a production hardware design.
- Some schematics/layouts can contain mistakes or be incomplete; always verify before using them
  in any safety-critical or high-power design. :contentReference[oaicite:2]{index=2}

**Disclaimer**: The information in this repository is for educational and repair purposes only.  
Use at your own risk.
