#ifndef _GPIO_EVENT_H_
#define _GPIO_EVENT_H_

#include <stdio.h>
#include <espressif/c_types.h>
#include <event/event_type.h>
#include <driver/gpio/gpio.h>

void gpio_event_init(void);
uint8_t gpio_add_event(event_t event_id, event_callback callback);
void gpio_get_events(void);
void gpio_regist_event(event_t event_id);
void gpio_post_event(void);
void gpio_event_pin2_blinky(void);

#endif  //  _GPIO_EVENT_H_
