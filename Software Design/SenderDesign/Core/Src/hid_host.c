/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 ELEC5550 Team0-03
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#include <hid_host.h>

extern queue_t outgoing_queue;
extern volatile bool hid_device;
extern volatile bool msc_device;
extern volatile bool tx_pending;
packet_t hid_pkt = {};

// Tiny USB Functions
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
	outgoing_queue.tail = 0;
	outgoing_queue.head = 0;

	hid_device = true;
	msc_device = false;
    tuh_hid_receive_report(dev_addr, instance);
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
	memset(&outgoing_queue, 0, sizeof(outgoing_queue));
	hid_device = false;
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	int8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
	switch (itf_protocol) {
		case HID_ITF_PROTOCOL_KEYBOARD:
			packet_create(&hid_pkt, report, len, HID_KEYBOARD, 0u, (uint16_t)0, 0u, 0u);

		break;
		case HID_ITF_PROTOCOL_MOUSE:
			packet_create(&hid_pkt, report, len, HID_MOUSE, 0u, (uint16_t)0, 0u, 0u);
		break;
	}
	if (queue_push(&outgoing_queue, &hid_pkt)) {
	    tx_pending = true;
	}
	tuh_hid_receive_report(dev_addr, instance);
	 GPIOA->ODR  ^= (1U << 5);
}












