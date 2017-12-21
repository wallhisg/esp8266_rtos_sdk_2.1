#ifndef _PERIPHERAL_EVENT_H_
#define _PERIPHERAL_EVENT_H_

#include <espressif/c_types.h>

#include <driver/uart/uart_event.h>

#define PER_EVENT_NUM       8
typedef uint16_t peripheral_event_flags;
typedef void (*peripheral_event_callback)(void);

#define PER_EVENT_GPIO              0b0000000000000000
#define PER_EVENT_GPIO_PIN2_BLINKY  0b0000000000000010
#define PER_EVENT_UART_RX           0b0000000000010000
#define PER_EVENT_UART_TX           0b0000000000100000

void peripheral_event_init();
uint8_t peripheral_add_event(uint16_t index, peripheral_event_callback callback);
void peripheral_regist_event(uint16_t handlerId);
void peripheral_post_event();

#endif  //  _PERIPHERAL_EVENT_H_
