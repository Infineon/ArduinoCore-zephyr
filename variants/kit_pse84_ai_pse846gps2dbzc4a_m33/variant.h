/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Arduino variant header for KIT-PSE84-AI (PSOC Edge E84, CM33 secure)
 *
 * Pin numbers correspond to digital-pin-gpios indices in the overlay:
 *
 *   D0  = P17_5             D12 = P17_2             D24 = P15_4 - A4
 *   D1  = P17_7             D13 = P17_3             D25 = P15_5 - A5
 *   D2  = P16_1             D14 = P17_0 - I2C1_SCL  D26 = P15_6 - A6
 *   D3  = P16_0             D15 = P17_1 - I2C1_SDA  D27 = P15_7 - A7
 *   D4  = P16_3             D16 = P9_3  - SINT3      D28 = P13_0 - A8
 *   D5  = P16_2             D17 = P9_2  - SINT2      D29 = P13_1 - A9
 *   D6  = P16_5             D18 = P9_1  - SINT1      D30 = P13_2 - A10
 *   D7  = P16_4             D19 = P9_0  - SINT0      D31 = P13_3 - A11
 *   D8  = P16_7             D20 = P15_0 - A0         D32 = P13_4 - A12
 *   D9  = P16_6             D21 = P15_1 - A1         D33 = P13_5 - A13
 *   D10 = P11_1             D22 = P15_2 - A2         D34 = P13_6 - A14
 *   D11 = P17_4             D23 = P15_3 - A3         D35 = P13_7 - A15
 *
 *   D36 = P10_7 - LED_0     D40 = P20_5 - LED_BLU   D44 = P21_6 - SPI_CLK
 *   D37 = P10_5 - LED_1     D41 = P7_0  - SW1/BTN   D45 = P21_5 - SPI_MOSI
 *   D38 = P20_6 - LED_RED   D42 = P8_0  - I2C_SCL   D46 = P21_4 - SPI_MISO
 *   D39 = P20_4 - LED_GRN   D43 = P8_1  - I2C_SDA   D47 = P21_7 - SPI_CS
 */

#pragma once

/** SPI pin definitions (Wire = SCB3 on P21) */
#define MOSI    45  /**< P21_5 */
#define MISO    46  /**< P21_4 */
#define SCK     44  /**< P21_6 */
#define SS      47  /**< P21_7 */

/** I2C pin definitions (Wire = SCB0 on P8, Wire1 = SCB5 on P17) */
#define SDA     43  /**< P8_1  */
#define SCL     42  /**< P8_0  */
#define SDA1    15  /**< P17_1 */
#define SCL1    14  /**< P17_0 */

#define PIN_WIRE_SDA    SDA
#define PIN_WIRE_SCL    SCL
#define PIN_WIRE1_SDA   SDA1
#define PIN_WIRE1_SCL   SCL1

/** Built-in LED aliases */
#undef LED_BUILTIN
#define LED_BUILTIN         36  /**< LED_0 on P10_7 (primary) */
#define LED_BUILTIN_1       36  /**< LED_0 on P10_7           */
#define LED_BUILTIN_2       37  /**< LED_1 on P10_5           */
#define LED_BUILTIN_ACTIVE  (HIGH)

/** RGB LED aliases */
#define LED_RED             38  /**< P20_6 */
#define LED_GREEN           39  /**< P20_4 */
#define LED_BLUE            40  /**< P20_5 */

/** Built-in button alias */
#define BTN_BUILTIN         41  /**< SW1 on P7_0 (active-low) */

/** Analog pin definitions (indices into digital-pin-gpios) */
#define A0              20
#define A1              21
#define A2              22
#define A3              23
#define A4              24
#define A5              25
#define A6              26
#define A7              27
#define A8              28
#define A9              29
#define A10             30
#define A11             31
#define A12             32
#define A13             33
#define A14             34
#define A15             35

/**
 * GPIO aliases using Infineon-style P<port>_<pin> names.
 * Values are indices into the digital-pin-gpios array in the overlay.
 */
/* Expansion GPIO - P16/P17 */
#define P17_5               0
#define P17_7               1
#define P16_1               2
#define P16_0               3
#define P16_3               4
#define P16_2               5
#define P16_5               6
#define P16_4               7
#define P16_7               8
#define P16_6               9
#define P11_1               10
#define P17_4               11
#define P17_2               12
#define P17_3               13
#define P17_0               14
#define P17_1               15

/* UART1 / serial interrupt pins */
#define P9_3                16
#define P9_2                17
#define P9_1                18
#define P9_0                19

/* Analog input pins - P15 (A0-A7) */
#define P15_0               20
#define P15_1               21
#define P15_2               22
#define P15_3               23
#define P15_4               24
#define P15_5               25
#define P15_6               26
#define P15_7               27

/* Analog input pins - P13 (A8-A15) */
#define P13_0               28
#define P13_1               29
#define P13_2               30
#define P13_3               31
#define P13_4               32
#define P13_5               33
#define P13_6               34
#define P13_7               35

/* On-board peripherals */
#define P10_7               36
#define P10_5               37
#define P20_6               38
#define P20_4               39
#define P20_5               40
#define P7_0                41
#define P8_0                42
#define P8_1                43
#define P21_6               44
#define P21_5               45
#define P21_4               46
#define P21_7               47

/** Serial interrupt aliases */
#define SERIAL_INT0         P9_0
#define SERIAL_INT1         P9_1
#define SERIAL_INT2         P9_2
#define SERIAL_INT3         P9_3
