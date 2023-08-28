// #include <Arduino.h>
// #include <WiFi.h>
// #include "weather.h"
// #include <HTTPClient.h>
// #include <WiFiClientSecure.h>
// #include <ArduinoJson.h>

// /* 天气现象代码: https://docs.seniverse.com/api/start/code.html */
// int getIconCode(String cond_code)
// {
//   int code = 0;

//   code = atoi(cond_code.c_str());
//   if ((code < 0) || (code > 39))
//   {
//     return 39;
//   }

//   return code;
// }

// const char *host = "api.seniverse.com"; // 网络服务器地址
// const int httpPort = 80;                // http端口80

// // 向服务器发送HTTP请求
// void updateWeather(weather_info_t *data, String key, String location)
// {
//   const char *ssid = "vivoS12";
//   const char *password = "7bsgbsg7";
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());

//   Serial.println("");
//   Serial.println("WiFi Conected!");

//   WiFiClient client;
//   const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(3) + 130;
//   DynamicJsonDocument doc(capacity);

//   String httpRequest = String("GET /v3/weather/now.json?key=SCXC4Mj_0aB96cZup&location=qingdao&language=en") + " HTTP/1.1\r\n" + // 请求的是首页信息，HTTP版本号是1.1
//                        "Host: " + host + "\r\n" +                                                                                        // 请求的网址信息
//                        "Connection: close\r\n" +                                                                                         // 信息发送完毕后断开连接
//                        "\r\n";                                                                                                           // 表示整个请求信息结束，空行代表整个结束；若有请求体信息，则在此空行之后加

//   // 通过串口输出连接服务器名称以便查阅连接服务器的网址
//   Serial.print("Connecting to ");
//   Serial.print(host);

//   // 80端口连接服务器
//   if (client.connect(host, 80))
//   {
//     Serial.println(" Success!"); // 连接成功后串口输出“Success”信息

//     // wificlient and serial all belong to stream class
//     client.print(httpRequest); // 向服务器发送HTTP请求

//     Serial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
//     Serial.println(httpRequest);

//     // 等待响应
//     Serial.println("Web Server Response:");
//     while (client.connected() || client.available())
//     { // 服务器保持连接
//       if (client.available())
//       {
//         String json = client.readStringUntil('\n'); // 等待响应并且读取响应信息
//         Serial.print(json);
//         deserializeJson(doc, json);

//         JsonObject results_0 = doc["results"][0];
//         String city = results_0["location"]["name"];
//         data->city = city;
//         String temperature = results_0["now"]["temperature"];
//         data->temperature = temperature;
//         String updateDate = results_0["last_update"];
//         data->updateDate = updateDate;
//         // strcpy(data->city,results_0["location"]["name"].as<const char *>());
//         // Serial.print(data->city);
//         // strcpy(data->temperature,results_0["now"]["temperature"].as<const char *>());
//         // Serial.print(data->temperature);
//         // strcpy(data->updateDate,results_0["last_update"].as<const char *>());
//         // Serial.print(data->updateDate);
//       }
//       /*
//         响应信息中的第一个 空行 分隔响应头和响应体
//       */
//     }

//     client.stop();                      // 断开与服务器的连接
//     Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
//     Serial.print(host);
//   }
//   else
//   { // 如果连接不成功则通过串口输出“连接失败”信息
//     Serial.println(" connection failed!");
//     client.stop();
//   }



  // const char *host = "api.seniverse.com"; // 网络服务器地址
  // const int httpPort = 80;

  // WiFiClient client;
  // weather_info_t *weatherInfo;
  // JsonObject results_0;
  // // const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(3) + 130;
  // DynamicJsonDocument doc(1400);

  // String httpRequest = String("GET /v3/weather/now.json?key=SCXC4Mj_0aB96cZup&location=qingdao&language=en") + " HTTP/1.1\r\n" + // 请求的是首页信息，HTTP版本号是1.1
  //                      "Host: " + host + "\r\n" +                                                                                // 请求的网址信息
  //                      "Connection: close\r\n" +                                                                                 // 信息发送完毕后断开连接
  //                      "\r\n";                                                                                                   // 表示整个请求信息结束，空行代表整个结束；若有请求体信息，则在此空行之后加

  // // 通过串口输出连接服务器名称以便查阅连接服务器的网址
  // Serial.print("Connecting to ");
  // Serial.print(host);

  // // 80端口连接服务器
  // if (client.connect(host, 80))
  // {
  //   Serial.println(" Success!"); // 连接成功后串口输出“Success”信息

  //   // wificlient and serial all belong to stream class
  //   client.print(httpRequest); // 向服务器发送HTTP请求

  //   Serial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
  //   Serial.println(httpRequest);

  //   // 等待响应
  //   Serial.println("Web Server Response:");
  //   while (client.connected() || client.available())
  //   { // 服务器保持连接
  //     if (client.available())
  //     {
  //       String json = client.readStringUntil('\n'); // 等待响应并且读取响应信息
  //       Serial.print(json);
  //       // deserializeJson(doc, json);
  //       // results_0 = doc["results"][0];

  //       // strcpy(weatherInfo.city, results_0["location"]["name"].as<const char *>());
  //       // Serial.print(weatherInfo.city);
  //       // strcpy(weatherInfo.temperature, results_0["now"]["temperature"].as<const char *>());
  //       // Serial.print(weatherInfo.temperature);
  //       // strcpy(weatherInfo.updateDate, results_0["last_update"].as<const char *>());
  //       // Serial.print(weatherInfo.updateDate);
  //     }
  //     /*
  //       响应信息中的第一个 空行 分隔响应头和响应体
  //     */
  //   }

  //   client.stop();                      // 断开与服务器的连接
  //   Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
  //   Serial.print(host);
  //   // String city = results_0["location"]["name"];
  //   // weatherInfo->city = city;
  //   // String temperature = results_0["now"]["temperature"];
  //   // weatherInfo->temperature = temperature;
  //   // String updateDate = results_0["last_update"];
  //   // weatherInfo->updateDate = updateDate;
  // }
  // else
  // { // 如果连接不成功则通过串口输出“连接失败”信息
  //   Serial.println(" connection failed!");
  //   client.stop();
  // }

  
// }
