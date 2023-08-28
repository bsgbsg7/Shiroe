#pragma once

#include "head.h"
#include <Arduino.h>
#include "driver/lv_port_indev.h"
#include "../../study/myLVGL.h"
#include "lvgl.h"
#include "examples/lv_examples.h"
#include "demos/lv_demos.h"
#include <DHT.h>
#include <DHT_U.h>
#include "../../study/weather.h"
#include "../../study/wifiscan.h"
#include "I2C_BM8563.h"
#include <WiFi.h>
#include "ArduinoJson.h"

#define DHTPIN 11
#define DHTTYPE DHT11

#ifndef music_pin
#define music_pin 12
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/*********************
	 *      INCLUDES
	 *********************/

	/*********************
	 *      DEFINES
	 *********************/

	/**********************
	 *      TYPEDEFS
	 **********************/
	typedef struct
	{
		lv_obj_t *screen;
		bool screen_del;
		lv_obj_t *screen_cont_1;
		lv_obj_t *screen_arc_1;
		lv_obj_t *screen_label_6;
		lv_obj_t *screen_line_1;
		lv_obj_t *screen_label_8;
		lv_obj_t *screen_line_3;
		lv_obj_t *screen_line_2;
		lv_obj_t *screen_line_4;
		lv_obj_t *screen_label_16;
		lv_obj_t *screen_label_12;
		lv_obj_t *screen_label_3;
		lv_obj_t *screen_label_7;
		lv_obj_t *screen_line_5;
		lv_obj_t *screen_label_2;
		lv_obj_t *screen_label_10;
		lv_obj_t *screen_label_4;
		lv_obj_t *screen_label_15;
		lv_obj_t *screen_label_5;
		lv_obj_t *screen_label_13;
		lv_obj_t *screen_label_11;
		lv_obj_t *screen_label_14;
		lv_obj_t *screen_arc_2;
		lv_obj_t *screen_arc_6;
		lv_obj_t *screen_label_26;
		lv_obj_t *screen_arc_5;
		lv_obj_t *screen_arc_4;
		lv_obj_t *screen_label_17;
		lv_obj_t *screen_label_18;
		lv_obj_t *screen_arc_3;
		lv_obj_t *screen_label_21;
		lv_obj_t *screen_label_19;
		lv_obj_t *screen_label_20;
		lv_obj_t *screen_label_22;
		lv_obj_t *screen_label_23;
		lv_obj_t *screen_label_24;
		lv_obj_t *screen_label_25;
		lv_obj_t *screen_label_27;
		lv_obj_t *screen_imgbtn_1;
		lv_obj_t *screen_imgbtn_1_label;
		lv_obj_t *screen_label_28;
	} lv_ui;

	LV_IMG_DECLARE(_1_alpha_100x100);

// PCF8563
#define BM8563_I2C_SDA 47
#define BM8563_I2C_SCL 48

	typedef struct _lv_clock
	{
		lv_obj_t *time_label;	 // 时间标签
		lv_obj_t *date_label;	 // 日期标签
		lv_obj_t *weekday_label; // 星期标签
	} lv_clock_t;

	typedef struct _lv_computer
	{
		lv_obj_t *cpu_temperature_label;
		lv_obj_t *cpu_occupation_label;
		lv_obj_t *gpu_temperature_label;
		lv_obj_t *gpu_occupation_label;
		lv_obj_t *cpu_label1;
		lv_obj_t *gpu_label1;
		lv_obj_t *cpu_label2;
		lv_obj_t *gpu_label2;
	} lv_computer_t;

	typedef struct _lv_weather
	{
		lv_obj_t *position;
		lv_obj_t *tem_now;
		lv_obj_t *tem_high_low;
		lv_obj_t *weather_air;
		lv_obj_t *update_time;
	} lv_weather_t;

	// typedef struct
	// {
	// 	String position;
	// 	int tem_now;
	// 	int tem_high_low;
	// 	String weather_air;
	// 	String update_time;
	// } My_Weather_t;

	typedef struct _lv_dht11
	{
		lv_obj_t *temperature;
		lv_obj_t *humidity;
		lv_obj_t *temperature_arc;
		lv_obj_t *humidity_arc;
	} lv_dht11_t;

	typedef struct
	{
		lv_obj_t *screen;
		bool screen_del;
		lv_obj_t *screen_ta_1;
		lv_obj_t *screen_label_1;
		lv_obj_t *screen_sw_1;
		lv_obj_t *screen_label_2;
		lv_obj_t *screen_label_3;
		lv_obj_t *screen_ta_2;
		lv_obj_t *screen_btn_1;
		lv_obj_t *screen_btn_1_label;
	} lv_home_config_t;


	void save_event(lv_event_t *e);
	extern void weather_phase();
	extern void alarm_phase(String temp);
	extern void time_phase(String temp);
	extern lv_ui guider_ui;
	lv_obj_t *home_config_load();
	static void time_change(lv_event_t *e);
	static void alarm_change(lv_event_t *e);
	void gesture_home(lv_event_t *event);
	String extractTimeFromDateTime(String dateTimeString);
	void weather_init();
	/**********************
	 * GLOBAL PROTOTYPES
	 **********************/
	extern lv_obj_t *home_load();
	// extern uint32_t slider_value;
	// extern void refresh_test(void);
	extern lv_home_config_t home_config_ui;
	// extern bool Delay(uint32_t time);
	/**********************
	 *      MACROS
	 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif
