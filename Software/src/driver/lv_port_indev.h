
/**
 * @file lv_port_indev_templ.h
 *
 */

 /*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H




#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lib/lvgl/lvgl.h"


/*********************
 *      DEFINES
 *********************/
// //编码器的3个按键

// #define EN_A    14
// #define EN_K    12
// #define EN_B    13

#define TOUCH_SDA  42
#define TOUCH_SCL  41
#define TOUCH_INT 40
#define TOUCH_RST 39
#define TOUCH_WIDTH  320
#define TOUCH_HEIGHT 480

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern void lv_port_indev_init(void);
extern lv_indev_t* indev_encoder;


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
