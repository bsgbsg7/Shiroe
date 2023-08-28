#if 1


#include "myTest.h"
#include "driver/lv_port_indev.h"
void test_label()
{
    static lv_obj_t* default_scr;
    default_scr = lv_scr_act();
    lv_obj_t* label = lv_label_create(default_scr);
    lv_label_set_text(label, "神秘藏宝室");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* label2 = lv_label_create(default_scr);
    lv_label_set_text(label2, "helloworld");
    lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 0, 0);

    static lv_style_t style; //定义一个样式
    lv_style_init(&style);   //样式初始化

    lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_text_font(&style, &Font_PuHui);
    lv_obj_add_style(label, &style, LV_STATE_DEFAULT);
}

LV_IMG_DECLARE(img_home);
LV_IMG_DECLARE(img_thumbsup);
void test_image()
{
    // http://lvgl.100ask.org/8.1/widgets/core/img.html
    static lv_obj_t* default_scr;
    default_scr = lv_scr_act();
    lv_obj_t* img_test_1 = lv_img_create(default_scr);
    lv_img_set_src(img_test_1, &img_home);

    //在图片1的基础上绘制
    lv_obj_t* img_test_2 = lv_img_create(img_test_1);
    lv_img_set_src(img_test_2, &img_thumbsup);

    LV_LOG_USER("img_test2_create");
    lv_obj_t* label2 = lv_label_create(default_scr);
    lv_label_set_text(label2, "test_image");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_del_delayed(img_test_1, 2000); //删除对象
    LV_LOG_USER("img_test2_del");
}


lv_obj_t* si_1;
lv_obj_t* label_si_1;
uint32_t slider_value = 0;
void my_slider_timer(lv_timer_t* timer)
{
    /*Use the user_data*/
    //   uint32_t *user_data = (uint32_t *)timer->user_data;
    //   LV_LOG_USER("my_timer called with user data: %d\n", *user_data);
    slider_value = lv_slider_get_value(si_1);
    LV_LOG_USER("value= %d", slider_value);

    lv_label_set_text_fmt(label_si_1, "%d", slider_value);
    lv_obj_align(label_si_1, LV_ALIGN_CENTER, 0, 10);
}

void test_slider(void)
{
    static lv_obj_t* default_scr;
    default_scr = lv_scr_act();
    si_1 = lv_slider_create(default_scr);
    lv_obj_set_pos(si_1, 0, 0);
    lv_slider_set_range(si_1, 0, 100);

    lv_slider_set_value(si_1, 30, 1);
    slider_value = lv_slider_get_value(si_1);
    LV_LOG_USER("value= %d", slider_value);

    // lv_obj_t* img_test_1 = lv_img_create(default_scr);
    // lv_img_set_src(img_test_1,&img_home);

    label_si_1 = lv_label_create(default_scr);
    lv_label_set_text_fmt(label_si_1, "%d", slider_value);
    lv_obj_align(label_si_1, LV_ALIGN_CENTER, 0, 10);

    lv_timer_create(my_slider_timer, 500, &slider_value);
}

void test_arc(void)
{
    static lv_obj_t* default_scr;
    default_scr = lv_scr_act();
    lv_obj_t* arc_1 = lv_arc_create(default_scr);
    lv_obj_align(arc_1, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_range(arc_1, 0, 100);
}

void set_background()
{
    static lv_obj_t* default_scr;
    default_scr = lv_scr_act();
    //背景颜色替换方法1
    static lv_style_t main_style;
    lv_style_init(&main_style);
    lv_style_set_bg_color(&main_style,lv_color_make(255, 255, 255));    //设置黑色
    //lv_style_set_bg_color(&main_style, lv_palette_main(LV_PALETTE_NONE));    //设置黑色
    lv_obj_add_style(default_scr, &main_style, 0);

    // lv_obj_set_style_bg_grad_color(&main_style,lv_color_make(0,0,0),0);
}


typedef struct {
    char name[20];
    uint8_t age;
}USER_DATA;
USER_DATA uaer_data = {
    .name = {"shenmi"},
    .age = 100 };
lv_timer_t* task1;
void lv_task1_hander(lv_timer_t* timer)
{
    LV_LOG_USER("LV_TICK1：%d", lv_tick_get());

    static uint32_t dat = 0;
    LV_LOG_USER("task1 = %d", dat++);

}
lv_timer_t* task2;
void lv_task2_hander(lv_timer_t* timer)
{
    LV_LOG_USER("LV_TICK2：%d", lv_tick_get());

    USER_DATA* data = (USER_DATA*)timer->user_data;
    LV_LOG_USER("task2 = %s,%d", data->name, data->age);
}

void btn1_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("点击BTN1");
        printf("点击BTN1");
        lv_timer_del(task1);
        lv_timer_del(task2);
    }
}
//任务测试
void task_test()
{
    task1 = lv_timer_create(lv_task1_hander, 500, NULL);
    task2 = lv_timer_create(lv_task2_hander, 1000, (void*)&uaer_data);
    LV_LOG_USER("LV_TICK0：%d", lv_tick_get());

    lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn1, 100, 100);
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_ALL, NULL);
}

