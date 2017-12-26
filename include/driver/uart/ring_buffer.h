#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdio.h>
#include <espressif/c_types.h>

#define LF      0x0A    //  '\n'
#define CR      0x0D    //  '\r'

#define UART_TX_BUFFER_SIZE     64
#define UART_RX_BUFFER_SIZE     8

typedef enum {
    RING_BUF_EMPTY = (1 << 0),
    RING_BUF_DATA_PRESENT = (1 << 1),
    RING_BUF_FULL = (1 << 2)
} ring_buffer_status_e;

typedef struct {
    uint8_t* head;
    uint8_t* tail;
    ring_buffer_status_e status;
} ring_buffer_t;

typedef ring_buffer_t* ring_buffer_ptr;

void ringbuff_init(void);
void ringbuff_rx_write_one_byte(const uint8_t byte);

uint8_t ringbuff_rx_read_one_byte(void);
uint16_t ringbuff_rx_read_bytes(uint8_t* buff);
const ring_buffer_status_e ringbuff_rx_status();

#endif  //  _RING_BUFFER_H_
