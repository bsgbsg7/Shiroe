#ifndef __MY_LVGL_H_
#define __MY_LVGL_H_

#include "lvgl.h"
#include "examples/lv_examples.h"
#include "demos/lv_demos.h"
#include <DHT.h>
#include <DHT_U.h>
#include "weather.h"
#include "wifiscan.h"
#include "I2C_BM8563.h"
#include <WiFi.h>


#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/


/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
extern void my_lvgl(void);
// extern uint32_t slider_value;
// extern void refresh_test(void);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif

