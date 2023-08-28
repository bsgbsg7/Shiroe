#if 1

#ifndef __MY_TEST_H_
#define __MY_TEST_H_


#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/

#include "lvgl.h"

#include "examples/lv_examples.h"
#include "demos/lv_demos.h"
/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
extern void my_test(void);
extern uint32_t slider_value;
extern void refresh_test(void);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif


#endif