void lv_obj_test()
{
    lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1, 50, 50);
    lv_obj_set_pos(btn1, 200, 100);
    lv_obj_set_scrollbar_mode(btn1, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scroll_dir(btn1, LV_DIR_HOR);

    lv_obj_t* child1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(child1, 100, 100);
    lv_obj_set_pos(child1, 50, 50);


    static lv_style_t  style1;
    lv_style_init(&style1);  //样式初始化 
    lv_style_set_bg_color(&style1,lv_color_make(0, 0, 0));
    lv_style_set_border_color(&style1, lv_color_make(0, 255, 0));   //设置边框颜色
    
    lv_obj_add_style(child1, &style1, LV_STATE_DEFAULT);

    lv_obj_t* child2 = lv_obj_create(child1);
    lv_obj_set_size(child2, 30, 30);
    lv_obj_set_pos(child2, 0, 0);

    static lv_style_t  style2;
    lv_style_init(&style2);  //样式初始化 
    lv_style_set_bg_color(&style2, lv_color_make(255, 0, 0));   //设置对象颜色
    lv_style_set_border_color(&style2, lv_color_make(0, 0, 255));   //设置边框颜色
    lv_obj_add_style(child2, &style2, LV_STATE_DEFAULT);

    
}

uint8_t choice_child = 0;
bool isSelected = false;
static void scroll_event_cb(lv_event_t* e)
{
    static uint8_t count;
    //static lv_point_t add;
    //printf("count=%d\r\n", count++);
 
    lv_obj_t* cont = lv_event_get_target(e);

    //lv_obj_get_scroll_end(cont, &add);
    //printf("add=%d,%d\r\n", add.x,add.y);
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for (i = 0; i < child_cnt; i++) {
        lv_obj_t* child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if (diff_y >= r) {
            x = r;
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);      //计算平移多少
        if (x == 0)
        {
            printf("child=%d", i);
            choice_child = i;
            isSelected = true;
        }

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);     //修改透明度，越远越暗
    }
}

lv_timer_t* task_select;
void lv_task_select_hander(lv_timer_t* timer)
{
    if (isSelected)
    {
        printf("选择的%d\r\n",choice_child);
        isSelected = false;
    }
}

void scroll_test(void)
{
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 240, 240);        //设置尺寸
    lv_obj_center(cont);    //放到中心位置
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    //垂直填充

    unsigned int i;
    for (i = 0; i < 20; i++) {
        lv_obj_t* btn = lv_btn_create(cont);
        lv_obj_set_width(btn, lv_pct(100));
        lv_obj_set_height(btn, lv_pct(50));

        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "Button %u", i);
    }

    lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0); //一个圈
    lv_obj_set_style_clip_corner(cont, true, 0);    //显示夹角，就是圆圈外的不显示
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);    //设置只能垂直拖拽
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);  //自动锁定拖拽后停留在中间的选项位置
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);     //取消拖拽条

    task_select = lv_timer_create(lv_task_select_hander, 500, NULL);    //打印输出选择的选项


}

static void btn_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
    }
    else if (code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void btn_test()
{
    lv_obj_t* label;

    lv_obj_t* btn3 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn3, btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn3, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn3);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t* btn4 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn4, btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn4, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn4, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn4, LV_SIZE_CONTENT);

    label = lv_label_create(btn4);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);

}

void led_test(void)
{
    lv_obj_t* myLed = lv_led_create(lv_scr_act());
    lv_obj_align(myLed, LV_ALIGN_CENTER, 0, -20);
    lv_led_set_color(myLed,lv_color_make(255,0,0));

    lv_obj_t* myLed2 = lv_led_create(lv_scr_act());
    lv_obj_align(myLed2, LV_ALIGN_CENTER, 0, 20);
    lv_led_set_brightness(myLed2,150);

    lv_obj_t* myLed3 = lv_led_create(lv_scr_act());
    lv_obj_align(myLed3, LV_ALIGN_CENTER, 0, 40);
    lv_led_off(myLed3);

    lv_obj_t* myLed4 = lv_led_create(lv_scr_act());
    lv_obj_align(myLed4, LV_ALIGN_CENTER, 0, 60);
    lv_led_on(myLed4);
}

