/*
 * hid.c
 *
 *  Created on: Aug 24, 2025
 *      Author: benlilburne
 */
#include "hid.h"
#define MAX_LENGTH 1024

hid_queue q;
sending_buffer sendingBuffer;

// Tiny USB Functions
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
	tuh_hid_receive_report(dev_addr, instance);
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
	q.tail = 0;
	q.head = 0;
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	hid_packet p;
	hid_packet_create(&p, report, len);
	hid_queue_push(&q, &p);
	tuh_hid_receive_report(dev_addr, instance);
	tx_pending = true;
}

// HID Packet Queue
void hid_packet_create(hid_packet *p,uint8_t const *report, uint16_t len) {
	p->len = (uint8_t) len;
	memcpy(p->data, report, len);
}

bool hid_queue_push(hid_queue *q, const hid_packet *p) {
	uint16_t head = q->head;
	uint16_t tail = q->tail;
	uint16_t next_pos = ((head + 1) % QUEUE_LENGTH);
	if(next_pos == tail) {
		return false;
	}
	q->buffer[next_pos] = *p;
	q->head = next_pos;
	return true;
}

bool hid_queue_pop(hid_queue *q, hid_packet *out) {
	uint16_t tail = q->tail;
	if(q->head == tail) {
		return false;
	}
	uint16_t queue_pos = (tail + 1)%QUEUE_LENGTH;
	*out = q->buffer[queue_pos];
	q->tail = queue_pos;
	return true;
}











