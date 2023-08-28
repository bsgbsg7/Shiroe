// #include <Arduino.h>
// #include <WiFi.h>
// #include "wifiscan.h"

// const char *ssid = "vivoS12";
// const char *password = "7bsgbsg7";

// void wifi_init(){
//   WiFi.mode(WIFI_STA); //设置为STA模式
//   WiFi.disconnect();   //断开当前可能的连接
//   delay(100);
// }

// void wifi_begin()
// {
//   // Serial.println("scan start");
//   // int n = WiFi.scanNetworks(); //扫描并返回搜索到的网络数量，该方法默认会阻塞
//   // Serial.println("scan done");
//   // if (n != 0)
//   // {
//   //   Serial.print(n);
//   //   Serial.println(" networks found");
//   //   for (int i = 0; i < n; ++i)
//   //   {
//   //     Serial.println();
//   //     Serial.print(i + 1);
//   //     Serial.print(":\t");
//   //     Serial.print(WiFi.SSID(i)); //打印网络名称
//   //     Serial.print("\t");
//   //     Serial.print(WiFi.RSSI(i)); //打印信号强度
//   //     Serial.print("\t");
//   //     Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "未加密" : "加密"); //打印是否加密
//   //     delay(10);
//   //   }
//   // }
//     // 开始连接wifi
//   WiFi.begin(ssid, password);

//   // 等待WiFi连接,连接成功打印IP
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(1000);
//     Serial.print("...");
//   }
//   Serial.println("");
//   Serial.println("WiFi Connected!");
// }

// void wifi_stop()
// {
//   WiFi.disconnect(true,false);
// }
