/*
 * Board-specific test pin definitions for extra/arduino_core_tests.
 *
 * Each board adds its own #if defined(ARDUINO_<BOARD>) block below (see
 * tests/README.md). This file intentionally has no board blocks of its own:
 * board-specific content belongs to the branch that owns that board.
 */
#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

/*
 * Infineon KIT-PSE84-AI (PSOC Edge E84).
 *
 * D0 (P17_5) and D1 (P17_7) must be connected with a jumper when running
 * test_digitalio_single. Both pins are exposed as digital GPIOs on the board
 * expansion header and are mapped by the Arduino variant overlay
 * (variants/kit_pse84_ai_pse846gps2dbzc4a_m33, digital-pin-gpios).
 */
#if defined(ARDUINO_KIT_PSE84_AI)
#define TEST_PIN_DIGITAL_IO_OUTPUT 0
#define TEST_PIN_DIGITAL_IO_INPUT 1
#endif

#endif // TEST_CONFIG_H
