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


#include <laser_rx_sender.h>
extern uint16_t crc16(const uint8_t *data, size_t len);

extern volatile bool acknowledge_received;
extern volatile bool retransmission_needed;
extern volatile bool msc_device;
extern volatile bool hid_device;
extern queue_t incoming_queue;

data_stream_t data_stream;
uint8_t stream_buffer[UART_RX_BUFSIZE];
uint8_t received_buffer[UART_RX_BUFSIZE];
uint32_t read_index = 0;
packet_t recent_pkt = {};

void uart_rx_init(void)
{
    read_index = 0;
    data_stream.data = stream_buffer;
    data_stream.capacity = sizeof(stream_buffer);
    data_stream.length = 0;
    data_stream.escape = false;
    data_stream.dropping = false;

    (void)huart2.Instance->SR;
    (void)huart2.Instance->DR;

    if (HAL_UART_Receive_DMA(&huart2, received_buffer, UART_RX_BUFSIZE) != HAL_OK) {
    	printf("Issue with DMA Receive");
    }
    (void)huart2.Instance->SR;
    (void)huart2.Instance->DR;

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
  if (huart != &huart2) return;

  volatile uint32_t sr = huart->Instance->SR;
  volatile uint32_t dr = huart->Instance->DR;
  (void)sr; (void)dr;

  data_stream.length   = 0;
  data_stream.escape   = false;
  data_stream.dropping = true;

  HAL_UART_DMAStop(huart);
  __HAL_DMA_DISABLE(huart->hdmarx);
  __HAL_DMA_SET_COUNTER(huart->hdmarx, UART_RX_BUFSIZE);
  __HAL_DMA_ENABLE(huart->hdmarx);
  HAL_UART_Receive_DMA(huart, received_buffer, UART_RX_BUFSIZE);
}

bool slip_decode(uint8_t b, size_t *frame_length, bool *frame_dropped)
{
	// End of Packet Delimiter Detected
	if(b == SLIP_END) {
		// Drop 0 Length Frames
        if (data_stream.length == 0) {
            data_stream.length   = 0;
            data_stream.escape   = false;
            data_stream.dropping = false;
            return false;
        }
		// Output length + dropped status
		*frame_length = data_stream.length;
		*frame_dropped = data_stream.dropping;

		// Reset data stream
		data_stream.length = 0;
		data_stream.escape = false;
		data_stream.dropping = false;
		return true;
	}

	// Ignore input as being dropped
	if(data_stream.dropping) {
		return false;
	}

	// SLIP escape characters handling
	if(data_stream.escape) {
		data_stream.escape = false;
		if(b == SLIP_ESC_END) {
			b = SLIP_END;
		}
		else if (b == SLIP_ESC_ESC) {
			b = SLIP_ESC;
		}
		else {
			data_stream.length = 0;
			data_stream.dropping = true;
			return false;
		}
	}
	else if(b == SLIP_ESC){
		data_stream.escape = true;
		return false;
	}

	// Check buffer overrun
	if(data_stream.length < data_stream.capacity) {
		data_stream.data[data_stream.length++] = b;
	}
	else {
		data_stream.length = 0;
		data_stream.escape = 0;
		data_stream.dropping = true;
	}
	return false;
}

bool parse_packet_sender(uint8_t *frame, size_t frame_len, packet_t *packet) {
	uint32_t frame_index = 0;

	if (frame_len < 1 + 2 + 2) return false;

	uint8_t  data_type  = frame[frame_index++];
	uint16_t payload_len = (uint16_t)frame[frame_index] | ((uint16_t)frame[frame_index+1] << 8);
	frame_index +=2;
	if (payload_len > 512) return false;

	uint32_t lba = 0;
	uint16_t block_count = 0;
	uint32_t offset = 0;
	uint32_t req_bytes = 0;

	// MSC Additional Packet Information
	if(data_type == MSC_START || data_type == MSC_WRITE_RESPONSE || data_type == MSC_READ_RESPONSE) {
		lba =
			(uint32_t)frame[frame_index] |
			((uint32_t)frame[frame_index+1] << 8) |
			((uint32_t)frame[frame_index+2] << 16) |
			((uint32_t)frame[frame_index+3] << 24);
		frame_index += 4;
		block_count =
			(uint16_t)frame[frame_index] |
			((uint16_t)frame[frame_index+1] << 8);
		frame_index += 2;
		offset =
			(uint32_t)frame[frame_index] |
			((uint32_t)frame[frame_index+1] << 8) |
			((uint32_t)frame[frame_index+2] << 16) |
			((uint32_t)frame[frame_index+3] << 24);
		frame_index += 4;
		req_bytes =
			(uint32_t)frame[frame_index] |
			((uint32_t)frame[frame_index+1] << 8) |
			((uint32_t)frame[frame_index+2] << 16) |
			((uint32_t)frame[frame_index+3] << 24);
		frame_index += 4;
	}

	 if (payload_len > frame_len - frame_index - 2) return false;

	 uint16_t received_crc =
	 	        ((uint16_t)frame[frame_len - 2] << 8) | (uint16_t)frame[frame_len - 1];
	uint16_t calculated_crc = crc16(frame, frame_len - 2);
	if (calculated_crc != received_crc) {
		return false;
	}
	packet_create(packet, &frame[frame_index], payload_len, data_type, lba, block_count, offset, req_bytes);
	return true;
}

void uart_rx_service_sender(void)
{
	 uint32_t next_write_index = (UART_RX_BUFSIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx)) % UART_RX_BUFSIZE;
	 bool frame_dropped = false;
	 size_t frame_length = 0;

	 while(read_index != next_write_index) {
		 uint8_t b = received_buffer[read_index];
		 read_index = (read_index + 1) % UART_RX_BUFSIZE;
		 if(slip_decode(b, &frame_length, &frame_dropped)) {
			 if(frame_dropped) {
				 queue_not_acknowledgement();
			 }
			 else {
				 if(parse_packet_sender(data_stream.data, frame_length, &recent_pkt)) {
					 if(recent_pkt.packet_type == CONTROL_NACK) {
							retransmission_needed = true;
					 }
					 else if(recent_pkt.packet_type == CONTROL_ACK) {
							acknowledge_received = true;
					 }
					 else {
						 handle_msc_request(&recent_pkt);
					}
				 }
			 }
		 }
	 }
}


