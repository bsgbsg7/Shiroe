#pragma once

#include "head.h"

lv_obj_t *ota_load();
void callback(char *topic, byte *payload, unsigned int length);
void mqtt_reconnect();
static void kb_label_1_event_cb(lv_event_t *e);
void mqtt_timer_cb(lv_timer_t *timer);
int fun_detect(char payload);
void phase_mqtt(int fun_choose);
void weather_phase();
void alarm_phase(String temp);
void time_phase(String temp);
extern I2C_BM8563_DateTypeDef dateStruct;
extern I2C_BM8563 rtc;
extern I2C_BM8563_TimeTypeDef alarmStruct;
extern int music_choose_flag; // 默认播放第一首歌
extern double ampmFlag;
void mqtt_gesture(lv_event_t *e);

extern int tem_now;
extern String position;
extern String weather_condition;
extern String air;
extern String update_time;

