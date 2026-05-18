/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Arduino variant header for KIT-PSE84-AI (PSOC Edge E84, CM33 secure)
 *
 * Pin numbers below correspond to the digital-pin-gpios indices
 * defined in the devicetree overlay:
 *
 *   D0  = P10_7  LED_0           D6  = P8_0   I2C_SCL
 *   D1  = P10_5  LED_1           D7  = P8_1   I2C_SDA
 *   D2  = P20_6  LED_RGB_RED     D8  = P21_6  SPI_CLK
 *   D3  = P20_4  LED_RGB_GREEN   D9  = P21_5  SPI_MOSI
 *   D4  = P20_5  LED_RGB_BLUE    D10 = P21_4  SPI_MISO
 *   D5  = P7_0   SW1 / BTN       D11 = P21_7  SPI_CS
 */

#pragma once

/** SPI pin definitions (D-number indices into digital-pin-gpios) */
#define MOSI    9
#define MISO    10
#define SCK     8
#define SS      11

/** I2C pin definitions (D-number indices into digital-pin-gpios) */
#define SDA     7
#define SCL     6
#define SDA1    21
#define SCL1    20

/** Explicit Wire instance pin aliases */
#define PIN_WIRE_SDA    SDA
#define PIN_WIRE_SCL    SCL
#define PIN_WIRE1_SDA   SDA1
#define PIN_WIRE1_SCL   SCL1

/** Built-in LED aliases (D-number indices into digital-pin-gpios) */
#undef LED_BUILTIN
#define LED_BUILTIN         0   /**< LED_0 on P10_7 (primary)    */
#define LED_BUILTIN_1       0   /**< LED_0 on P10_7              */
#define LED_BUILTIN_2       1   /**< LED_1 on P10_5              */
#define LED_BUILTIN_ACTIVE  (HIGH)

/** RGB LED aliases */
#define LED_RED             2   /**< P20_6 */
#define LED_GREEN           3   /**< P20_4 */
#define LED_BLUE            4   /**< P20_5 */

/** Built-in button alias */
#define BTN_BUILTIN         5   /**< SW1 on P7_0 (active-low) */

/**
 * GPIO aliases using Infineon-style P<port>_<pin> names.
 * These map directly to the Arduino D-number indices from digital-pin-gpios.
 */
#define P10_7               0
#define P10_5               1
#define P20_6               2
#define P20_4               3
#define P20_5               4
#define P7_0                5
#define P8_0                6
#define P8_1                7
#define P21_6               8
#define P21_5               9
#define P21_4               10
#define P21_7               11
#define P16_0               12
#define P16_1               13
#define P16_2               14
#define P16_3               15
#define P16_4               16
#define P16_5               17
#define P16_6               18
#define P16_7               19
#define P17_0               20
#define P17_1               21
#define P17_2               22
#define P17_3               23
#define P17_4               24
#define P17_5               25
#define P17_7               26
#define P11_1               27
#define P9_0                28
#define P9_1                29
#define P9_2                30
#define P9_3                31

/** Optional aliases matching board documentation naming */
#define SERIAL_INT0         P9_0
#define SERIAL_INT1         P9_1
#define SERIAL_INT2         P9_2
#define SERIAL_INT3         P9_3
