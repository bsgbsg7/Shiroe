// #include "Arduino.h"
// #define pin 3

// void SendData(char addr) // 发送函数。
// {

//   digitalWrite(pin, HIGH); /*开始拉高*/
//   delayMicroseconds(1000);
//   digitalWrite(pin, LOW);     /*开始引导码*/
//   delayMicroseconds(3200);    /*此处延时最少要大于2ms*/
//   for (int i = 0; i < 8; i++) /*总共8位数据  */
//   {
//     digitalWrite(pin, HIGH);
//     if (addr & 0x01) /*3:1表示数据位1,每个位用两个脉冲表示  */
//     {
//       delayMicroseconds(600);
//       digitalWrite(pin, LOW);
//       delayMicroseconds(200);
//     }
//     else /*1：3表示数据位0 ,每个位用两个脉冲表示  */
//     {
//       delayMicroseconds(200);
//       digitalWrite(pin, LOW);
//       delayMicroseconds(600);
//     }
//     addr >>= 1;
//   }
//   digitalWrite(pin, HIGH);
// }

// void show_number1()
// {
//   // 设置音量为20
//   SendData(0x0a); // 清空数字
//   SendData(0x09); // 音量20
//   SendData(0x00);
//   SendData(0x0c); // 设置音量
//   delay(2000);    // 延时

//   // 选取曲目1播放
//   SendData(0x0a); // 清空数字
//   SendData(0x01); // 曲目数字，对应00001.mp3
//   SendData(0x0b); // 选曲播放
//   delay(2000);

//   // 开始播放
//   // SendData(0x11);//开始播放
//   // delay(2000);
// }

// void show_number2()
// {
//   // 设置音量为20
//   SendData(0x0a); // 清空数字
//   SendData(0x09); // 音量20
//   SendData(0x00);
//   SendData(0x0c); // 设置音量
//   delay(2000);    // 延时

//   // 选取曲目2播放
//   SendData(0x0a); // 清空数字
//   SendData(0x02); // 曲目数字，对应00002.mp3
//   SendData(0x0b); // 选曲播放
//   delay(2000);

//   // 开始播放
//   // SendData(0x11);//开始播放,这部分可以没有
//   // delay(2000);
// }

// void setup()
// {
//   pinMode(pin, OUTPUT);
// }

// void loop()
// {

//   // SendData(0x0a);    //清空数字
//   // SendData(0x01);    //曲目号
//   // SendData(0x00);
//   // SendData(0x0b);    //选曲播放
//   // delay(2000);    //延时

//   // //发送0x01
//   // SendData(0x0a);
//   // SendData(0x01);
//   // SendData(0x0b);
//   // delay(2000);

//   // //发送0x07
//   // SendData(0x0a);
//   // SendData(0x07);
//   // SendData(0x0b);
//   // delay(2000);

//   // //发送0x03
//   // SendData(0x0a);
//   // SendData(0x03);
//   // SendData(0x0b);
//   // delay(2000);

//   show_number1(); // 播放音频1
//   delay(5000);
//   show_number2(); // 播放音频2
//   delay(5000);
// }
