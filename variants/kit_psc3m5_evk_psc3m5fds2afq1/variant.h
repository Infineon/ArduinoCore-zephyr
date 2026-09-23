/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifndef __PINS_ARDUINO__
#define __PINS_ARDUINO__

#include <stdint.h>

/*
 * Minimal Arduino-style pin map for the Infineon KIT_PSC3M5_EVK.
 *
 * This board exposes its real GPIOs in the Zephyr DTS and we mapped the
 * Arduino-style aliases to the actual board-defined GPIOs in the overlay.
 * The mapping here is intentionally conservative and matches the board's
 * runtime GPIO layout without inventing a non-existent Uno R3 header.
 */

#define PINS_COUNT          (4u)
#define NUM_DIGITAL_PINS    (4u)
#define NUM_ANALOG_INPUTS   (0u)
#define NUM_ANALOG_OUTPUTS  (0u)

#define PIN_LED             (2u)
#define LED_BUILTIN         PIN_LED

/* Arduino-style digital pins used by the board overlay */
#define PIN_D0              (0u)
#define PIN_D1              (1u)
#define PIN_D2              (2u)
#define PIN_D3              (3u)

/*
 * If the board gains ADC-capable GPIOs in the future, these can be mapped here
 * and exposed as A0..A7 in the same way as the rest of the ArduinoCore-zephyr
 * variants. For now the board DTS only exposes the user GPIOs/LEDs and UART.
 */
#define PIN_A0              (0u)
#define PIN_A1              (1u)
#define PIN_A2              (2u)
#define PIN_A3              (3u)
#define PIN_A4              (4u)
#define PIN_A5              (5u)
#define PIN_A6              (6u)
#define PIN_A7              (7u)

#define PIN_SERIAL_RX       (0u)
#define PIN_SERIAL_TX       (1u)

#define SERIAL_HOWMANY      (1u)
#define SERIAL1_TX          (1u)
#define SERIAL1_RX          (0u)

#define SERIAL_PORT_HARDWARE Serial1
#define SERIAL_PORT_MONITOR Serial

#define WIRE_HOWMANY        (0u)

#endif /* __PINS_ARDUINO__ */