void sw_test(void)
{
    lv_obj_t* my_sw = lv_switch_create(lv_scr_act());
    lv_obj_align(my_sw, LV_ALIGN_CENTER, 0, -20);
}

static void btnm_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char* txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);
    }
}


static const char* btnm_map[] = { "1", "2", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2", "" };

void btnm_test(void)
{
    lv_obj_t* btnm1 = lv_btnmatrix_create(lv_scr_act());
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_btnmatrix_set_btn_width(btnm1, 10, 2);        /*Make "Action1" twice as wide as "Action2"*/
    lv_btnmatrix_set_btn_ctrl(btnm1, 10, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btnm1, 11, LV_BTNMATRIX_CTRL_CHECKED);
    lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btnm1, btnm_event_handler, LV_EVENT_ALL, NULL);
}

static void btn2_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    if (code == LV_EVENT_DRAW_PART_BEGIN) {
        lv_obj_draw_part_dsc_t* dsc = lv_event_get_param(e);

        /*Change the draw descriptor the 2nd button*/
        if (dsc->id == 1) {
            dsc->rect_dsc->radius = 0;
            if (lv_btnmatrix_get_selected_btn(obj) == dsc->id)  dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_GREY, 3);
            else dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_BLUE);

            dsc->rect_dsc->shadow_width = 6;
            dsc->rect_dsc->shadow_ofs_x = 3;
            dsc->rect_dsc->shadow_ofs_y = 3;
            dsc->label_dsc->color = lv_color_white();
        }
        /*Change the draw descriptor the 3rd button*/
        else if (dsc->id == 2) {
            dsc->rect_dsc->radius = LV_RADIUS_CIRCLE;
            if (lv_btnmatrix_get_selected_btn(obj) == dsc->id)  dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_RED, 3);
            else dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);

            dsc->label_dsc->color = lv_color_white();
        }
        else if (dsc->id == 3) {
            dsc->label_dsc->opa = LV_OPA_TRANSP; /*Hide the text if any*/

        }
    }
    if (code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t* dsc = lv_event_get_param(e);

        /*Add custom content to the 4th button when the button itself was drawn*/
        if (dsc->id == 3) {
            LV_IMG_DECLARE(img_star);
            lv_img_header_t header;
            lv_res_t res = lv_img_decoder_get_info(&img_star, &header);
            if (res != LV_RES_OK) return;

            lv_area_t a;
            a.x1 = dsc->draw_area->x1 + (lv_area_get_width(dsc->draw_area) - header.w) / 2;
            a.x2 = a.x1 + header.w - 1;
            a.y1 = dsc->draw_area->y1 + (lv_area_get_height(dsc->draw_area) - header.h) / 2;
            a.y2 = a.y1 + header.h - 1;

            lv_draw_img_dsc_t img_draw_dsc;
            lv_draw_img_dsc_init(&img_draw_dsc);
            img_draw_dsc.recolor = lv_color_black();
            if (lv_btnmatrix_get_selected_btn(obj) == dsc->id)  img_draw_dsc.recolor_opa = LV_OPA_30;

            lv_draw_img(&a, dsc->draw_area, &img_star, &img_draw_dsc);
        }
    }
}

/**
 * Add custom drawer to the button matrix to customize butons one by one
 */
void btnm2_test(void)
{
    lv_obj_t* btnm = lv_btnmatrix_create(lv_scr_act());
    lv_obj_add_event_cb(btnm, btn2_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_center(btnm);
}

static lv_obj_t* meter;
static void set_value(void* indic, int32_t v)
{
    lv_meter_set_indicator_value(meter, indic, v);
}

void lmeter_test(void)
{
    meter = lv_meter_create(lv_scr_act());
    lv_obj_center(meter);
    lv_obj_set_size(meter, 200, 200);

    /*Add a scale first*/
    lv_meter_scale_t* scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

    lv_meter_indicator_t* indic;

    /*Add a blue arc to the start*/
    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(meter, indic, 0);
    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Make the tick lines blue at the start of the scale*/
    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(meter, indic, 0);
    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Add a red arc to the end*/
    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter, indic, 80);
    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Make the tick lines red at the end of the scale*/
    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(meter, indic, 80);
    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_var(&a, indic);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
    
}

