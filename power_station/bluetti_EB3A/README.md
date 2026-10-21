# Bluetti EB3A – inverter firmware reverse engineering

This repository is **not** about the hardware (yet), but about the **firmware** of the  
**Bluetti EB3A** portable power station – specifically the code running in the inverter  
and in the front-panel / UI controller.

At the moment this is an **early-stage reverse engineering & re-creation attempt**:

- original firmware binaries collected and archived,
- a **Ghidra project** with disassembly / decompilation in progress,
- a **C “skeleton project”** where the decoded logic is gradually being rewritten  
  as readable, rebuildable source code.

There are no guarantees that this will ever become a full reimplementation – this is a  
“whenever there is time and mood” long-term side quest.

---

## Scope

Current focus:

- understanding **inverter firmware** behavior (power paths, modes, limits, protections),
- understanding the **front panel / UI** logic (display, keys, communication with inverter),
- *not yet* focusing on PCB-level hardware mapping or full schematic of the EB3A.

Targets here are for the **110 V model** (North America / JP variant), but many concepts  
may overlap with 230 V units.

---

## Firmware binaries (`fw_orig/`)

Folder: `fw_orig/` – original firmware dumps as obtained from the community.

Files:

- `fw_orig/Inventor_EB3A_110_08.bin`  
  Firmware for the **inverter** controller (110 V version, revision `110_08`).

- `fw_orig/Panel_EB3A_110_08.bin`  
  Firmware for the **front panel / interface** (display / buttons) for the same model/revision.

These binaries were originally posted by a user on the **gsm.in.ua** forum in a thread about  
EB3A backup firmware:

> “Резервна копія прошивки Bluetti EB3A” – see  
> https://gsm.in.ua/threads/rezervna-kopija-proshivki-bluetti-eb3a.879/

This repository simply **rehosts** those binaries for research and reverse engineering purposes.

---

## Ghidra analysis project (`fw_dasm/`)

Folder: `fw_dasm/` – the **Ghidra project** for disassembling & decompiling the EB3A firmware.

What’s happening there:

- importing the `.bin` images and mapping them to the appropriate CPU / address space,
- incrementally:
  - identifying **reset vectors**, startup code, ISR tables,
  - naming **functions**, **global variables**, and **state flags**,
  - documenting protocol / communication between inverter and panel where possible.

This is very much **work in progress**:

- many functions are still auto-named (`FUN_xxxx` style),
- data structures and state machines are only partially understood,
- comments and symbol names are evolving as new patterns are discovered.

Treat this as an “active notebook” rather than finished documentation.

---

## Re-created C source (`fw_src_recreated/`)

Folder: `fw_src_recreated/` – an attempt to **recreate** the original firmware logic as C code.

The idea:

- take insights from Ghidra (`fw_dasm/`),
- rewrite the logic into a **clean C project**, where:
  - functions and variables have meaningful names,
  - hardware accesses are abstracted as clear HAL-style calls,
  - control flow is easier to inspect and modify.

Right now this is just a **skeleton project**:

- some basic file / project structure,
- early stubs and partial functions,
- no full build pipeline or working binary yet.

Think of it as an experiment: “can we turn this blob of machine code into something that  
resembles human-written firmware again?”

---

## Project layout

```text
bluetti_EB3A/
  fw_orig/
    Inventor_EB3A_110_08.bin   – original inverter firmware (110 V model, rev 110_08)
    Panel_EB3A_110_08.bin      – original panel/UI firmware (110 V model, rev 110_08)

  fw_dasm/
    ...                        – Ghidra project for disassembly / decompilation
                                 (functions/vars gradually renamed and annotated)

  fw_src_recreated/
    ...                        – C “re-created” firmware project (skeleton, WIP)
                                 where decoded logic is being rewritten as source

  README.md                    – this file
```

## Status

Current reverse-engineering progress:

- 🟡 **Original firmware collection** – done for EB3A 110 V, rev `110_08` (inverter + panel).
- 🟡 **Ghidra project (`fw_dasm/`)** – **very early stage**, ongoing:
  - basic mapping, some functions/ISRs identified,
  - naming + commenting is gradually expanding when time allows.
- 🟡 **C re-creation (`fw_src_recreated/`)** – **skeleton only**:
  - just enough structure to start porting logic from Ghidra,
  - not yet in a compilable, testable state.

Expect this repo to evolve **slowly** and in bursts, depending on available time and interest.


## Legal & ethical notes

- All Bluetti-related names (e.g. **Bluetti**, **EB3A**) are trademarks of their respective owners.
- The original firmware is:
  - taken from a **public community backup** post (see link above),
  - hosted here purely for **education, research, and interoperability**.
- This project:
  - is **not affiliated** with Bluetti or any associated company,
  - does **not** provide tools or instructions for bypassing safety limits or protections,
  - is **not** intended to encourage unsafe modifications of power hardware.

If you are a rights holder and have concerns about particular files or details,  
please contact the maintainer of this repository for discussion.

---

## Safety note (even though this is “just firmware”)

The Bluetti EB3A is a **mains-connected, high-power device** with:

- high-voltage DC inside,
- lithium battery packs,
- inverter stages that can happily vaporize traces if something goes wrong.

Any modification of firmware or control logic **directly affects safety**.  
Do not flash experimental firmware into a live unit unless you fully understand  
the consequences and are prepared for the risks (including fire, damage, or injury).

This repository is best treated as:

> “a lab notebook for understanding how EB3A behaves internally”,  
> not as a ready-made toolkit for hacking production devices.

