
#include <lvgl.h>
#include "common.h"
#include <Arduino.h>
#include "SD.h"

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    sd_init();
  if (!SD.begin())
  {
    Serial.print(".");
  }
  Serial.println("SD card Ready!");
  Serial.printf("SD.cardSize = %lld \r\n", SD.cardSize());
  Serial.printf("SD.totalBytes = %lld \r\n", SD.totalBytes());
  Serial.printf("SD.usedBytes = %lld \r\n", SD.usedBytes());
  Serial.printf("SD.cardType = %d \r\n", SD.cardType());
  Serial.println(SD.mkdir("/doc1"));
  Serial.printf("is there /doc1? :%d \r\n", SD.exists("/doc1"));
  Serial.printf("is there /test.txt? :%d \r\n", SD.exists("/test.txt"));

  File file = SD.open("/test.txt", FILE_WRITE);
  file.printf("hello bsgbsg7!!!");
  file.close();
  Serial.println("SD Write done!");
  file = SD.open("/test.txt", FILE_READ);
  Serial.println(file.readString());
  file.close();
  SD.end();

  // //读取文件数据
  // file = SD.open("/doc1/test.txt", FILE_READ);
  // if (file)
  // {
  //   Serial.print("文件内容是：");
  //   while (file.available())
  //   {
  //     Serial.print((char)file.read());
  //   }
  // }

}

void loop()
{
}