static lv_group_t* g;
static lv_obj_t* t1;
static void msgbox_event_cb(lv_event_t* e)
{
    lv_obj_t* obj = lv_event_get_current_target(e);
    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

void mbox_test(void)
{
    static const char* btns[] = { "Apply", "Close", "" };

    lv_obj_t* mbox1 = lv_msgbox_create(NULL, "Hello", "This is a message box with two buttons.", btns, true);
    lv_obj_add_event_cb(mbox1, msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
}

static void set_angle(void* obj, int32_t v)
{
    lv_arc_set_angles(obj, v,v+10);
    printf("v=%d\r\n", v);
}

void loaderArc_test(void)
{
    /*Create an Arc*/
    lv_obj_t* arc = lv_arc_create(lv_scr_act());
    
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_angles(arc, 0, 0);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_start(&a);
}


void tabview_test(void)
{
    lv_group_t* g = lv_group_create();

    lv_obj_t* scr = lv_scr_act();
   lv_obj_t* tabview = lv_tabview_create(scr, LV_DIR_TOP, LV_DPI_DEF / 3);
   lv_obj_set_size(tabview, lv_obj_get_width(scr), lv_obj_get_height(scr));
   lv_obj_t* t1 = lv_tabview_add_tab(tabview, LV_SYMBOL_WIFI "Text input1");
   lv_obj_t* label1 = lv_label_create(t1);
   lv_label_set_text(label1, "i`m label1");
   lv_obj_t* t2 = lv_tabview_add_tab(tabview, LV_SYMBOL_BELL "Text input2");
   lv_obj_t* label2 = lv_label_create(t2);
   lv_label_set_text(label2, "i`m label2");
   lv_obj_t* t3 = lv_tabview_add_tab(tabview, LV_SYMBOL_WARNING "Text input3");
   lv_obj_t* label3 = lv_label_create(t3);
   lv_label_set_text(label3, "i`m label3");
   lv_tabview_set_act(tabview, 2, LV_ANIM_ON);//选定tab3

    lv_group_add_obj(g, tabview);
    // lv_group_add_obj(g, t2);
    // lv_group_add_obj(g, t3);
   lv_indev_set_group(indev_encoder, g);
}

void tileview_test(void)
{
    lv_group_t* g = lv_group_create();
    // lv_group_set_default(g);

    lv_obj_t* scr = lv_scr_act();
    lv_obj_t* tv = lv_tileview_create(scr);

    static lv_style_t style_tile;
    lv_style_init(&style_tile);
    lv_style_set_bg_color(&style_tile, lv_color_black());
    lv_style_set_bg_opa(&style_tile, LV_OPA_100);
    lv_style_set_border_width(&style_tile, 2);
    lv_style_set_border_color(&style_tile, lv_color_black());

    static lv_style_t chFont_style;
    lv_style_init(&chFont_style);
    //lv_style_set_text_opa(&chFont_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&chFont_style, lv_color_make(255, 255, 255));
    //lv_style_set_text_font(&chFont_style, &ch_font20);

    lv_obj_add_style(scr, &style_tile, 0);

    //应用1
    lv_obj_t* tile1 = lv_tileview_add_tile(tv, 0, 0, LV_DIR_ALL);
    lv_obj_add_style(tile1, &style_tile, 0);
    //显示个图片
    LV_IMG_DECLARE(img_time);
    lv_obj_t* img1 = lv_img_create(tile1);
    lv_img_set_src(img1, &img_time);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, -20);
    //lv_obj_set_size(img1, 100, 100);
    //lv_img_set_zoom(img1, 128); //缩放50%
    //显示标题
    lv_obj_t* label = lv_label_create(tile1);
    lv_obj_add_style(label, &chFont_style,0);
    lv_label_set_text(label, "Time");
    lv_obj_align_to(label,img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_group_add_obj(g, tile1);

    /*应用2 */
    lv_obj_t* tile2 = lv_tileview_add_tile(tv, 1, 0, LV_DIR_ALL);
    lv_obj_add_style(tile2, &style_tile, 0);
    //显示个图片
    LV_IMG_DECLARE(img_weather);
    lv_obj_t* img2 = lv_img_create(tile2);
    lv_img_set_src(img2, &img_weather);
    lv_obj_align(img2, LV_ALIGN_CENTER, 0, -20);
    //lv_obj_set_size(img1, 100, 100);
    //lv_img_set_zoom(img1, 128); //缩放50%
    //显示标题
    lv_obj_t* label2 = lv_label_create(tile2);
    lv_obj_add_style(label2, &chFont_style, 0);
    lv_label_set_text(label2, "Weather");
    lv_obj_align_to(label2, img2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_group_add_obj(g, tile2);

    /*应用2 */
    lv_obj_t* tile3 = lv_tileview_add_tile(tv, 2, 0, LV_DIR_ALL);
    lv_obj_add_style(tile3, &style_tile, 0);
    //显示个图片
    LV_IMG_DECLARE(img_set);
    lv_obj_t* img3 = lv_img_create(tile3);
    lv_img_set_src(img3, &img_set);
    lv_obj_align(img3, LV_ALIGN_CENTER, 0, -20);
    //lv_obj_set_size(img1, 100, 100);
    //lv_img_set_zoom(img1, 128); //缩放50%
    //显示标题
    lv_obj_t* label3 = lv_label_create(tile3);
    lv_obj_add_style(label3, &chFont_style, 0);
    lv_label_set_text(label3, "Set");
    lv_obj_align_to(label3, img3, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_group_add_obj(g, tile3);

    // ///*Tile3: a list*/
    // //lv_obj_t* tile3 = lv_tileview_add_tile(tv, 2, 0, LV_DIR_ALL);
    // //lv_obj_t* list = lv_list_create(tile3);
    // //lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));

    // //lv_list_add_btn(list, NULL, "One");
    // //lv_list_add_btn(list, NULL, "Two");
    // //lv_list_add_btn(list, NULL, "Three");
    // //lv_list_add_btn(list, NULL, "Four");
    // //lv_list_add_btn(list, NULL, "Five");
    // //lv_list_add_btn(list, NULL, "Six");
    // //lv_list_add_btn(list, NULL, "Seven");
    // //lv_list_add_btn(list, NULL, "Eight");
    // //lv_list_add_btn(list, NULL, "Nine");
    // //lv_list_add_btn(list, NULL, "Ten");

    // lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
    // lv_obj_set_size(btn1, 80, 40);
    // lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -100);

    // lv_obj_t* btn2= lv_btn_create(lv_scr_act());
    // lv_obj_set_size(btn2, 80, 40);
    // lv_obj_align_to(btn2,btn1,LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // lv_obj_t* slider = lv_slider_create(lv_scr_act());
    // lv_obj_align_to(slider, btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_indev_set_group(indev_encoder, g);

}

void key_contorl_test()
{
    lv_group_t* g = lv_group_create();
    // lv_group_set_default(g);

    lv_obj_t* btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1, 80, 40);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -100);

    lv_obj_t* btn2= lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn2, 80, 40);
    lv_obj_align_to(btn2,btn1,LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t* btn3 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn3, 80, 40);
    lv_obj_align_to(btn3, btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t* btn4 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn4, 80, 40);
    lv_obj_align_to(btn4, btn3, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t* slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, 100, 40);
    lv_obj_align_to(slider, btn4, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    

    
    lv_group_add_obj(g, btn1);
    lv_group_add_obj(g, btn2);
    lv_group_add_obj(g, btn3);
    // lv_group_add_obj(g, btn4);
    lv_group_add_obj(g, slider);
    lv_indev_set_group(indev_encoder, g);
}

void refresh_test(void)
{
    static int countTime;
    uint32_t dt;
    if((countTime++)%2)
    {
        lv_obj_clean(lv_scr_act());
        dt = micros();
        
        LV_IMG_DECLARE(img_girl);
        lv_obj_t* img1 = lv_img_create(lv_scr_act());
        
        lv_img_set_src(img1, &img_girl);
        lv_img_set_angle(img1,900);  //旋转90度
        lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);

        dt = micros() - dt;
        LV_LOG_USER("dt1=%dus",dt);
    }
    else
    {
        lv_obj_clean(lv_scr_act());
        dt = micros();

        LV_IMG_DECLARE(img_girl2);
        lv_obj_t* img2 = lv_img_create(lv_scr_act());
        
        lv_img_set_src(img2, &img_girl2);
        lv_img_set_angle(img2,900);  //旋转90度
        lv_obj_align(img2, LV_ALIGN_CENTER, 0, 0);

        dt = micros() - dt;
        LV_LOG_USER("dt2=%dus",dt);
    }
    
   
}

void my_test(void)
{
    printf("my_test\r\n");
    
    test_label();
    // test_image();

    // test_slider();
    //test_arc();
    // set_background();
    //task_test();
    //lv_obj_test();
    //scroll_test();
    //btn_test();
    //led_test();
    //sw_test();
    //btnm_test();
    //btnm2_test();
    //lmeter_test();
    //mbox_test();
    //loaderArc_test();
    //tabview_test();
    // tileview_test();
    //key_contorl_test();
    //lv_example_tileview_1();

    // lv_timer_create(refresh_test, 1000, NULL);
    
}

#endif
