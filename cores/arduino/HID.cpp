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
