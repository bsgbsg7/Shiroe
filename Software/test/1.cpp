
// #include <lvgl.h>
// #include <Arduino.h>
// #include "common.h"
// #include <WiFi.h>
// #include <Arduino.h>
// #include "driver/lv_port_disp.h"
// #include "driver/lv_port_indev.h"
// #include <Wire.h>
// #define Addr_SHT30 0x44
// #define SDA_PIN 47
// #define SCL_PIN 48
// #include "SoftwareSerial.h"

// #include "DFRobotDFPlayerMini.h"

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


// void setup()
// {
//   Serial.begin(115200); /* prepare for possible serial debug */

//   Wire.begin(SDA_PIN, SCL_PIN);

//   Serial.println("Setup done");
// }
// /*
//    MQ-2烟雾传感器的使用
// */

// void loop()
// {

//   unsigned int data[6];
//   Wire.beginTransmission(Addr_SHT30);
//   Wire.write(0x2C);
//   Wire.write(0x06);
//   Wire.endTransmission();
//   delay(500);
//   Wire.requestFrom(Addr_SHT30, 6, true);
//   if (Wire.available() == 6)
//   {
//     data[0] = Wire.read();
//     data[1] = Wire.read();
//     data[2] = Wire.read();
//     data[3] = Wire.read();
//     data[4] = Wire.read();
//     data[5] = Wire.read();
//   }

//   float Temp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
//   float Humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

//   Serial.print("相对湿度：");
//   Serial.print(Humidity);
//   Serial.println(" %RH");
//   Serial.print("温度：");
//   Serial.print(Temp);
//   Serial.println(" °C");
//   delay(1000);
// }
