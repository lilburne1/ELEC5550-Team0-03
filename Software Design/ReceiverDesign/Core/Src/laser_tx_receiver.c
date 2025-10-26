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


#include <laser_tx_receiver.h>

extern queue_t outgoing_queue;
extern volatile bool tx_pending;
extern volatile bool msc_device;
extern volatile bool hid_device;
packet_t acknowledge_packet = {};
packet_t nacknowledge_packet = {};
packet_t sending_packet = {};
uint8_t temp_buf[UART_TX_MAX];

static uint8_t uart_txbuffer[UART_TX_MAX];
static size_t uart_txlen = 0;

volatile bool uart_busy = false;
volatile bool retransmission_needed = false;
volatile bool acknowledge_received = false;
volatile uint32_t transmission_time;

uint16_t crc16(const uint8_t *data, size_t len) {
	uint16_t crc = 0xFFFF;
	while (len--){
	   crc ^= (uint16_t)(*data++) << 8;
	   for (int i=0;i<8;i++)
	      crc = (crc & 0x8000) ? (uint16_t)((crc<<1)^0x1021) : (uint16_t)(crc<<1);
	   }
	 return crc;
}

size_t slip_encode(uint8_t *data, size_t len, uint8_t *out, size_t out_len) {
	size_t write_index = 0;
	out[write_index++] = SLIP_END;

	for(size_t i=0; i<len; i++) {
		uint8_t byte = data[i];
		if(byte == SLIP_END) {
			out[write_index++] = SLIP_ESC;
			out[write_index++] = SLIP_ESC_END;
		}
		else if(byte == SLIP_ESC) {
			out[write_index++] = SLIP_ESC;
			out[write_index++] = SLIP_ESC_ESC;
		}
		else {
			out[write_index++] = byte;
		}
	}
	if((write_index + 1) > out_len) {
		return 0;
	}
	out[write_index++] = SLIP_END;
	return write_index;
}

size_t tx_packetize(packet_t* pkt, uint8_t* frame, size_t out_cap) {
	size_t frame_len = 0;

	frame[frame_len++] = pkt->packet_type;
	frame[frame_len++] = pkt->len & 0xFF;
	frame[frame_len++] = (pkt->len >> 8) & 0xFF;

	if(msc_device) {
		frame[frame_len++] = (uint8_t)(pkt->lba & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->lba >> 8) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->lba >> 16) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->lba >> 24) & 0xFF);

		frame[frame_len++] = (uint8_t)(pkt->block_count & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->block_count >> 8) & 0xFF);

		frame[frame_len++] = (uint8_t)(pkt->offset & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->offset >> 8) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->offset >> 16) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->offset >> 24) & 0xFF);

		frame[frame_len++] = (uint8_t)(pkt->req_bytes & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->req_bytes >> 8) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->req_bytes >> 16) & 0xFF);
		frame[frame_len++] = (uint8_t)((pkt->req_bytes >> 24) & 0xFF);
	}

	memcpy(&frame[frame_len], pkt->payload, pkt->len);
	frame_len = frame_len + pkt->len;

	uint16_t crc = crc16(frame, frame_len);
	frame[frame_len++] = (uint8_t)(crc >> 8);
	frame[frame_len++] = (uint8_t)(crc & 0xFF);

	return frame_len;
}

void queue_acknowledgement() {
	packet_create(&acknowledge_packet, NULL, 0, CONTROL_ACK, 0u, (uint16_t)0, 0u, 0u);
	queue_push(&outgoing_queue, &acknowledge_packet);
	tx_pending = true;
}

void queue_not_acknowledgement() {
	packet_create(&nacknowledge_packet, NULL, 0, CONTROL_NACK, 0u, (uint16_t)0, 0u, 0u);
	queue_push(&outgoing_queue, &nacknowledge_packet);
	tx_pending = true;
}

void uart_tx_receiver() {
	if(uart_busy) {
		return;
	}
	bool packet_available = queue_pop(&outgoing_queue, &sending_packet);
	if(!packet_available) {
		tx_pending = false;
		return;
	}
	size_t frame_len = tx_packetize(&sending_packet, temp_buf, sizeof(temp_buf));
	if(frame_len == 0) {
		return;
	}
	uart_txlen = slip_encode(temp_buf, frame_len, uart_txbuffer, sizeof(uart_txbuffer));
	if (uart_txlen == 0) {
		return;
	}

	HAL_StatusTypeDef st = HAL_UART_Transmit_DMA(&huart2, uart_txbuffer, (uint16_t)uart_txlen);
	if (st != HAL_OK) {
	        return;
	}
	transmission_time = HAL_GetTick();
	uart_busy = true;
	acknowledge_received = false;
	retransmission_needed = false;
}

void uart_retransmission_tx() {
	if(uart_busy) {
		return;
	}
	HAL_StatusTypeDef st = HAL_UART_Transmit_DMA(&huart2, uart_txbuffer, (uint16_t)uart_txlen);
	if (st != HAL_OK) {
	        return;
	}
	transmission_time = HAL_GetTick();
	uart_busy = true;
	acknowledge_received = false;
	retransmission_needed = false;
}

void uart_tx_service_receiver(void) {
	if(!tx_pending) {
		return;
	}
	uint32_t now = HAL_GetTick();
	if(acknowledge_received) {
		uart_tx_receiver();
	}
	else if(retransmission_needed) {
		uart_retransmission_tx();
	}
	else if(msc_device  && (uint32_t) (now - transmission_time) >= MSC_RETRANSMISSION_TIME) {
		uart_retransmission_tx();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
	if(huart != &huart2) {
		return;
	}
	uart_busy = false;
}
