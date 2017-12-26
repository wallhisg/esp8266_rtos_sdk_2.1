#ifndef _PERIPHERAL_EVENT_H_
#define _PERIPHERAL_EVENT_H_

#include <espressif/c_types.h>
#include <event/event_type.h>
#include <driver/gpio/gpio_event.h>
#include <driver/uart/uart_event_func.h>

void peripheral_event_init();
uint8_t peripheral_add_event(event_t index, event_callback callback);
void peripheral_regist_event(event_t handlerId);
void peripheral_post_event();

#endif  //  _PERIPHERAL_EVENT_H_
