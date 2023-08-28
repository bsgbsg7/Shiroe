#include "gui.h"
#include "app/lunar/lunar.h"
#include "app/ota/ota.h"
#include "app/menu/setting.h"
bool RUNMODE = 0;
lv_obj_t *now_screen;
bool use_lvgl = true;

bool gesture_top = false;
bool gesture_bottom = false;
bool gesture_left = false;
bool gesture_right = false;
/**
 * @brief 每个节点都是类型为 APP_CHAINS 的结构体，
 * 并包含指向下一个节点和上一个节点的指针。
 * 此外，每个节点还包含一个名为 app_load 的成员，
 * 用来存储某种类型的数据
 *
 */
typedef struct APP_CHAINS
{
    struct APP_CHAINS *next;
    struct APP_CHAINS *last;
    load app_load;
} App_chains;
App_chains *app_chains;

void gui_init(uint8_t runMODE)
{
    app_chains = (App_chains *)malloc(sizeof(App_chains));
    app_chains->last = NULL;
    app_chains->next = NULL;
    app_chains->app_load = home_load;
    // switch (runMODE)
    // {
    // case TIME:
    // {
        // app_open(setting_load, LV_SCR_LOAD_ANIM_NONE, 0, 10);
    // }
    // break;
    // case WEATHER:
    // {
    //     // app_open(weather_load, LV_SCR_LOAD_ANIM_NONE, 0, 10);
    // }
    // break;
    // case CALENDAR:
    // {
    // app_open(lunar_load, LV_SCR_LOAD_ANIM_NONE, 0, 10);
    // }
    // break;
    // case BILIBILI:
    // {
    // app_open(ota_load, LV_SCR_LOAD_ANIM_NONE, 200, 10);
    // }
    // break;
    // case CALCULATOR:
    // {
    // app_open(alarm_load, LV_SCR_LOAD_ANIM_NONE, 0, 10);
    // }
    // break;
    // default:
    lv_scr_load(app_chains->app_load());
    // }
}

void app_open(load app_load, lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay_time, bool auto_del, void (*Other_app)())
{
    RUNMODE = !RUNMODE;
    // 确定下一个链表
    app_chains->next = (App_chains *)malloc(sizeof(App_chains));
    app_chains->next->last = app_chains;
    app_chains->next->next = NULL;
    app_chains->next->app_load = app_load;
    app_chains = app_chains->next;

    gesture_bottom = false;
    gesture_left = false;
    gesture_right = false;
    gesture_top = false;

    lv_scr_load_anim((*app_chains->app_load)(), anim_type, time, delay_time, auto_del);

    if (Other_app != NULL)
    {
        // other_app = Other_app;
        use_lvgl = 0;
    }
}

void app_return(lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay_time, bool auto_del)
{
    RUNMODE = !RUNMODE;
    App_chains *temp;
    temp = app_chains;
    app_chains = app_chains->last;
    app_chains->next = NULL;
    free(temp);

    gesture_bottom = false;
    gesture_left = false;
    gesture_right = false;
    gesture_top = false;

    lv_scr_load_anim((*app_chains->app_load)(), anim_type, time, delay_time, auto_del);
}

void null_func()
{
}

/**
 * @brief 这个函数的目的是处理经典手势事件，根据手势的方向和条件执行相应的操作。
 * 在函数的开头，它使用 lv_indev_wait_release() 函数等待输入设备的释放
 * ，然后使用 lv_indev_get_gesture_dir() 函数获取手势的方向。
 *
 * @param event
 */
void gesture_classical(lv_event_t *event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    load back_task = (load)lv_event_get_user_data(event);
    if (gesture_left && dir == LV_DIR_LEFT)
    {
        back_task();
        app_return(LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 20);
    }
    else if (gesture_right && dir == LV_DIR_RIGHT)
    {
        // if (tp.points[0].y > 80 && tp.points[0].y < 320)
        // {
        back_task();
        app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 20);
        // }
    }
    else if (gesture_top && dir == LV_DIR_TOP)
    {
        // if (!gesture_left && !gesture_right && !gesture_bottom)
        // {
        back_task();
        app_return(LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 20);
        // }
    }
    else if (gesture_bottom && dir == LV_DIR_BOTTOM)
    {
        back_task();
        app_return(LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 20);
        // app_open(conpanel_load, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 10);
    }
}

void gesture(bool top, bool bottom, bool left, bool right, void (*back_task)())
{
    gesture_top = top;
    gesture_bottom = bottom;
    gesture_left = left;
    gesture_right = right;

    lv_obj_add_event_cb(now_screen, gesture_classical, LV_EVENT_GESTURE, (void *)back_task);
}