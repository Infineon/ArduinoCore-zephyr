/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Arduino variant header for KIT-PSE84-AI (PSoC Edge E84, CM33 secure)
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
