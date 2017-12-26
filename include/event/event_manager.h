#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

#include <stdio.h>
#include <event/peripheral_event.h>

void system_event_init(void);
void system_regist_event(void);
void system_post_event(void);

#endif  //  _EVENT_MANAGER_H_
