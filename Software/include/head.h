#ifndef HEAD_H
#define HEAD_H

#include <lvgl.h>
#include <Arduino.h>
#include "common.h"
#include <WiFi.h>
#include <Arduino.h>
#include "driver/lv_port_disp.h"
#include "driver/lv_port_indev.h"
#include <Wire.h>
#include <PubSubClient.h>
#if LV_BUILD_EXAMPLES
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
  */
#include "examples/lv_examples.h"
#endif

// #include <lv_demo.h>
#include "demos/lv_demos.h"
#include "myGuiStudy.h"
#include "study/myTest.h"
#include "study/myLVGL.h"
#include "study/wifiscan.h"
#include "study/weather.h"
// #include "app/home/home.h"
// #include "app/music/music.h"
// #include "app/lunar/lunar.h"
// #include "app/lunar/lunarCalender.h"


//HEAD
#include "pins_arduino.h"
#include <Arduino.h>
//wifi
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include "esp_wifi.h"
#include <esp_now.h>
//bluetooth
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEService.h>
//flash
//#include <LittleFS.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>
//devices
// #include "display.h"
// #include "touch.h"
// #include "SDcard.h"

#include "bilibili.h"
// #include "weather.h"

#include "gui.h"

// extern xSemaphoreHandle lv_handler_lock;
// extern xSemaphoreHandle lv_http_lock;
extern lv_obj_t *setting_title;
extern void (*other_app)();

LV_FONT_DECLARE(chinese_hei_16);

extern lv_style_t style_chinese_16_white;
extern lv_style_t style_chinese_16_black;

#endif