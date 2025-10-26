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


#include "msc_host.h"

extern queue_t outgoing_queue;

static uint8_t g_dev_addr;
static uint32_t g_block_count;
static uint16_t g_block_size;
volatile bool transfer_status;
volatile bool transfer_done;
packet_t msc_packet = {};

extern volatile bool msc_device;
extern volatile bool hid_device;

static uint8_t block_buffer[512];

void tuh_msc_mount_cb(uint8_t dev_addr) {
    g_dev_addr = dev_addr;
    uint8_t lun = 0;

    g_block_count = tuh_msc_get_block_count(dev_addr, lun);
    g_block_size  = tuh_msc_get_block_size(dev_addr, lun);
    printf("mounted");

    msc_device  = true;
    hid_device = false;

	packet_create(&msc_packet, NULL, 0, MSC_START,  (uint32_t) g_block_count, (uint16_t) g_block_size, 0u, 0u);
    queue_push(&outgoing_queue, &msc_packet);
    tx_pending = true;
}

void tuh_msc_umount_cb(uint8_t dev_addr) {
    (void)dev_addr;
    msc_device = false;
    g_dev_addr = 0;

	packet_create(&msc_packet, NULL, 0, MSC_END, 0u, (uint16_t) 0, 0u, 0u);
    queue_push(&outgoing_queue, &msc_packet);
}

bool msc_read_complete_cb(uint8_t dev_addr, const tuh_msc_complete_data_t* cb_data)
{
    (void)dev_addr;
    transfer_status = (cb_data->csw->status == 0);
    transfer_done = true;
    return true;
}

bool msc_write_complete_cb(uint8_t dev_addr, const tuh_msc_complete_data_t* cb_data)
{
    (void)dev_addr;
    transfer_status = (cb_data->csw->status == 0);
    transfer_done = true;
    return true;
}

static bool transfer_wait(uint32_t timeout_ms) {
  uint32_t start = HAL_GetTick();
  while (!transfer_done) {
    tuh_task();
    if ((HAL_GetTick() - start) > timeout_ms) {
    	return false;
    }
  }
  return transfer_status;
}

bool msc_read_block(uint32_t lba, uint32_t blocks, uint8_t *out) {
	if(!msc_device) {
		return false;
	}
	transfer_done = false;
	if(!tuh_msc_read10(g_dev_addr, 0, block_buffer, lba, 1, msc_read_complete_cb, 0)) {
		return false;
	}
	if(!transfer_wait(1500)) {
		return false;
	}
	memcpy(out, block_buffer, 512);
	return true;
}

bool msc_write_block(uint32_t lba, uint8_t* in) {
	if(!msc_device) {
		return false;
	}
	memcpy(block_buffer, in, 512);
	transfer_done = false;
	if(!tuh_msc_write10(g_dev_addr, 0, block_buffer, lba, 1, msc_write_complete_cb, 0)) {
		return false;
	}
	if(!transfer_wait(1500)) {
		return false;
	}
	return true;
}

void handle_msc_request(packet_t *pkt) {
	if(pkt->packet_type == MSC_WRITE) {
		uint8_t *data = pkt->payload;
		if(msc_write_block(pkt->lba, data)) {
			packet_create(&msc_packet, NULL, 0, MSC_WRITE_RESPONSE, 0u, (uint16_t) 0, 0u, 0u);
		    queue_push(&outgoing_queue, &msc_packet);
		    return;
		}
	}
	else if(pkt->packet_type == MSC_READ) {
		uint8_t data[512];
		if(msc_read_block(pkt->lba, pkt->block_count, data)) {
			packet_create(&msc_packet, data, sizeof(data), MSC_READ_RESPONSE, 0u, (uint16_t) 0, 0u, 0u);
			queue_push(&outgoing_queue, &msc_packet);
			return;
		}
	}
}









