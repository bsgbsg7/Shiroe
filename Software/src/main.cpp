#include "head.h"
const char *ssid = "vivoS12";
const char *password = "7bsgbsg7";
void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  lv_init();

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */

  lv_port_disp_init();  // 显示设备初始化
  lv_port_indev_init(); // 初始化输入设备，这里是触摸屏或者拨动按键
  WiFi.begin(ssid, password);
  pinMode(music_pin,OUTPUT);
  gui_init();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  // LV_LOG_USER("slider_value=%d",slider_value);
}



