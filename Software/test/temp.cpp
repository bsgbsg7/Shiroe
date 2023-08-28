
// #include <lvgl.h>
// #include <Arduino.h>
// #include "common.h"
// #include <WiFi.h>
// #include <Arduino.h>
// #include "driver/lv_port_disp.h"
// #include "driver/lv_port_indev.h"
// #include <Wire.h>
// #define Addr_SHT30 0x44
// #define SDA_PIN 48
// #define SCL_PIN 47

// #include "SoftwareSerial.h"

// #include "DFRobotDFPlayerMini.h"

// SoftwareSerial mySoftwareSerial(12, 11);

// DFRobotDFPlayerMini myDFPlayer;

// #if LV_BUILD_EXAMPLES
// /*If you want to use the LVGL examples,
//   make sure to install the lv_examples Arduino library
//   and uncomment the following line.
//   */
// #include "examples/lv_examples.h"
// #endif

// // #include <lv_demo.h>
// #include "demos/lv_demos.h"
// #include "myGuiStudy.h"
// #include "study/myTest.h"
// #include "study/myLVGL.h"
// #include "study/wifiscan.h"
// #include "study/weather.h"
// #include "generated/gui_guider.h"
// #include "generated/events_init.h"
// // 声明函数
// void rtu_pages_memory_leak_test(void);
// void lv_mem_usage_print(lv_mem_monitor_t *mem);

// #define Sensor_AO A0
// #define Sensor_DO 5

// unsigned int sensorValue = 0;

// void setup()
// {
//   Serial.begin(115200); /* prepare for possible serial debug */
//   pinMode(Sensor_DO, INPUT);
//   Wire.begin(SDA_PIN, SCL_PIN);

//   mySoftwareSerial.begin(9600);

//   myDFPlayer.begin(mySoftwareSerial);

//   myDFPlayer.volume(100);

//   String LVGL_Arduino = "Hello Arduino! ";
//   LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

//   Serial.println(LVGL_Arduino);
//   Serial.println("I am LVGL_Arduino");

//   // lv_ui guider_ui;

//   // lv_init();

//   tft.begin();        /* TFT init */
//   tft.setRotation(3); /* Landscape orientation, flipped */

//   // lv_port_disp_init();  // 显示设备初始化
//   // lv_port_indev_init(); // 初始化输入设备，这里是触摸屏或者拨动按键

//   // rtu_pages_memory_leak_test();

//   // my_lvgl();
//   // wifi_init();
//   // // wifi_begin();
//   // // setup_ui(&guider_ui);
//   // // events_init(&guider_ui);

//   // rtu_pages_memory_leak_test();
//   myDFPlayer.loop(1);
//   Serial.println("Setup done");
// }
// /*
//    MQ-2烟雾传感器的使用
// */

// void loop()
// {
//   // lv_timer_handler(); /* let the GUI do its work */

//   sensorValue = analogRead(Sensor_AO);
//   Serial.print("Sensor DO Value = ");
//   Serial.println(sensorValue);
//   Serial.println(digitalRead(Sensor_DO));

//   if (digitalRead(Sensor_DO) == LOW)
//   {
//     Serial.println("Alarm!");
//   }
//   delay(1000);

//   // unsigned int data[6];
//   // Wire.beginTransmission(Addr_SHT30);
//   // Wire.write(0x2C);
//   // Wire.write(0x06);
//   // Wire.endTransmission();
//   // delay(500);
//   // Wire.requestFrom(Addr_SHT30, 6, true);
//   // if (Wire.available() == 6)
//   // {
//   //   data[0] = Wire.read();
//   //   data[1] = Wire.read();
//   //   data[2] = Wire.read();
//   //   data[3] = Wire.read();
//   //   data[4] = Wire.read();
//   //   data[5] = Wire.read();
//   // }

//   // float Temp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
//   // float Humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

//   // Serial.print("相对湿度：");
//   // Serial.print(Humidity);
//   // Serial.println(" %RH");
//   // Serial.print("温度：");
//   // Serial.print(Temp);
//   // Serial.println(" °C");
//   delay(1000);
// }

// void lv_mem_usage_print(lv_mem_monitor_t *mem)
// {

//   Serial.printf("-- lv memory usage --\r\n");

//   Serial.printf("  -- Total heap size--%d\r\n", mem->total_size);

//   Serial.printf("  -- free_cnt--%d\r\n", mem->free_cnt);

//   Serial.printf("  -- Size of available memory--%d\r\n", mem->free_size);

//   Serial.printf("  -- free_biggest_size--%d\r\n", mem->free_biggest_size);

//   Serial.printf("  -- used_cnt--%d\r\n", mem->used_cnt);

//   Serial.printf("  -- Max size of Heap memory used--%d\r\n", mem->max_used);

//   Serial.printf("  -- Percentage used--%d%%\r\n", mem->used_pct);

//   Serial.printf("  -- Amount of fragmentation--%d\r\n", mem->frag_pct);
// }

// void rtu_pages_memory_leak_test(void)
// {

//   Serial.printf("\r\n");

//   Serial.printf("  *  rtu_pages_memory_leak_test  *  \r\n");

//   Serial.printf("------------------------------------\r\n");

//   lv_mem_monitor_t mon;

//   lv_mem_monitor(&mon);

//   lv_mem_usage_print(&mon);
// }
