#ifndef __MUSIC_H_
#define __MUSIC_H_

#include "head.h"

#ifndef music_pin
#define music_pin 12
#endif

#ifdef __cplusplus
extern "C" {
#endif
void SendData(char addr);
void show_number1();
void show_number2();
void show_number3();
lv_obj_t *alarm_load();
void music_alarm_change();
void music_alarm_timer_cb(lv_timer_t *timer);
void alarm_cancel_event(lv_event_t *e);
LV_IMG_DECLARE(_1_alpha_155x160);
extern int music_choose_flag;
#ifdef __cplusplus
} /* extern "C" */
#endif


#endif

