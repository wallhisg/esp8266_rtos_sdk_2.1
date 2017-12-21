#include <event/peripheral_event.h>

static peripheral_event_flags per_event_flags = 0;
static peripheral_event_callback per_callback_table[PER_EVENT_NUM];

void peripheral_event_init()
{
    
}

uint8_t peripheral_add_event(uint16_t index, peripheral_event_callback callback)
{
    if (index > PER_EVENT_NUM)
    {
        return -1;
    }
    
    per_callback_table[index] = callback;
    return 0;
}

void peripheral_regist_event(uint16_t handlerId)
{
    per_event_flags |= 1 << handlerId;
}

void peripheral_post_event()
{
    peripheral_event_flags events = per_event_flags;
    per_event_flags = 0;
    
    if (events)
    {
        uint16_t mask;
        uint16_t id;
        
        for (id = 0, mask = 0x01; id <PER_EVENT_NUM; id++, mask <<= 1)
        {
            if ((events & mask) && per_callback_table[id])
            {
                per_callback_table[id]();
            }
        }
    }
}
