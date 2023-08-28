// #include "head.h"
// const char *ssid = "vivoS12";
// const char *password = "7bsgbsg7";

// lv_ui guider_ui;
// xSemaphoreHandle lv_handler_lock;
// lv_style_t style_chinese_16_white;
// lv_style_t style_chinese_16_black;
// bool open_finish = 0;
// void open_anim(void *p)
// {
//   uint8_t red = 63;
//   uint8_t green = 72;
//   uint8_t blue = 204;
//   uint32_t color = tft.color565(red, green, blue);
//   int x = screenWidth / 2, y = screenHeight / 3, R = 25;
//   double x1 = 0, y1 = 0, d = 0;
//   double x2 = 0, y2 = 0;
//   double x3 = 0, y3 = 0;
//   double x1b = 0, y1b = 0;
//   double x2b = 0, y2b = 0;
//   double x3b = 0, y3b = 0;
//   double pi = 3.1415926535;
//   double a1 = -90, a2 = 30, a3 = 150;
//   double a1b = -90, a2b = 30, a3b = 150;

//   double a1_v = 0;
//   double a2_v = 0;
//   double a3_v = 0;

//   double a1b_v = 0;
//   double a2b_v = 0;
//   double a3b_v = 0;
//   while (a1 < 30)
//   {
//     x1 = R * cos(a1 * pi / 180) + x;
//     y1 = R * sin(a1 * pi / 180) + y;
//     tft.drawPixel((int)x1, (int)y1, color);
//     x2 = R * cos(a2 * pi / 180) + x;
//     y2 = R * sin(a2 * pi / 180) + y;
//     tft.drawPixel((int)x2, (int)y2, color);
//     x3 = R * cos(a3 * pi / 180) + x;
//     y3 = R * sin(a3 * pi / 180) + y;
//     tft.drawPixel((int)x3, (int)y3, color);
//     delay(1);
//     a1_v += 0.001;
//     a2_v += 0.001;
//     a3_v += 0.001;
//     a1 += a1_v + 4 * pi / 180;
//     a2 += a2_v + 4 * pi / 180;
//     a3 += a3_v + 4 * pi / 180;
//   }
//   delay(300);
//   while (y3 > y - 2 * R)
//   {
//     tft.drawPixel((int)x3, (int)y3, color);
//     tft.drawPixel((int)x2, (int)y2, color);
//     tft.drawPixel((int)x1, (int)y1, color);
//     y3--;
//     x1 += pow(3, 0.5) / 2;
//     y1 += 0.5;
//     x2 -= pow(3, 0.5) / 2;
//     y2 += 0.5;
//     delay(10);
//   }
//   delay(100);
//   while (a1b < 0)
//   {
//     x1b = 2 * R * cos(a1b * pi / 180) + x;
//     y1b = 2 * R * sin(a1b * pi / 180) + y;
//     tft.drawPixel((int)x1b, (int)y1b, color);
//     x2b = 2 * R * cos(a2b * pi / 180) + x;
//     y2b = 2 * R * sin(a2b * pi / 180) + y;
//     tft.drawPixel((int)x2b, (int)y2b, color);
//     x3b = 2 * R * cos(a3b * pi / 180) + x;
//     y3b = 2 * R * sin(a3b * pi / 180) + y;
//     tft.drawPixel((int)x3b, (int)y3b, color);
//     delay(1);
//     a1b_v += 0.01;
//     a2b_v += 0.01;
//     a3b_v += 0.01;
//     a1b += a1b_v + 2 * pi / 180;
//     a2b += a1b_v + 2 * pi / 180;
//     a3b += a1b_v + 2 * pi / 180;
//   }
//   open_finish = 1;
//   vTaskDelete(NULL);
// }

// void setup()
// {
//   Serial.begin(115200); /* prepare for possible serial debug */

//   lv_init();

//   tft.begin();        /* TFT init */
//   tft.setRotation(3); /* Landscape orientation, flipped */

//   lv_port_disp_init();  // 显示设备初始化
//   lv_port_indev_init(); // 初始化输入设备，这里是触摸屏或者拨动按键

//   WiFi.begin(ssid, password);

//   xTaskCreatePinnedToCore(open_anim, NULL, 4096, NULL, 1, NULL, 0);
//   lv_handler_lock = xSemaphoreCreateMutex();
//   // vTaskStartScheduler();
//   if (xSemaphoreTake(lv_handler_lock, portMAX_DELAY))
//   {
//     gui_init();
//     while (1)
//     {
//       if (open_finish)
//       {
//         break;
//       }
//       delay(50);
//     }
//     delay(2000);
//     xSemaphoreGive(lv_handler_lock);
//   }

//   Serial.println("Setup done");
// }

// void loop()
// {
//   if (xSemaphoreTake(lv_handler_lock, portMAX_DELAY))
//   {
//     lv_timer_handler();
//     xSemaphoreGive(lv_handler_lock);
//   }
// }











// #include "head.h"
// const char *ssid = "vivoS12";
// const char *password = "7bsgbsg7";

