/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Arduino pluggable HID implementation for Zephyr USBD (device_next stack).
 *
 * Wraps Zephyr's usbd_hid API:
 *   - hid_device_register()     — registers descriptor + callbacks before USB init
 *   - hid_device_submit_report() — sends an IN report
 */

#include "HID.h"

#ifdef CONFIG_USBD_HID_SUPPORT

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>

/*
 * Retrieve the first enabled zephyr,hid-device instance from the devicetree.
 * The hid_dev0 node under usbhs@54900000 in the PSE84 variant overlay satisfies
 * this.  Any other variant that enables a zephyr,hid-device node will also work.
 */
#if !DT_HAS_COMPAT_STATUS_OKAY(zephyr_hid_device)
#error "HID.cpp: CONFIG_USBD_HID_SUPPORT is set but no zephyr,hid-device node " \
       "with status = okay found in devicetree.  Add an hid_dev0 node to your " \
       "variant overlay."
#endif

static const struct device *const _hid_dev = DEVICE_DT_GET(DT_INST(0, zephyr_hid_device));

/*
 * Combined HID report descriptor buffer.
 * Built from all registered HIDSubDescriptor fragments in HID_::begin().
 * Declared with 4-byte alignment as required by the Zephyr USB stack.
 */
static uint8_t __aligned(4) _rdesc[HID_MAX_DESCRIPTOR_SIZE];

/*
 * Report send buffer.  Must be 4-byte aligned (Zephyr USB DMA requirement).
 * +1 for a possible report ID prefix byte.
 */
static uint8_t __aligned(4) _report_buf[HID_MAX_REPORT_SIZE + 1];

/* ── Loader USB init helper (defined in usb_device_descriptor.c) ────────── */

/*
 * usbd_init_device() is compiled into the loader and exported to sketches
 * via the LLEXT symbol table.  It initialises the USBD stack (descriptors,
 * configurations, USB classes) and calls usbd_init().  Subsequent calls
 * from different subsystems (e.g. SerialUSB after HID) are no-ops that
 * return the existing context, so call order is safe.
 */
extern "C" struct usbd_context *usbd_init_device(usbd_msg_cb_t msg_cb);

/*
 * Minimal USBD message callback for HID-only sketches.
 * Handles VBUS ready/removed events so the USB device enumerates correctly
 * on boards that support VBUS detection.
 */
static void _usbd_hid_msg_cb(struct usbd_context *const ctx,
			      const struct usbd_msg *const msg)
{
	if (usbd_can_detect_vbus(ctx)) {
		if (msg->type == USBD_MSG_VBUS_READY) {
			usbd_enable(ctx);
		} else if (msg->type == USBD_MSG_VBUS_REMOVED) {
			usbd_disable(ctx);
		}
	}
}

/* ── Zephyr USBD HID callbacks ───────────────────────────────────────────── */

static void _iface_ready_cb(const struct device *dev, const bool ready) {
	(void)dev;
	HID()._setIfaceReady(ready);
}

/*
 * input_report_done is optional; without it hid_device_submit_report()
 * operates synchronously, which is fine for Arduino's single-threaded model.
 */
static const struct hid_device_ops _hid_ops = {
	.iface_ready = _iface_ready_cb,
	.get_report = nullptr,
	.set_report = nullptr,
	.set_idle = nullptr,
	.get_idle = nullptr,
	.set_protocol = nullptr,
	.input_report_done = nullptr,
	.output_report = nullptr,
	.sof = nullptr,
};

/* ── HID_ implementation ─────────────────────────────────────────────────── */

HID_::HID_()
	: _descriptors(nullptr), _descriptorSize(0), _registered(false), _active(false),
	  _ifaceReady(false) {
}

void HID_::AppendDescriptor(HIDSubDescriptor *node) {
	node->next = nullptr;

	if (_descriptors == nullptr) {
		_descriptors = node;
	} else {
		HIDSubDescriptor *tail = _descriptors;
		while (tail->next != nullptr) {
			tail = tail->next;
		}
		tail->next = node;
	}

	_descriptorSize += node->length;
}

int HID_::begin() {
	if (_registered) {
		/* Already registered — descriptor cannot be changed after init. */
		_active = true;
		return 0;
	}

	if (_descriptors == nullptr || _descriptorSize == 0) {
		return -EINVAL;
	}

	if (_descriptorSize > HID_MAX_DESCRIPTOR_SIZE) {
		return -ENOMEM;
	}

	if (!device_is_ready(_hid_dev)) {
		return -ENODEV;
	}

	/* Build contiguous descriptor buffer from the linked-list fragments. */
	uint16_t offset = 0;
	for (HIDSubDescriptor *d = _descriptors; d != nullptr; d = d->next) {
		memcpy(_rdesc + offset, d->data, d->length);
		offset += d->length;
	}

	int ret = hid_device_register(_hid_dev, _rdesc, offset, &_hid_ops);
	if (ret < 0) {
		return ret;
	}

	_registered = true;

	/*
	 * Initialise the USB device stack if it hasn't been started yet.
	 * usbd_init_device() is a no-op (returns existing context) when
	 * Serial.begin() or another subsystem already called it, so calling
	 * it here is always safe.
	 *
	 * Important ordering constraint: hid_device_register() MUST be called
	 * before usbd_init_device() (which calls usbd_init() internally).
	 * That is satisfied above.  If Serial.begin() was called first, USB
	 * is already initialised and hid_device_register() was too late for
	 * this boot — the HID interface won't appear until the next reset.
	 */
	struct usbd_context *ctx = usbd_init_device(_usbd_hid_msg_cb);
	if (ctx == nullptr) {
		return -ENODEV;
	}

	/*
	 * On hardware without VBUS detection usbd_enable() must be called
	 * explicitly.  When VBUS detection is available the enable happens
	 * in _usbd_hid_msg_cb() on USBD_MSG_VBUS_READY.
	 */
	if (!usbd_can_detect_vbus(ctx)) {
		ret = usbd_enable(ctx);
		/* -EALREADY is not an error — another path already enabled USB. */
		if (ret < 0 && ret != -EALREADY) {
			return ret;
		}
	}

	_active = true;
	return 0;
}

void HID_::end() {
	_active = false;
}

int HID_::SendReport(uint8_t id, const void *data, int len) {
	if (!_active || !_ifaceReady) {
		return -ENODEV;
	}

	if (id > 0) {
		/* Prefix report ID byte when report IDs are in use. */
		if (len + 1 > (int)sizeof(_report_buf)) {
			return -EINVAL;
		}
		_report_buf[0] = id;
		memcpy(_report_buf + 1, data, len);
		return hid_device_submit_report(_hid_dev, (uint16_t)(len + 1), _report_buf);
	} else {
		if (len > (int)sizeof(_report_buf)) {
			return -EINVAL;
		}
		memcpy(_report_buf, data, len);
		return hid_device_submit_report(_hid_dev, (uint16_t)len, _report_buf);
	}
}

/* ── Singleton accessor ──────────────────────────────────────────────────── */

HID_ &HID() {
	static HID_ hid;
	return hid;
}

#endif /* CONFIG_USBD_HID_SUPPORT */
