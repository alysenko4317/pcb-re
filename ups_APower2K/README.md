# APower 2K UPS / Inverter Reverse Engineering

This directory contains work-in-progress reverse-engineering notes for an APower 2K UPS/inverter.

At the moment, the main useful artifact is a manually reconstructed inverter schematic. The **sPlan 8** version is the primary working file, while an **SVG** export is kept synchronized with it for convenient viewing:

- `inv_1615.spl8` — primary editable schematic;
- `inv_1615.svg` — SVG version for direct viewing.

![APower 2K inverter schematic](inv_1615.svg)

The schematic was traced manually, so it may contain mistakes and should not be treated as a verified reference. It is also still incomplete: not every part of the circuit has been traced in the same level of detail, and not all component reference designators have been assigned yet.

The uneven level of detail is intentional and reflects the actual repair work performed on this particular unit. The **EG8026-based section** was the most heavily damaged part of the inverter: the controller itself, many surrounding SMD components, and several PCB traces had to be reconstructed or replaced. As a result, this part of the schematic is currently the most complete and detailed, since it was traced directly as part of the repair process.

The **EG1615-based section** was affected much less severely and, apart from the power switches, did not require the same degree of reconstruction. That part of the schematic was therefore traced mainly for completeness and has so far been a lower-priority reverse-engineering task.

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
