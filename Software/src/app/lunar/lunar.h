#pragma once
#include "head.h"
// #include "app/lunar/lunarCalender.h"
typedef struct _lv_lunar
{
    lv_obj_t *lunar_year;    
    lv_obj_t *lunar_monthdate;    
    lv_obj_t *lunar_jieqi;    
    lv_obj_t *lunar_advice;    
} lv_lunar_t;

lv_obj_t *lunar_load();
void gesture_lunar(lv_event_t *event);
extern I2C_BM8563_DateTypeDef dateStruct;
extern I2C_BM8563 rtc;
LV_IMG_DECLARE(_rabbit_alpha_197x264);
LV_IMG_DECLARE(_bull_alpha_197x264);
LV_IMG_DECLARE(_chicken_alpha_197x264);
LV_IMG_DECLARE(_dog_alpha_197x264);
LV_IMG_DECLARE(_mouse_alpha_197x264);
LV_IMG_DECLARE(_monkey_alpha_197x264);
LV_IMG_DECLARE(_pig_alpha_197x264);
LV_IMG_DECLARE(_sheep_alpha_197x264);
LV_IMG_DECLARE(_snake_alpha_197x264);
LV_IMG_DECLARE(_tiger_alpha_197x264);
LV_IMG_DECLARE(_hourse_alpha_197x264);
void advice_cb(lv_timer_t *timer);
void advice_init();
extern WiFiClient advice_client;;