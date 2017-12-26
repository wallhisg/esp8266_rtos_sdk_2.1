#ifndef _EVENT_TYPE_H_
#define _EVENT_TYPE_H_

#include <espressif/c_types.h>

// peripheral event define
// GPIO & UART event define0

#define PER_EVENT_NUM       8

typedef enum {
    PER_EVENT_GPIO = 0x00,
    PER_EVENT_GPIO_PIN2_BLINKY = 0x01
} gpio_event_e;

typedef enum {
    PER_EVENT_UART_RX_BUF_EMPTY     = 0x0001,
    PER_EVENT_UART_RX_DATA_PRESENT  = 0x0002,
    PER_EVENT_UART_RX_BUF_FULL      = 0x0004,
    PER_EVENT_UART_CTRL_LETTER      = 0x0008,
    PER_EVENT_UART_TX_BUF_EMPTY     = 0x0100,
    PER_EVENT_UART_TX_DATA_PRESENT  = 0x0200,
    PER_EVENT_UART_TX_BUF_FULL      = 0x0400
} per_event_uart_e;

typedef struct {
    gpio_event_e gpio_event;
    per_event_uart_e uart_event;
} per_event_t;

typedef uint16_t event_t;
typedef uint16_t event_flags_t;
typedef void (*event_callback)(void);

static event_flags_t gpio_event_flag = 0;

static event_callback gpio_callback_table[PER_EVENT_NUM];
static event_callback per_uart_callback_table[PER_EVENT_NUM];
#endif  //  _EVENT_TYPE_H_
