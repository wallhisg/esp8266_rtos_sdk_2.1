#include <event/event_manager.h>

void system_event_init(void)
{
    gpio_event_init();
}
void system_regist_event()
{
    
}

void system_post_event(void)
{
    peripheral_post_event();
}
