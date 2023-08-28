#ifndef _COMMON_H
#define _CONMON_H

#include "TFT_eSPI.h"
#define screenWidth 480
#define screenHeight 320

#define SD_CS 38   //SD卡的CS引脚

// #define USE_DMA

extern TFT_eSPI tft; /* TFT instance */
extern void sd_init();




#endif