#include <driver/gpio/gpio_event.h>


extern uint8_t peripheral_add_event(uint16_t index, event_callback callback);
extern void peripheral_regist_event(uint16_t handlerId);

void gpio_event_init(void)
{
    
//     peripheral_add_event(PER_EVENT_GPIO_PIN2_BLINKY, gpio_event_pin2_blinky);
    gpio_add_event(PER_EVENT_GPIO_PIN2_BLINKY, gpio_event_pin2_blinky);
}

uint8_t gpio_add_event(event_t event_id, event_callback callback)
{
    if (event_id > PER_EVENT_NUM)
    {
        return -1;
    }
    
    gpio_callback_table[event_id] = callback;
    return 0;
}

void gpio_get_events(void)
{
    gpio_regist_event(PER_EVENT_GPIO_PIN2_BLINKY);
}


void gpio_regist_event(event_t event_id)
{
    gpio_event_flag |= 1 << event_id;
}

void gpio_post_event(void)
{
    event_flags_t events = gpio_event_flag;
    gpio_event_flag = 0;
    
    if (events)
    {
        event_t mask;
        event_t id;
        
        for (id = 0, mask = 0x01; id <PER_EVENT_NUM; id++, mask <<= 1)
        {
            if ((events & mask) && gpio_callback_table[id])
            {
                gpio_callback_table[id]();
            }
        }
    }
}


void gpio_event_pin2_blinky(void)
{
    uint32_t status;
    status = gpio_input_get();
    status = status & GPIO_Pin_2;
    if (status)
    {
        
        GPIO_OUTPUT_SET (2, 0);
        printf("status %x\r\n", status);
    }
    else
    {
        GPIO_OUTPUT_SET (2, 1);
        printf("status %x\r\n", status);
    }
}
