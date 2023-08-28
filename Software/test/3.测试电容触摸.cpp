#include <lvgl.h>
#include "common.h"
#include <Arduino.h>
#include "driver/lv_port_disp.h"
//#include "driver/lv_port_indev.h"

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

#include "TAMC_GT911.h"

#define TOUCH_SDA  1
#define TOUCH_SCL  5
#define TOUCH_INT 2
#define TOUCH_RST 21
#define TOUCH_WIDTH  320
#define TOUCH_HEIGHT 480

TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

void setup() {
  Serial.begin(115200);
  Serial.println("TAMC_GT911 Example: Ready");
  tp.begin();
  Serial.println("tp.begin();end");
  tp.setRotation(ROTATION_NORMAL);
  Serial.println("tp.setRotation(ROTATION_NORMAL);end");
}

void loop() {
  tp.read();
  if (tp.isTouched){
    for (int i=0; i<tp.touches; i++){
      Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");;
      Serial.print("  x: ");Serial.print(tp.points[i].x);
      Serial.print("  y: ");Serial.print(tp.points[i].y);
      Serial.print("  size: ");Serial.println(tp.points[i].size);
      Serial.println(' ');
    }
  }
}