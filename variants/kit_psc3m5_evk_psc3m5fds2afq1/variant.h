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
 * Arduino header pin map for the Infineon KIT_PSC3M5_EVK.
 *
 * This board has Uno R3-compatible headers, but the upstream DTS does not
 * define Arduino aliases. The SPI aliases map to the documented header signals
 * on P7.0-P7.3 through the variant overlay.
 *
 * Hardware PWM is available on D3 and D9 at 500 Hz. D5 and D6 share those
 * timers and are not exposed as PWM outputs. No DAC outputs are configured.
 */

#define PINS_COUNT          (28u)
#define NUM_DIGITAL_PINS    (22u)
#define NUM_ANALOG_INPUTS   (6u)
#define NUM_ANALOG_OUTPUTS  (0u)

#define PIN_LED             (16u)
#define LED_BUILTIN         PIN_LED
#define LED_BUILTIN_1       (17u)
#define BUTTON_1            (18u)
#define BUTTON_2            (19u)

/* Arduino-style digital pins used by the board overlay */
#define PIN_D0              (0u)
#define PIN_D1              (1u)
#define PIN_D2              (2u)
#define PIN_D3              (3u)
#define PIN_D4              (4u)
#define PIN_D5              (5u)
#define PIN_D6              (6u)
#define PIN_D7              (7u)
#define PIN_D8              (8u)
#define PIN_D9              (9u)
#define PIN_D10             (10u)
#define PIN_D11             (11u)
#define PIN_D12             (12u)
#define PIN_D13             (13u)
#define PIN_D14             (14u)
#define PIN_D15             (15u)
#define PIN_D16             (16u)
#define PIN_D17             (17u)
#define PIN_D18             (18u)
#define PIN_D19             (19u)
#define PIN_D20             (20u)
#define PIN_D21             (21u)

#define PIN_SPI_SS          (PIN_D10)
#define PIN_SPI_SCK         (PIN_D13)
#define PIN_SPI_MOSI        (PIN_D11)
#define PIN_SPI_MISO        (PIN_D12)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

#define SPI_HOWMANY         (1u)

/* A0-A5 are analog-only inputs, separate from the digital GPIO table. */
#define PIN_A0              (22u)
#define PIN_A1              (23u)
#define PIN_A2              (24u)
#define PIN_A3              (25u)
#define PIN_A4              (26u)
#define PIN_A5              (27u)

#define PIN_SERIAL_RX       (20u)
#define PIN_SERIAL_TX       (21u)

#define SERIAL_HOWMANY      (1u)
#define SERIAL1_TX          PIN_SERIAL_TX
#define SERIAL1_RX          PIN_SERIAL_RX

#define SERIAL_PORT_HARDWARE Serial1
#define SERIAL_PORT_MONITOR Serial

#define WIRE_HOWMANY        (0u)

#endif /* __PINS_ARDUINO__ */