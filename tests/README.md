# Arduino core test configuration

Configuration for the shared
[`arduino-core-tests`](https://github.com/Infineon/arduino-core-tests) suite
(vendored at [`extra/arduino_core_tests`](../extra/arduino_core_tests)) lives
in a single file, `tests/test_config.h`, exactly where that submodule's own
Makefile expects it (`../../tests/test_config.h`, relative to the submodule).

This matches how `arduino-core-psoc6` and `XMC-for-Arduino` consume the same
submodule: one `test_config.h`, with a `#if defined(ARDUINO_<BOARD>)` block
per board. `arduino-cli` already defines `ARDUINO_{build.board}` for every
compile (see `platform.txt`'s `recipe.c.o.pattern`/`recipe.cpp.o.pattern` and
each board's `<board>.build.board` in `boards.txt`), so the right block is
selected automatically — no per-board staging or wrapper script needed.

## Adding a board

1. Add a `#if defined(ARDUINO_<BOARD>)` block to `tests/test_config.h`, using
   the board's `build.board` value from `boards.txt` (e.g. `KIT_PSE84_AI`).
2. Define the pins required by the selected tests, using Arduino pin numbers
   (e.g. `D0`), not MCU port/pin names.
3. Verify each Arduino pin against the board's variant overlay
   (`variants/<variant>/*.overlay`, `zephyr,user` / `digital-pin-gpios`) and
   the board schematic or official pinout.
4. Document any required jumpers or external connections next to the block.

For example, `test_digitalio_single` requires two physically connected GPIOs:

```c
#if defined(ARDUINO_<BOARD>)
#define TEST_PIN_DIGITAL_IO_OUTPUT <output pin>
#define TEST_PIN_DIGITAL_IO_INPUT <input pin>
#endif
```

The test drives the output pin and reads the input pin, so both the pin
assignment and the physical connection between them are part of the test
configuration.

## Running a test

```bash
make -C extra/arduino_core_tests FQBN=<fqbn> UNITY_PATH=Unity \
  TESTS=-DTEST_DIGITALIO_SINGLE test_digitalio_single compile
```

No wrapper script is needed: `tests/test_config.h` already sits where the
submodule's Makefile expects it.

To flash and run on a single connected board, add `PORT=` and
`ENABLE_SYNC=0`:

```bash
make -C extra/arduino_core_tests FQBN=<fqbn> UNITY_PATH=Unity PORT=<port> \
  TESTS=-DTEST_DIGITALIO_SINGLE ENABLE_SYNC=0 test_digitalio_single
```

`ENABLE_SYNC` defaults to `1`: tests named `*_connected2_*` (e.g.
`test_wire_connected2_masterpingpong`) need two boards and wait for each
other's handshake over serial, so leave it enabled for those. Tests named
`*_single` (or loopback tests like `test_spi_connected1_loopback`) only use
one board; without `ENABLE_SYNC=0` they hang forever printing
`synchronising with host...`, since nothing ever answers the handshake.
