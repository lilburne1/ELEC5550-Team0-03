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


#include "msc_device.h"

extern volatile bool msc_device;
extern queue_t incoming_queue;
extern queue_t outgoing_queue;
extern volatile bool tx_pending;
extern volatile bool msc_response_ready;


uint32_t g_block_count = 0;
uint32_t g_block_size = 0;

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    (void)lun;
    *block_count = g_block_count;
	*block_size = g_block_size;
}

void tud_msc_inquiry_cb(uint8_t lun,
                        uint8_t vendor_id[8],
                        uint8_t product_id[16],
                        uint8_t product_rev[4]) {
    (void)lun;
    memcpy(vendor_id,  "Bridge  ", 8);
    memcpy(product_id, "MSC Forwarder   ", 16);
    memcpy(product_rev,"1.0 ", 4);
}

int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
	(void) buffer;
	(void) bufsize;
	tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
	return -1;
}

bool tud_msc_test_unit_ready_cb (uint8_t lun)
{
	(void) lun;
    return msc_device;
}

bool msc_request_read(uint32_t lba, uint32_t offset, uint32_t req_bytes, void* buffer) {
	uint16_t block_count = (uint16_t)((req_bytes + (512 - 1)) / 512);

	packet_t outgoing_pkt = {};
	packet_create(&outgoing_pkt, NULL, 0, MSC_READ, lba, block_count, offset, req_bytes);
	queue_push(&outgoing_queue, &outgoing_pkt);

	uint32_t start = HAL_GetTick();
	tx_pending = true;
	msc_response_ready = false;

	while(!msc_response_ready) {
		uart_tx_service_receiver();
		uart_rx_service_receiver();
		tud_task();

		 if ((HAL_GetTick() - start) > 1000) {
		       return false;
		 }
	}
	packet_t incoming_pkt = {};
	if(!queue_pop(&incoming_queue, &incoming_pkt)) {
		return false;
	}
	if(incoming_pkt.len != req_bytes) {
		return false;
	}
	memcpy(buffer, incoming_pkt.payload, req_bytes);
	return true;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
	 (void)lun; (void)offset;
	 return msc_request_read(lba, offset, bufsize, buffer) ? bufsize : -1;
}

bool msc_request_write(uint32_t lba, uint32_t offset, uint32_t req_bytes, uint8_t* buffer) {
	uint16_t block_count = (uint16_t)((req_bytes + (512 - 1)) / 512);

	packet_t outgoing_pkt = {};
	packet_create(&outgoing_pkt, (uint8_t*)buffer, req_bytes, MSC_WRITE, lba, block_count, offset, req_bytes);
	queue_push(&outgoing_queue, &outgoing_pkt);

	uint32_t start = HAL_GetTick();
	tx_pending = true;
	msc_response_ready = false;

	while(!msc_response_ready) {
		uart_tx_service_receiver();
		uart_rx_service_receiver();
		tud_task();

		 if ((HAL_GetTick() - start) > 1000) {
		       return false;
		 }
	}
	packet_t incoming_pkt = {};
	if(!queue_pop(&incoming_queue, &incoming_pkt)) {
		return false;
	}
	return true;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
	 (void)lun; (void)offset;
	 return msc_request_write(lba, offset, bufsize, (uint8_t*) buffer) ? bufsize : -1;
}

void msc_set_parameters(packet_t* pkt) {
	msc_device = true;
	g_block_count = pkt->lba;
	g_block_size = pkt->block_count;
}






