/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "../../lib/lvgl/lvgl.h"
#include "common.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
/*Change to your screen resolution*/
#define BUF_MODE 3 //模式选择
//模式1，单缓存
//模式2，双缓存
//模式3，PSRAM双缓存
//模式4，DMA+PSRAM

lv_disp_draw_buf_t draw_buf;

#if (BUF_MODE == 1)
    #define COLOR_BUF_SIZE screenWidth*20
    lv_color_t buf[COLOR_BUF_SIZE] ;

#elif (BUF_MODE == 2)
    #define COLOR_BUF_SIZE screenWidth*20
    DMA_ATTR lv_color_t buf[COLOR_BUF_SIZE] ;
    DMA_ATTR lv_color_t buf2[ COLOR_BUF_SIZE ];
#elif(BUF_MODE == 3)
    //EXT_RAM_ATTR
    #define COLOR_BUF_SIZE TFT_WIDTH*TFT_HEIGHT
    // static lv_color_t ext_color_buf[COLOR_BUF_SIZE] __attribute__((at(0x3F800000)));
    lv_color_t *ext_color_buf;
    lv_color_t *ext_color_buf2;
#elif(BUF_MODE == 4)    //全屏的1/4作为缓冲区，当pushPixelsDMA只有32767大小，用PSRAM+DMA时候不能超过这个
    //EXT_RAM_ATTR
    #define COLOR_BUF_SIZE TFT_WIDTH*TFT_HEIGHT/4
    // static lv_color_t ext_color_buf[COLOR_BUF_SIZE] __attribute__((at(0x3F800000)));
    lv_color_t *ext_color_buf;
    lv_color_t *ext_color_buf2;
#endif
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     LVGL requires a buffer where it internally draws the widgets.
     Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     The buffer has to be greater than 1 display row
     
     There are 3 buffering configurations:
     1. Create ONE buffer:
          LVGL will draw the display's content here and writes it to your display
     
      2. Create TWO buffer:
           LVGL will draw the display's content to a buffer and writes it your display.
           You should use DMA to write the buffer's content to the display.
           It will enable LVGL to draw the next part of the screen to the other buffer while
           the data is being sent form the first buffer. It makes rendering and flushing parallel.
     
     3. Double buffering
           Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
           This way LVGL will always provide the whole rendered screen in `flush_cb`
           and you only need to change the frame buffer's address.
     */

    // /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    // /* Example for 2) */
    // static lv_disp_draw_buf_t draw_buf_dsc_2;
    // static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
    // static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    // /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*An other screen sized buffer*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    // static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    // lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    // /*Set up the functions to access to your display*/

    // /*Set the resolution of the display*/
    // disp_drv.hor_res = 480;
    // disp_drv.ver_res = 320;

    // /*Used to copy the buffer's content to the display*/
    // disp_drv.flush_cb = disp_flush;

    // /*Set a display buffer*/
    // disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    // lv_disp_drv_register(&disp_drv);

#if (BUF_MODE == 1)
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, COLOR_BUF_SIZE );  //单缓冲
#elif (BUF_MODE == 2)
    lv_disp_draw_buf_init( &draw_buf, buf, buf2, COLOR_BUF_SIZE );  //双缓冲
#elif(BUF_MODE == 3 || BUF_MODE == 4)
    LV_LOG_USER("befor:ESP.getFreeHeap()：%d,PSRAM=%d,Free_PSRAM=%d",ESP.getFreeHeap(),ESP.getPsramSize(),ESP.getFreePsram());

    ext_color_buf = (lv_color_t*)heap_caps_malloc(COLOR_BUF_SIZE*2, MALLOC_CAP_SPIRAM);
    ext_color_buf2 = (lv_color_t*)heap_caps_malloc(COLOR_BUF_SIZE*2, MALLOC_CAP_SPIRAM);
    lv_disp_draw_buf_init( &draw_buf, ext_color_buf, ext_color_buf2, COLOR_BUF_SIZE ); //使用外部PSRAM
    LV_LOG_USER("ext_color_buf：%d,ext_color_buf2=%d",ext_color_buf,ext_color_buf2);
    LV_LOG_USER("after malloc:ESP.getFreeHeap()：%d,PSRAM=%d,Free_PSRAM=%d",ESP.getFreeHeap(),ESP.getPsramSize(),ESP.getFreePsram());

#endif

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
    //lv_disp_set_bg_color(lv_disp_get_default(),lv_color_make(0,0xff,0));   //设置黑色背景   //需要取消默认主题
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    // /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    // int32_t x;
    // int32_t y;
    // for(y = area->y1; y <= area->y2; y++) {
    //     for(x = area->x1; x <= area->x2; x++) {
    //         /*Put a pixel to the display. For example:*/
    //         /*put_px(x, y, *color_p)*/
    //         color_p++;
    //     }
    // }

    // /*IMPORTANT!!!
    //  *Inform the graphics library that you are ready with the flushing*/
    // lv_disp_flush_ready(disp_drv);
    uint32_t dt_start = micros();

    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );

#if USE_DMA
    tft.setSwapBytes(true);
    tft.pushPixelsDMA( ( uint16_t * )&color_p->full, w * h );
#else
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
#endif
    
    
    tft.endWrite();


    lv_disp_flush_ready( disp_drv );
    uint32_t dt_interval = micros() - dt_start;

#if MY_DEBUG
    #if USE_DMA
        LV_LOG_USER("USEDMA:start_time=%d,interval=%dus",dt_start,dt_interval);
    #else
        LV_LOG_USER("NO DMA:start_time=%d,interval=%dus",dt_start,dt_interval);
    #endif
#endif
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
