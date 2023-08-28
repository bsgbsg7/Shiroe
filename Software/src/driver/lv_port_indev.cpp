/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "../../lib/lvgl/lvgl.h"
#include "common.h"
#include "TAMC_GT911.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

static void mouse_init(void);
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static uint32_t keypad_get_key(void);

static void encoder_init(void);
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static void encoder_handler(void);

static void button_init(void);
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

//自己添加的
static uint8_t key_init(void);//按键初始化设置
static uint8_t key_pressed(void);//EN_K按键是否按下
static void enc_get_new_moves(void);//EN_A EN_B按下

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;
lv_indev_t * indev_mouse;
lv_indev_t * indev_keypad;
lv_indev_t * indev_encoder;
lv_indev_t * indev_button;

static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    /*------------------
     * Mouse
     * -----------------*/

    // /*Initialize your touchpad if you have*/
    // mouse_init();

    // /*Register a mouse input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = mouse_read;
    // indev_mouse = lv_indev_drv_register(&indev_drv);

    // /*Set cursor. For simplicity set a HOME symbol now.*/
    // lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    // lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    // lv_indev_set_cursor(indev_mouse, mouse_cursor);

    // /*------------------
    //  * Keypad
    //  * -----------------*/

    // /*Initialize your keypad or keyboard if you have*/
    // keypad_init();

    // /*Register a keypad input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    // indev_drv.read_cb = keypad_read;
    // indev_keypad = lv_indev_drv_register(&indev_drv);

    // /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
    //  *add objects to the group with `lv_group_add_obj(group, obj)`
    //  *and assign this input device to group to navigate in it:
    //  *`lv_indev_set_group(indev_keypad, group);`*/

    // /*------------------
    //  * Encoder
    //  * -----------------*/

    // /*Initialize your encoder if you have*/
    // encoder_init();

    // /*Register a encoder input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_ENCODER;
    // indev_drv.read_cb = encoder_read;
    // indev_encoder = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/




    // /*------------------
    //  * Button
    //  * -----------------*/

    // /*Initialize your button if you have*/
    // button_init();

    // /*Register a button input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_BUTTON;
    // indev_drv.read_cb = button_read;
    // indev_button = lv_indev_drv_register(&indev_drv);

    // /*Assign buttons to points on the screen*/
    // static const lv_point_t btn_points[2] = {
    //         {10, 10},   /*Button 0 -> x:10; y:10*/
    //         {40, 100},  /*Button 1 -> x:40; y:100*/
    // };
    // lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
    // // uint16_t calData[5] = { 190, 3483, 281, 3510, 3 };
    // uint16_t calData[5] = { 214, 3592, 246, 3544, 3 };
    // tft.setTouch(calData);
    tp.begin();
    tp.setRotation(ROTATION_NORMAL);
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // uint16_t touchX, touchY;

    // bool touched = tft.getTouch( &touchX, &touchY, 600 );

    // if( !touched )
    // {
    //     data->state = LV_INDEV_STATE_REL;
    // }
    // else
    // {
    //     data->state = LV_INDEV_STATE_PR;

    //     /*Set the coordinates*/
    //     data->point.x = touchX;
    //     data->point.y = touchY;

    //     Serial.print( "Data x " );
    //     Serial.print( touchX );

    //     Serial.print( ",Data y " );
    //     Serial.println( touchY );
    // }
  tp.read();
  if (tp.isTouched){
    for (int i=0; i<tp.touches; i++){
      Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");
      Serial.print("  x: ");Serial.print(tp.points[i].x);
      Serial.print("  y: ");Serial.print(tp.points[i].y);
      Serial.print("  size: ");Serial.println(tp.points[i].size);
      Serial.println(' ');
      if(tp.points[i].x > 0 && tp.points[i].x <= TOUCH_WIDTH)
        data->point.y = TOUCH_WIDTH - tp.points[i].x-1;
      if(tp.points[i].y > 0 && tp.points[i].y <= TOUCH_HEIGHT)  
        data->point.x = tp.points[i].y-1;
    }
    data->state = LV_INDEV_STATE_PR;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

     
    (*x) = 0;
    (*y) = 0;
}

/*------------------
 * Mouse
 * -----------------*/

/*Initialize your mouse*/
static void mouse_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /*Get the current x and y coordinates*/
    mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the mouse button is pressed or released*/
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/*Return true is the mouse button is pressed*/
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the mouse is pressed*/
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = 0;
    (*y) = 0;
}

/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    /*Get the current x and y coordinates*/
    mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();
    if(act_key != 0) {
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch(act_key) {
        case 1:
            act_key = LV_KEY_NEXT;
            break;
        case 2:
            act_key = LV_KEY_PREV;
            break;
        case 3:
            act_key = LV_KEY_LEFT;
            break;
        case 4:
            act_key = LV_KEY_RIGHT;
            break;
        case 5:
            act_key = LV_KEY_ENTER;
            break;
        }

        last_key = act_key;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    /*Your code comes here*/

    return 0;
}

/*------------------
 * Encoder
 * -----------------*/

static uint8_t key_init(void)
{ 
    // LV_LOG_USER("static uint8_t key_init(void)");
    // //初始化按键是输入下拉的，按下是高电平
    // pinMode(EN_K,INPUT_PULLDOWN);
    // pinMode(EN_A,INPUT_PULLDOWN);
    // pinMode(EN_B,INPUT_PULLDOWN);

    return 0;
}

/*Initialize your keypad*/
static void encoder_init(void)
{
    /*Your code comes here*/
    //初始化编码器
    key_init();
}

static uint8_t key_pressed(void)
{
    // if (digitalRead(EN_K) == HIGH)
    // {
    //     LV_LOG_USER("LV_KEY_ENTER");
    //     //return 1;//
    //     return LV_KEY_ENTER;
    // }

    return 0;
}


static void enc_get_new_moves(void)
{
    // static uint8_t sta = 0;
    // int16_t new_moves = 0;
    // if (sta == 0)
    // {
    //     sta = 1;
    //     if (digitalRead(EN_A) == HIGH)
    //     {
    //         LV_LOG_USER("EN_A PRESS");
    //         new_moves = -1;
    //         encoder_diff--;
    //     }
    //     if (digitalRead(EN_B) == HIGH)
    //     {
    //         LV_LOG_USER("EN_B PRESS");
    //         new_moves = 1;
    //         encoder_diff++;

    //     }
    // }
    // if ( (digitalRead(EN_A) == LOW) && (digitalRead(EN_B) == LOW) )//按键未按下
    // {
    //     sta = 0;
    // }
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    enc_get_new_moves();
    data->enc_diff = encoder_diff;
    if (key_pressed())
    {
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    encoder_diff = 0;
}



/*Call this function in an interrupt to process encoder events (turn, press)*/
static void encoder_handler(void)
{
    /*Your code comes here*/
    LV_LOG_USER("encoder_handler");
    encoder_diff += 0;
    encoder_state = LV_INDEV_STATE_REL;
}

/*------------------
 * Button
 * -----------------*/

/*Initialize your buttons*/
static void button_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the button*/
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 0; i < 2; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{

    /*Your code comes here*/

    return false;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
