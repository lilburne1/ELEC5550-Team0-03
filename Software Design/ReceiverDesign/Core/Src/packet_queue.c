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


#include "packet_queue.h"

// Packet Queue
void packet_create(packet_t *p, uint8_t const *report, uint16_t len, uint8_t packet_type, uint32_t lba, uint16_t block_count, uint32_t offset, uint32_t req_bytes)
{
	p->packet_type = packet_type;
	if (len > MAX_PACKET_LENGTH) {
		return;
	}
	p->len =len;
	p->lba = lba;
	p-> block_count = block_count;
	p->offset = offset;
	p->req_bytes = req_bytes;
	memcpy(p->payload, report, p->len);
}

bool queue_push(queue_t *q, const packet_t *p) {
	uint16_t head = q->head;
	uint16_t tail = q->tail;
	uint16_t next_pos = ((head + 1) % QUEUE_LENGTH);
	if(next_pos == tail) {
		return false;
	}
	q->queue[q->head] = *p;
	q->head = next_pos;
	return true;
}

bool queue_pop(queue_t *q, packet_t *p) {
	uint16_t tail = q->tail;
	if(q->head == tail) {
		return false;
	}
	uint16_t queue_pos = (tail + 1)%QUEUE_LENGTH;
	*p = q->queue[q->tail];
	q->tail = queue_pos;
	return true;
}
