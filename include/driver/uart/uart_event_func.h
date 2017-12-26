#ifndef _UART_EVENT_H_
#define _UART_EVENT_H_

#include <stdio.h>
#include <espressif/esp_common.h>
#include <event/event_type.h>
#include <driver/uart/uart.h>


void uart_event_init();
void uart_get_events(void);
void uart_event_full_buffer();

#endif  //  _UART_EVENT_H_
