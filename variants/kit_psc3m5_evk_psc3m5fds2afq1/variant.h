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
 * This board has Uno R3-compatible headers, but the upstream DTS does not
 * define Arduino aliases. The SPI aliases map to the documented header signals
 * on P7.0-P7.3 through the variant overlay.
 */

#define PINS_COUNT          (8u)
#define NUM_DIGITAL_PINS    (8u)
#define NUM_ANALOG_INPUTS   (0u)
#define NUM_ANALOG_OUTPUTS  (0u)

#define PIN_LED             (2u)
#define LED_BUILTIN         PIN_LED

/* Arduino-style digital pins used by the board overlay */
#define PIN_D0              (0u)
#define PIN_D1              (1u)
#define PIN_D2              (2u)
#define PIN_D3              (3u)
#define PIN_D4              (4u)
#define PIN_D5              (5u)
#define PIN_D6              (6u)
#define PIN_D7              (7u)

#define PIN_SPI_SS          (PIN_D4)
#define PIN_SPI_SCK         (PIN_D5)
#define PIN_SPI_MOSI        (PIN_D6)
#define PIN_SPI_MISO        (PIN_D7)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

#define SPI_HOWMANY         (1u)

/*
 * If ADC inputs are enabled for this variant, map them here and expose them as
 * A0..A7 in the same way as the rest of the ArduinoCore-zephyr variants.
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