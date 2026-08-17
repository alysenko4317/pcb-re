# APower 2K UPS / Inverter Reverse Engineering

This directory contains work-in-progress reverse-engineering notes for an APower 2K UPS/inverter.

At the moment, the main useful artifact is a manually reconstructed inverter schematic in **sPlan 8** format:

- `inv_1615.spl8`

The schematic was traced manually, so it may contain mistakes and should not be treated as a verified reference.

Most of the inverter power stage appears to follow the reference designs for the **EG1615** and **EG8026** controllers quite closely.

A **Nation** microcontroller located on the power board appears to control the battery charger. This part is more interesting from the reverse-engineering perspective, since it does not appear to simply replicate a datasheet reference circuit.

The charger power stage uses a **four-switch full-bridge / synchronous buck-boost topology**, so in principle it can operate with the input voltage either above or below the battery voltage.

There is another Nation MCU on the board containing the EG1615 and EG8026. Its exact role has not yet been investigated.

The unit arrived in very poor condition after an apparently unsuccessful previous repair attempt. The power stage was heavily damaged, including:

- all major power switches;
- several PCB traces;
- the EG8026 controller;
- part of the EG8026 surrounding circuitry.

Other files in this directory are currently mostly datasheets, reference materials, and miscellaneous notes. The repository is still being organized and may be updated as the reverse-engineering work progresses.