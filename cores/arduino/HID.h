/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Arduino-compatible Pluggable HID layer for Zephyr USBD (device_next stack).
 *
 * This header provides the same API that the standard Arduino HID libraries
 * (Keyboard, Mouse, etc.) expect, bridging them to the Zephyr USBD HID class
 * driver (usbd_hid).
 *
 * Timing constraint
 * -----------------
 * Zephyr requires hid_device_register() to be called BEFORE usbd_init().
 * usbd_init() is triggered by Serial.begin() (via SerialUSB_.enable_usb_device_next()).
 * Therefore:
 *   - Keyboard.begin() / Mouse.begin() must be called BEFORE Serial.begin().
 *   - Calling begin() after Serial.begin() will have no effect and SendReport
 *     will return -ENODEV.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus

#if defined(CONFIG_USBD_HID_SUPPORT)

/*
 * Define _USING_HID so that Arduino HID libraries (Keyboard.h, Mouse.h, etc.)
 * know to use the pluggable HID path rather than the legacy AVR path.
 */
#define _USING_HID

/* Maximum total size of the combined HID report descriptor (all plugins). */
#ifndef HID_MAX_DESCRIPTOR_SIZE
#define HID_MAX_DESCRIPTOR_SIZE 256
#endif

/* Maximum input report size: 1 byte report ID + 63 bytes payload. */
#ifndef HID_MAX_REPORT_SIZE
#define HID_MAX_REPORT_SIZE 64
#endif

/**
 * @brief A fragment of a HID report descriptor contributed by one HID plugin.
 *
 * Each library (Keyboard, Mouse, etc.) constructs one of these with its own
 * fragment and registers it via HID().AppendDescriptor() from its constructor.
 * The HID singleton builds the combined descriptor when begin() is called.
 */
struct HIDSubDescriptor {
    const void   *data;
    const uint16_t length;
    HIDSubDescriptor *next;

    HIDSubDescriptor(const void *d, const uint16_t l)
        : data(d), length(l), next(nullptr) {}
};

/**
 * @brief Arduino pluggable HID singleton.
 *
 * Libraries append their descriptor fragments via AppendDescriptor() (typically
 * in their constructor).  begin() registers the combined descriptor with the
 * Zephyr USBD HID driver.  SendReport() submits an IN report.
 */
class HID_ {
public:
    HID_();

    /**
     * Register the accumulated HID report descriptor with the Zephyr USBD HID
     * driver.  Must be called before Serial.begin().
     *
     * @return 0 on success, negative errno on failure.
     */
    int begin();

    /** Deactivate HID reporting (does not de-register the descriptor). */
    void end();

    /**
     * Submit an HID input report.
     *
     * @param id   Report ID (0 if no report IDs are used in the descriptor).
     * @param data Pointer to the report payload.
     * @param len  Payload length in bytes.
     * @return 0 on success, negative errno on failure.
     */
    int SendReport(uint8_t id, const void *data, int len);

    /**
     * Append a HID report descriptor fragment.  Typically called by library
     * constructors before begin().
     */
    void AppendDescriptor(HIDSubDescriptor *node);

    /** Returns true if begin() succeeded and the USB interface is ready. */
    bool isReady() const { return _ifaceReady; }

    /* Allow the iface_ready callback (C linkage) to update _ifaceReady. */
    void _setIfaceReady(bool r) { _ifaceReady = r; }

private:
    HIDSubDescriptor *_descriptors;
    uint16_t          _descriptorSize;
    bool              _registered;
    bool              _active;
    volatile bool     _ifaceReady;
};

/** Access the HID singleton. */
HID_& HID();

#endif /* CONFIG_USBD_HID_SUPPORT */
#endif /* __cplusplus */
