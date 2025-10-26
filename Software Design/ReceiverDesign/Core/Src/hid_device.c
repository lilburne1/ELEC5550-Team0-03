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

#include "hid_device.h"
packet_t received_hid_pkt = {};

void send_hid_report(packet_t *p) {
	if(p->packet_type == HID_KEYBOARD) {
		tud_hid_report(REPORT_ID_KEYBOARD, p->payload, p->len);
	}
	else if(p->packet_type == HID_MOUSE) {
		  if(p->len == 3) {
		        uint8_t report[5] = {
		            p->payload[0],
		            p->payload[1],
		            p->payload[2],
		            0,
		            0
		        };
		        tud_hid_report(REPORT_ID_MOUSE, report, 5);
		    }
		  else {
		        tud_hid_report(REPORT_ID_MOUSE, p->payload, p->len);
		  }
//		tud_hid_report(REPORT_ID_MOUSE, p->payload, p->len);
	}
}

void hid_task(void) {
	if(hid_device) {
		if(tud_hid_ready()) {
			if(queue_pop(&incoming_queue, &received_hid_pkt)) {
				send_hid_report(&received_hid_pkt);
			}
		}
	}
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  // not Implemented
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // not implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
	  // not implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

void tud_mount_cb(void)
{
//	 not implemented
}

void tud_umount_cb(void)
{
//	  not implemented
}





