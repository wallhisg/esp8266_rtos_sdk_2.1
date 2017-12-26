#include <event/peripheral_event.h>




void peripheral_event_init()
{
    gpio_event_init();
//     uart_event_init();
}

uint8_t peripheral_add_event(event_t index, event_callback callback)
{

}

void peripheral_regist_event(event_t handlerId)
{
}

void peripheral_post_event()
{
    gpio_post_event();
}
