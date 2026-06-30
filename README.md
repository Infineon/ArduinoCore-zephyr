PSOC™# Arduino Core for Zephyr on Infineon PSOC™ Edge E84

> [!IMPORTANT]
> This project is a work in progress.
> It does not yet cover the full Arduino API surface or all PSOC™ Edge features.

This repository contains the Zephyr-based Arduino core port for:

- Infineon KIT-PSE84-AI (PSOC™ Edge E84)

## Install In Arduino IDE

Use Arduino IDE 2.x and install the platform through Boards Manager.

1. Open Arduino IDE.
2. Go to File > Preferences.
3. In Additional boards manager URLs, add:

```text
https://github.com/michal-gora/ArduinoCore-zephyr/releases/latest/download/package_infineon_pse84_index.json
```

If you are using a different fork, replace the owner/repo segment with your repository path.

4. Open Boards Manager.
5. Search for PSOC™ Edge.
6. Install Infineon PSOC™ Edge Boards.
7. Select board:

```text
Tools > Board > Infineon PSOC™ Edge Boards > Infineon KIT-PSE84-AI (PSOC™ Edge E84)
```

8. Select your serial/debug port.
9. Run Tools > Burn Bootloader once.
10. Upload a sketch.

### Optional: Install With Arduino CLI

```bash
arduino-cli core install infineon:zephyr_pse84 --additional-urls https://github.com/michal-gora/ArduinoCore-zephyr/releases/latest/download/package_infineon_pse84_index.json
```

## Current Arduino API Coverage

Current status for this port is summarized below.

| Area | Status | Notes |
| --- | --- | --- |
| Sketch lifecycle (`setup`, `loop`) | ✅ | Core runtime is present and used by samples. |
| Timing (`millis`, `micros`, `delay`, `yield`) | ✅ | Implemented in Zephyr-backed core code. |
| Digital GPIO (`pinMode`, `digitalRead`, `digitalWrite`) | ✅ | Implemented in core. |
| Interrupts (`attachInterrupt`, `detachInterrupt`) | ✅ | Implemented in core. |
| Analog input (`analogRead`) | ❌ | Not supported yet on this PSOC™ Edge port. |
| Analog output (`analogWrite`) | ❌ | Not supported yet on this PSOC™ Edge port. |
| UART Serial | ✅ | Zephyr UART-backed serial is implemented. |
| USB Serial | ❌ | Current PSOC™ Edge board config is non-native USB for sketch upload/runtime serial. |
| SPI | ❌ | Not supported yet on this PSOC™ Edge port. |
| I2C (`Wire`) | ✅ | Supported on this PSOC™ Edge port. |
| Threads (`Thread`) | ❌ | Not supported yet on this PSOC™ Edge port. |
| CAN library | ❌ | Explicitly skipped for this board. |
| Ethernet library | ❌ | Explicitly skipped for this board. |
| RTC library | ❌ | Explicitly skipped for this board. |
| WiFi library | ❌ | Explicitly skipped for this board. |

## Known Scope And Limits

- This is an early PSOC™ Edge port focused on enabling core Arduino workflows on Zephyr.
- API compatibility is incomplete and may change between releases.
- Some subsystems compile but are not yet fully validated on KIT-PSE84-AI.

## Contributing And Feedback

Feedback and collaboration are highly encouraged.

- Report bugs and request features in [Issues](/../../issues)
- Submit fixes through [Pull Requests](/../../pulls)
- Ask questions and discuss roadmap in [Discussions](/../../discussions)

When reporting issues, please include:

- Board and host OS
- Core version
- Minimal sketch to reproduce
- Full build/upload logs

## Troubleshooting

### Sketch Does Not Start

- Build and upload in `Debug` mode, then open serial output and run `sketch` from the Zephyr shell.
- For hard faults or early crashes, capture logs through the board debug/UART path.

### `llext` Undefined Symbol Errors

If upload succeeds but execution fails with an `Undefined symbol` error, the sketch is using a symbol not exported by the loader image for this build.

- Rebuild and flash the loader for this board.
- If needed, extend exported symbols in loader integration sources and rebuild.

## Development Notes

This core uses Zephyr plus a board-specific loader. Sketches are built as loadable artifacts and executed by the preflashed loader firmware.

- Loader and boot integration: [loader](loader)
- Core implementation: [cores/arduino](cores/arduino)
- PSOC™ Edge E84 variant files: [variants/kit_pse84_ai_pse846gps2dbzc4a_m33](variants/kit_pse84_ai_pse846gps2dbzc4a_m33)

Key implementation areas in this repository:

- Board definition and upload tooling: [boards.txt](boards.txt)
- Variant for KIT-PSE84-AI: [variants/kit_pse84_ai_pse846gps2dbzc4a_m33](variants/kit_pse84_ai_pse846gps2dbzc4a_m33)
- Core implementation: [cores/arduino](cores/arduino)

If you are extending support, start by validating small samples first (for example blinky, hello, threads) and then move to subsystem-specific libraries.