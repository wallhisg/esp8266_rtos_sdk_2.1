#include <driver/uart/ring_buffer.h>

static uint8_t rx_buff[UART_RX_BUFFER_SIZE];

ring_buffer_t ringbuff_rx;

void ringbuff_init(void)
{
    ringbuff_rx.head = rx_buff;
    ringbuff_rx.tail = rx_buff;
    ringbuff_rx.status = RING_BUF_EMPTY;
}


void ringbuff_rx_write_one_byte(const uint8_t byte)
{
    if ((byte == LF) || (byte == CR) || (byte == '\0'))
    {
        // remember add event with case RING_BUF_FULL
//         system_regist_event();
    }
    
    if (ringbuff_rx.status == RING_BUF_FULL)
    {
        printf("ring full\r\n");
        return;
    }
    
    *ringbuff_rx.tail = byte;
    ringbuff_rx.tail++;
    
    if (ringbuff_rx.tail == (rx_buff + UART_RX_BUFFER_SIZE))
    {
        ringbuff_rx.tail = rx_buff;
    }
    ringbuff_rx.status = RING_BUF_DATA_PRESENT;
    
    if (ringbuff_rx.tail == ringbuff_rx.head)
    {
        ringbuff_rx.status = RING_BUF_FULL;
    }
    printf("byte: %c", byte);
}

uint8_t ringbuff_rx_read_one_byte()
{
    uint8_t byte = 0;
    
    byte = *ringbuff_rx.head;
    ringbuff_rx.head++;
    
    if (ringbuff_rx.head == (rx_buff + UART_RX_BUFFER_SIZE))
    {
        ringbuff_rx.head = rx_buff;
    }
    
    if (ringbuff_rx.head == ringbuff_rx.tail)
    {
        ringbuff_rx.status = RING_BUF_EMPTY;
    }
    
    ringbuff_rx.status = RING_BUF_DATA_PRESENT;
    
    return byte;
}

uint16_t ringbuff_rx_read_bytes(uint8_t* buff)
{
    uint16_t num_bytes_read = 0;
    while (num_bytes_read < UART_RX_BUFFER_SIZE)
    {
        if (ringbuff_rx.status == RING_BUF_EMPTY)
        {
            break;
        }
        else
        {
            buff[num_bytes_read++] = ringbuff_rx_read_one_byte();
        }
    }
    return num_bytes_read;
}

const ring_buffer_status_e ringbuff_rx_status()
{
    return ringbuff_rx.status;
}
