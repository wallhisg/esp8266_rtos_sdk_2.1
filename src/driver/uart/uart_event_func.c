#include <driver/uart/uart_event_func.h>

extern uint8_t peripheral_add_event(uint16_t index, event_callback callback);
extern void peripheral_regist_event(uint16_t handlerId);
extern void gpio_event_pin2_blinky(void);

void uart_event_init()
{
    peripheral_add_event(PER_EVENT_UART_RX_BUF_FULL, uart_event_full_buffer);
}

void uart_get_events(void)
{
    if (ringbuff_rx_status() != RING_BUF_FULL)
    {
        peripheral_regist_event(PER_EVENT_UART_RX_BUF_FULL);
//         uart_event_full_buffer();
        
    }
}

void uart_event_full_buffer()
{
    peripheral_regist_event(PER_EVENT_GPIO_PIN2_BLINKY);
    printf("uart_event_full_buffer\r\n");
    vTaskDelay (500/portTICK_RATE_MS);
    
}