// lv_ui guider_ui;
// lv_home_config_t home_config_ui;
// xSemaphoreHandle lv_handler_lock;
// xSemaphoreHandle lv_http_lock;
// lv_style_t style_chinese_16_white;
// lv_style_t style_chinese_16_black;
// bool open_finish = 0;
// void open_anim(void *p)
// {
//   uint8_t red = 63;
//   uint8_t green = 72;
//   uint8_t blue = 204;
//   uint32_t color = tft.color565(red, green, blue);
//   int x = screenWidth / 2, y = screenHeight / 3, R = 25;
//   double x1 = 0, y1 = 0, d = 0;
//   double x2 = 0, y2 = 0;
//   double x3 = 0, y3 = 0;
//   double x1b = 0, y1b = 0;
//   double x2b = 0, y2b = 0;
//   double x3b = 0, y3b = 0;
//   double pi = 3.1415926535;
//   double a1 = -90, a2 = 30, a3 = 150;
//   double a1b = -90, a2b = 30, a3b = 150;

//   double a1_v = 0;
//   double a2_v = 0;
//   double a3_v = 0;

//   double a1b_v = 0;
//   double a2b_v = 0;
//   double a3b_v = 0;
//   while (a1 < 30)
//   {
//     x1 = R * cos(a1 * pi / 180) + x;
//     y1 = R * sin(a1 * pi / 180) + y;
//     tft.drawPixel((int)x1, (int)y1, color);
//     x2 = R * cos(a2 * pi / 180) + x;
//     y2 = R * sin(a2 * pi / 180) + y;
//     tft.drawPixel((int)x2, (int)y2, color);
//     x3 = R * cos(a3 * pi / 180) + x;
//     y3 = R * sin(a3 * pi / 180) + y;
//     tft.drawPixel((int)x3, (int)y3, color);
//     delay(1);
//     a1_v += 0.001;
//     a2_v += 0.001;
//     a3_v += 0.001;
//     a1 += a1_v + 4 * pi / 180;
//     a2 += a2_v + 4 * pi / 180;
//     a3 += a3_v + 4 * pi / 180;
//   }
//   delay(300);
//   while (y3 > y - 2 * R)
//   {
//     tft.drawPixel((int)x3, (int)y3, color);
//     tft.drawPixel((int)x2, (int)y2, color);
//     tft.drawPixel((int)x1, (int)y1, color);
//     y3--;
//     x1 += pow(3, 0.5) / 2;
//     y1 += 0.5;
//     x2 -= pow(3, 0.5) / 2;
//     y2 += 0.5;
//     delay(10);
//   }
//   delay(100);
//   while (a1b < 0)
//   {
//     x1b = 2 * R * cos(a1b * pi / 180) + x;
//     y1b = 2 * R * sin(a1b * pi / 180) + y;
//     tft.drawPixel((int)x1b, (int)y1b, color);
//     x2b = 2 * R * cos(a2b * pi / 180) + x;
//     y2b = 2 * R * sin(a2b * pi / 180) + y;
//     tft.drawPixel((int)x2b, (int)y2b, color);
//     x3b = 2 * R * cos(a3b * pi / 180) + x;
//     y3b = 2 * R * sin(a3b * pi / 180) + y;
//     tft.drawPixel((int)x3b, (int)y3b, color);
//     delay(1);
//     a1b_v += 0.01;
//     a2b_v += 0.01;
//     a3b_v += 0.01;
//     a1b += a1b_v + 2 * pi / 180;
//     a2b += a1b_v + 2 * pi / 180;
//     a3b += a1b_v + 2 * pi / 180;
//   }
//   open_finish = 1;
//   vTaskDelete(NULL);
// }

// void setup()
// {
//   Serial.begin(115200); /* prepare for possible serial debug */

//   lv_init();

//   tft.begin();        /* TFT init */
//   tft.setRotation(3); /* Landscape orientation, flipped */

//   lv_port_disp_init();  // 显示设备初始化
//   lv_port_indev_init(); // 初始化输入设备，这里是触摸屏或者拨动按键

//   WiFi.begin(ssid, password);

//   // xTaskCreatePinnedToCore(open_anim, NULL, 4096, NULL, 1, NULL, 0);
//   lv_handler_lock = xSemaphoreCreateMutex();
//   lv_http_lock = xSemaphoreCreateMutex();
//   // vTaskStartScheduler();
//   if (xSemaphoreTake(lv_handler_lock, portMAX_DELAY))
//   {
//     gui_init();
//     while (1)
//     {
//       // if (open_finish)
//       if (1)
//       {
//         break;
//       }
//       delay(50);
//     }
//     delay(2000);
//     xSemaphoreGive(lv_handler_lock);
//   }

//   Serial.println("Setup done");
// }

// void loop()
// {
//   if (xSemaphoreTake(lv_handler_lock, portMAX_DELAY))
//   {
//     lv_timer_handler();
//     xSemaphoreGive(lv_handler_lock);
//   }
// }
