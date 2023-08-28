// #include <Arduino.h>
// #include "driver/lv_port_indev.h"
// #include "myLVGL.h"


// void demo1()
// {

//     lv_obj_t *obj = lv_obj_create(lv_scr_act());
//     // LV_PCT(百分比)
//     lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
//     // lv_obj_set_size(obj,480,320);

//     lv_obj_t *label = lv_label_create(obj);
//     lv_label_set_text(label, "Hello Bsgbsg7!");
//     lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

//     // static lv_style_t style; //定义一个样式
//     // lv_style_init(&style);   //样式初始化

//     // lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_BLUE));
//     // lv_style_set_text_font(&style, &Font_PuHui);
//     // lv_obj_add_style(label, &style, LV_STATE_DEFAULT);
// }

// void demo2()
// {
//     /**
//         样式存储在lv_style_t变量中。样式变量应该是静态、全局或动态分配的。
//         也就是它们不能是函数中的局部变量,因为当函数结束时它们会被销毁。
//         样式初始化示例：
//     */
//     static lv_style_t style_obj;
//     lv_style_init(&style_obj);

//     /**
//         设置样式属性:
//         lv_style_set_<property_name>(&style, <value>);
//     */
//     lv_style_set_bg_color(&style_obj, lv_palette_main(LV_PALETTE_YELLOW));  // 设置背景色
//     lv_style_set_text_color(&style_obj, lv_palette_main(LV_PALETTE_GREEN)); // 设置文字颜色
//     lv_style_set_bg_opa(&style_obj, LV_OPA_COVER);                          // 设置透明度

//     // 创建对象
//     lv_obj_t *obj = lv_obj_create(lv_scr_act());
//     lv_obj_t *obj2 = lv_obj_create(lv_scr_act());
//     lv_obj_align(obj2, LV_ALIGN_CENTER, 0, 0);

//     // 注意本地样式具有最高的优先级
//     // 本地样式
//     // lv_obj_set_style_bg_color(obj,  lv_palette_main(LV_PALETTE_YELLOW), 0);    // 设置背景色
//     // lv_obj_set_style_bg_color(obj2,  lv_palette_main(LV_PALETTE_AMBER), 0);   // 设置背景色
//     // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_STATE_PRESSED);	                  // 设置背景透明度

//     /**
//         添加样式到对象:
//         lv_obj_add_style(obj, &style, <selector>)
//         参数“obj”就是要R-ed 值 (不能是互斥，否添加到的对象，
//         “style”是指向样式变量的指针，
//         <selector>是应添加样式的部分和状态的,O则就是清除标志，没法合并)。
//     */
//     // 添加默认状态
//     lv_obj_add_style(obj, &style_obj, 0);  // 默认状态: LV_STATE_DEFAULT
//     lv_obj_add_style(obj2, &style_obj, 0); // 默认状态: LV_STATE_DEFAULT

//     // 按下后的样式
//     lv_obj_add_style(obj, &style_obj, LV_STATE_PRESSED);  // 按下状态，当对象被按下的时候应用该样式
//     lv_obj_add_style(obj2, &style_obj, LV_STATE_PRESSED); // 按下状态，当对象被按下的时候应用该样式
//     // 滑杆示例(不需要深入研究，体会部件样式的部分和状态即可)，看里面的注释：体验流程
//     // lv_example_slider_2();
// }

// static void my_demo3_event_cb(lv_event_t *e)
// {
//     lv_obj_t *obj = lv_event_get_target(e);            // 获取触发事件的对象
//     lv_obj_t *parent = lv_event_get_current_target(e); // 获取触发事件对象的父对象(事件冒泡才有)
//     /*
//         注意此处的parent获取到了最外层的，即obj1
//     */
//     lv_event_code_t code = lv_event_get_code(e);             // 获取当前部件触发的事件代码
//     lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e); // 获取添加事件时传递的用户数据

//     switch (code)
//     {
//     case LV_EVENT_PRESSED:
//         lv_label_set_text(label, "LV_EVENT_PRESSED");
//         lv_obj_align_to(label, obj, LV_ALIGN_CENTER, 0, 0);
//         lv_obj_set_style_bg_color(parent, lv_color_hex(0xc43e1c), 0); // 通过本地样式(私有样式)设置背景色
//         lv_obj_set_style_bg_color(obj, lv_color_hex(0xc43e1c), 0);    // 通过本地样式(私有样式)设置背景色
//         printf("LV_EVENT_PRESSED\n");
//         break;
//     case LV_EVENT_CLICKED:
//         lv_obj_remove_local_style_prop(parent, LV_STYLE_BG_COLOR, 0); // 删除通过本地样式(私有样式)设置的背景色
//         lv_obj_remove_local_style_prop(obj, LV_STYLE_BG_COLOR, 0);    // 删除通过本地样式(私有样式)设置的背景色
//         lv_label_set_text(label, "Bsgbsg7");
//         lv_obj_align_to(label, obj, LV_ALIGN_CENTER, 0, 0);
//         printf("LV_EVENT_CLICKED\n");
//         break;
//     default:
//         // printf("NONE\n");
//         break;
//     }
// }
// void demo3()
// {
//     // 事件冒泡
//     /* 创建一个基础对象 obj1 */
//     lv_obj_t *obj1 = lv_obj_create(lv_scr_act());
//     lv_obj_set_size(obj1, LV_PCT(100), LV_PCT(100));
//     lv_obj_center(obj1); // 将对象与其父对象的中心对齐，这里的父对象是屏幕：lv_scr_act()

//     /* 以 obj1 创建一个基础对象 obj2 */
//     lv_obj_t *obj2 = lv_obj_create(obj1);
//     lv_obj_set_size(obj2, LV_PCT(80), LV_PCT(80));
//     lv_obj_center(obj2);                             // 将对象与其父对象的中心对齐，这里的父对象是屏幕：obj1
//     lv_obj_add_flag(obj2, LV_OBJ_FLAG_EVENT_BUBBLE); // 启用事件冒泡，将接收到的所有事件传播给父级

//     /* 以 obj2 创建一个基础对象 obj3 */
//     lv_obj_t *obj3 = lv_obj_create(obj2);
//     lv_obj_set_size(obj3, LV_PCT(60), LV_PCT(60));
//     lv_obj_center(obj3);                             // 将对象与其父对象的中心对齐，这里的父对象是屏幕：obj2
//     lv_obj_add_flag(obj3, LV_OBJ_FLAG_EVENT_BUBBLE); // 启用事件冒泡，将接收到的所有事件传播给父级

//     /* 以 obj3 创建一个基础对象 obj4 */
//     lv_obj_t *obj4 = lv_obj_create(obj3);
//     lv_obj_set_size(obj4, LV_PCT(40), LV_PCT(40));
//     lv_obj_center(obj4);                             // 将对象与其父对象的中心对齐，这里的父对象是屏幕：obj3
//     lv_obj_add_flag(obj4, LV_OBJ_FLAG_EVENT_BUBBLE); // 启用事件冒泡，将接收到的所有事件传播给父级

//     /* 以屏幕为父类，创建一个label部件(对象) */
//     lv_obj_t *label = lv_label_create(lv_scr_act());
//     lv_label_set_text(label, "Bsgbsg7");                 // 设置label展示的文字
//     lv_obj_align_to(label, obj1, LV_ALIGN_CENTER, 0, 0); // 将label相对于obj1对齐

//     // 将给obj1添加事件回调函数，所有的事件类型都能触发该回调函数，传递的数据为label
//     lv_obj_add_event_cb(obj1, my_demo3_event_cb, LV_EVENT_ALL, label);
//     /*
//        注意此处应该理解为obj2,3,4被触发之后，会将事件传给父类，直到传到能够处理此事件的父类（即lv_obj_add_event_cb内部的obj）
//        如果是obj2，则都会传给obj2处理，obj1无法处理
//    */
// }

// static void my_demo4_event_cb(lv_event_t *e)
// {
//     lv_obj_t *obj = lv_event_get_target(e);                  // 获取触发事件的部件(对象)
//     lv_event_code_t code = lv_event_get_code(e);             // 获取当前部件(对象)触发的事件代码
//     lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e); // 获取添加事件时传递的用户数据
//     switch (code)
//     {
//     case LV_EVENT_PRESSED:
//         lv_label_set_text(label, "LV_EVENT_PRESSED");
//         lv_obj_remove_local_style_prop(obj, LV_STYLE_BG_COLOR, 0);            // 删除通过本地样式(私有样式)设置的背景色
//         lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), 0); // 通过本地样式(私有样式)设置背景色
//         printf("LV_EVENT_PRESSED\n");
//         break;
//     case LV_EVENT_LONG_PRESSED:
//         lv_label_set_text(label, "LV_EVENT_LONG_PRESSED");
//         lv_obj_remove_local_style_prop(obj, LV_STYLE_BG_COLOR, 0);           // 删除通过本地样式(私有样式)设置的背景色
//         lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), 0); // 通过本地样式(私有样式)设置背景色
//         printf("LV_EVENT_LONG_PRESSED\n");
//         break;
//     default:
//         lv_label_set_text(label, lv_label_get_text(label));
//         // lv_obj_remove_local_style_prop(obj, LV_STYLE_BG_COLOR, 0);      // 删除通过本地样式(私有样式)设置的背景色
//         // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
//         // printf("default\n");
//         break;
//     }
// }
// void demo4()
// {
//     /* 创建基础部件(对象) */
//     lv_obj_t *obj = lv_obj_create(lv_scr_act());
//     lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));

//     /* 创建label部件(对象) */
//     lv_obj_t *label = lv_label_create(lv_scr_act());
//     lv_label_set_text(label, "Hello LVGL!"); // 设置label展示的文字
//     lv_obj_center(label);                    // 将对象与其父对象的中心对齐,这里的父对象是屏幕:lv_scr_act()

//     // 为obj1添加事件回调函数,所有的事件类型都能触发该回调函数
//     lv_obj_add_event_cb(obj, my_demo4_event_cb, LV_EVENT_ALL, label);
// }

// static void demo5_btn_toggle_event_cb(lv_event_t *e)
// {
//     lv_obj_t *obj = lv_event_get_target(e);      // 获取触发事件的部件(对象)
//     lv_event_code_t code = lv_event_get_code(e); // 获取当前部件(对象)触发的事件代码

//     switch (code)
//     {
//     case LV_EVENT_VALUE_CHANGED:
//         printf("LV_EVENT_VALUE_CHANGED\n");
//         break;
//     default:
//         // printf("NONE\n");
//         break;
//     }
// }
// void demo5()
// {
//     /* 创建一个btn部件(对象) */
//     lv_obj_t *btn = lv_btn_create(lv_scr_act()); // 创建一个btn部件(对象),他的父对象是活动屏幕对象

//     // 修改按钮部件（对象）矩形背景部分的样式
//     lv_obj_set_style_bg_color(btn, lv_color_hex(0x1e1e1e), LV_PART_MAIN | LV_STATE_PRESSED);

//     // 打开了 LV_OBJ_FLAG_CHECKABLE ，当对象被点击时有选中切换(Toggle)状态的效果
//     // 其触发的是 LV_EVENT_VALUE_CHANGED 事件类型
//     // lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

//     // 添加事件回调函数,只有LV_EVENT_VALUE_CHANGED能触发该回调函数，注意需要打开flag
//     lv_obj_add_event_cb(btn, demo5_btn_toggle_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
// }

// static void demo6_sw_event_handler(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *sw = lv_event_get_target(e);

//     if (code == LV_EVENT_VALUE_CHANGED)
//     {
//         // 判断开关状态
//         /* 方式1 */
//         // 返回 bool 类型， 开-1 ； 关-2
//         // if(lv_obj_has_state(sw, LV_STATE_CHECKED))
//         //    LV_LOG_USER("ON!");
//         // else
//         //    LV_LOG_USER("OFF!");

//         // 更简洁的写法
//         LV_LOG_USER("State: %s", lv_obj_has_state(sw, LV_STATE_CHECKED) ? "ON" : "OFF");
//         // printf("State: %s", lv_obj_has_state(sw, LV_STATE_CHECKED) ? "ON" : "OFF");

//         /* 方式2 */
//         // if(lv_obj_get_state(sw) & LV_STATE_CHECKED)
//         //     LV_LOG_USER("ON!");
//         // else
//         //     LV_LOG_USER("OFF!");
//         // LV_LOG_USER("State: %s", (lv_obj_get_state(sw) & LV_STATE_CHECKED) ? "ON" : "OFF");
//     }
// }
// void demo6()
// {
//     /* 创建一个 switch 部件(对象) */
//     lv_obj_t *sw = lv_switch_create(lv_scr_act()); // 创建一个 switch 部件(对象),他的父对象是活动屏幕对象
//     lv_obj_center(sw);                             // 方法1：让对象居中，简洁
//     // lv_obj_align(sw, LV_ALIGN_CENTER, 0, 0);            // 方法2：让对象居中，较为灵活

//     // 修改开关对象的大小，注意比例不能是 1:1 (比如：宽高都是100)，否则只能看到一个大圆
//     lv_obj_set_size(sw, 200, 100);

//     // 开操作
//     // lv_obj_add_state(sw, LV_STATE_CHECKED);                       // 开关默认处于关闭状态，这里设置为打开状态
//     // lv_obj_add_state(sw, LV_STATE_CHECKED | LV_STATE_DISABLED);   // 当前状态是开，并且不可更改

//     // 关操作
//     // lv_obj_clear_state(sw, LV_STATE_CHECKED);	        // 关
//     // lv_obj_add_state(sw, LV_STATE_DISABLED); 		        // 当前状态是关，并且不可更改

//     // 清除禁用状态，一般由其他部件(外部)清除
//     // lv_obj_clear_state(sw, LV_STATE_ DISABLED);       // 清除禁用状态，按钮可正常使用

//     // 添加事件，当我们点击开关，改变开关的状态时，会触发 LV_EVENT_VALUE_CHANGED 事件类型
//     // 当然我们可以处理他触发的其他事件类型，比如： LV_EVENT_CLICKED
//     lv_obj_add_event_cb(sw, demo6_sw_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
// }

// static void demo7_btn_event_handler(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *btn = lv_event_get_target(e);
//     lv_obj_t *dd = (lv_obj_t *)lv_event_get_user_data(e);

//     if (code == LV_EVENT_VALUE_CHANGED)
//     {
//         // 由于 btn 在初始化时已经设置了 LV_OBJ_FLAG_CHECKABLE
//         // 那么这里就可以检查 btn 当前的状态，达到打开或关闭下拉列表的目的
//         // 这里的用法请参考 3_5_1 和 3_4_1
//         if (lv_obj_has_state(btn, LV_STATE_CHECKED))
//             lv_dropdown_open(dd);
//         else
//             lv_dropdown_close(dd);
//     }
// }
// static void demo7_dd_event_handler(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *obj = lv_event_get_target(e);

//     if (code == LV_EVENT_VALUE_CHANGED)
//     {
//         LV_LOG_USER("%d", lv_dropdown_get_selected(obj)); // 获取选项的索引值，从0开始
//         char tmp_buf[32];
//         lv_dropdown_get_selected_str(obj, tmp_buf, sizeof(tmp_buf)); // 获取选项的内容
//         LV_LOG_USER("%s", tmp_buf);
//     }
//     else if (code == LV_EVENT_CLICKED)
//     {
//         // 改变下拉列表中列表的样式
//         // 这些修改可以在初始化的时候就设置好，你也可以像下面这样动态地修改样式
//         lv_obj_t *dd_list = lv_dropdown_get_list(obj);
//         if (dd_list != NULL)
//         {
//             // 需要在 lv_conf.h 中打开宏 LV_FONT_MONTSERRAT_28，要使用其他内置字体同理
//             // 注意：如果你初始化时使用了中文字库，并且设置了中文选项，这里设置之后内容将不能展示出来
//             // lv_obj_set_style_text_font(dd_list, &lv_font_montserrat_28, 0);

//             // 修改列表的背景颜色
//             // 这里只修改颜色，你还可以修改其他样式属性
//             lv_obj_set_style_bg_color(dd_list, lv_color_hex(0xc43e1c), 0);
//             // lv_obj_set_style_bg_color(dd_list, lv_color_hex(0xc43e1c), LV_PART_MAIN); // 这样写和上面那个是一样的

//             // 修改滚动条的样式
//             // 这里只修改颜色，你还可以修改其他样式属性
//             lv_obj_set_style_border_color(dd_list, lv_color_hex(0xcc3e1c), LV_PART_SCROLLBAR);

//             // 修改当按下、选中或按下+选中选项时的样式
//             // 这里只修改颜色，你还可以修改其他样式属性
//             lv_obj_set_style_bg_color(dd_list, lv_color_hex(0xafbeac), LV_PART_SELECTED);
//         }
//     }
// }
// void demo7(void)
// {

// #if 0
//     // 使用物理按键控制，注意上面要包含 "lv_drivers/win32drv/win32drv.h"，
//     // LVGL按键控制，视频教程： https://www.bilibili.com/video/BV1Ya411r7K2?p=19
//     // 创建一个组，稍后将需要使用键盘或编码器或按钮控制的部件(对象)添加进去，并且将输入设备和组关联
//     // 如果将这个组设置为默认组，那么对于那些在创建时会添加到默认组的部件(对象)就可以省略 lv_group_add_obj()
//     lv_group_t * g = lv_group_create();

//     // 将上面创建的组设置为默认组
//     // 如果稍后创建的部件(对象)，使用默认组那必须要在其创建之前设置好默认组，否则不生效
//     lv_group_set_default(g);

//     // 将输入设备和组关联(使用前先打开上面注释掉的头文件)
//     lv_indev_set_group(lv_win32_keypad_device_object, g);     // 键盘
//     lv_indev_set_group(lv_win32_encoder_device_object, g);      // 鼠标上的滚轮(编码器)

// #endif // 0

//     /* 创建一个 lv_dropdown 部件(对象) */
//     lv_obj_t *dd = lv_dropdown_create(lv_scr_act()); // 创建一个 lv_dropdown 部件(对象),他的父对象是活动屏幕对象

//     // 将部件(对象)添加到组，如果设置了默认组，这里可以省略，因为 lv_dropdown 是 LV_OBJ_CLASS_GROUP_DEF_TRUE
//     // lv_group_add_obj(g, dd);

// #if 1
//     lv_dropdown_set_options(dd, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10"); // 写法1：添加选项，索引从0开始
// #if 0
//     lv_dropdown_set_options(dd, "1\n"
//                                 "2\n"
//                                 "3\n"
//                                 "4\n"
//                                 "5\n"
//                                 "6\n"
//                                 "7\n"
//                                 "8\n"
//                                 "9\n"
//                                 "10"); // 写法2：添加选项(连续写两个双引号相当于将两者连起来，相当于上面的方法1)
// #endif // 写法1：添加选项，索引从0开始
// #endif // 写法2：添加选项(连续写两个双引号相当于将两者连起来，相当于上面的方法1)

//     lv_obj_center(dd); // 方法1：让对象居中，简洁
//     // lv_obj_align(dd, LV_ALIGN_CENTER, 0, 0);            // 方法2：让对象居中，较为灵活

// #if 0
//     // 获取下拉列表中的列表来修改样式或其他修改
//     lv_obj_t * dd_list = lv_dropdown_get_list(dd);

// #if 0
//     // 显示中文
//     // 修改列表所使用的字体，如果要展示中文则要在这里设置
//     LV_FONT_DECLARE(lv_font_source_han_sans_bold_14);   // 声明字体，可以放到函数外部
//     lv_obj_set_style_text_font(dd, &lv_font_source_han_sans_bold_14, 0);        // 设置新的字体(下拉列表的按钮)
//     lv_obj_set_style_text_font(dd_list, &lv_font_source_han_sans_bold_14, 0);   // 设置新的字体(下拉列表中的列表)
//     lv_dropdown_set_options(dd, "一\n二\n三\n四\n五\n六\n七\n八\n九\n十");      // 设置中文选项
//     //lv_dropdown_set_symbol(dd, LV_SYMBOL_CALL);   // 注意：如果使用自定义的字库，并且你的字库中没有这些符号，那么下拉列表的符号就不会显示了
// #endif

//     // 使用内置字体
//     lv_obj_set_style_text_font(dd, &lv_font_montserrat_30, 0);          // 设置新的字体(下拉列表的按钮)
//     lv_obj_set_style_text_font(dd_list, &lv_font_montserrat_30, 0);     // 设置新的字体(下拉列表中的列表)

//     // 设置字体对齐位置
//     //lv_obj_set_style_text_align(dd, LV_TEXT_ALIGN_CENTER, 0);           // 这样设置下拉列表的按钮的文字对齐位置不会生效，可以在 LV_EVENT_DRAW_MAIN 事件中修改，但是不建议修改，详情请阅读 lv_dropdown.c 的源码
//     lv_obj_set_style_text_align(dd_list, LV_TEXT_ALIGN_RIGHT, 0);       // 设置下拉列表中的列表的文字对齐

// #endif // 0

//     // lv_dropdown_add_option(dd, "11", 10);         // 追加一个选项
//     // lv_dropdown_add_option(dd, "十一", 10);       // 追加一个选项(使用中文的时候)
//     // lv_dropdown_set_selected(dd, 3);              // 设置默认选中的选项，索引从0开始
//     // lv_dropdown_set_symbol(dd, LV_SYMBOL_CALL);   // 设置按钮显示的字符
//     // lv_dropdown_set_text(dd, "Some text");        // 设置当选中选项之后展示的内容，如果没有这句，那么选中的是什么就展示什么

// #if 0
//     // 改变列表创建的方向
//     // 下拉列表按钮上的符号会跟随 所设置的方向自动调整(左侧或右侧)
//     // 官方参考示例：  http://lvgl.100ask.net/8.2/widgets/core/dropdown.html#drop-down-in-four-directions

//     lv_dropdown_set_dir(dd, LV_DIR_LEFT);     // 左侧
//     lv_dropdown_set_dir(dd, LV_DIR_RIGHT);    // 右侧
//     lv_dropdown_set_dir(dd, LV_DIR_TOP);      // 顶部
//     lv_dropdown_set_dir(dd, LV_DIR_BOTTOM);   // 底部
// #endif // 0

//     // 添加事件
//     // 当我们点击下拉列表时，会触发 LV_EVENT_CLICKED 事件类型，并且会创建出一个列表，列表中展示我们在前面设置的选项供我们选择
//     // 当我们选中一个选项时，会触发 LV_EVENT_VALUE_CHANGED 事件类型
//     lv_obj_add_event_cb(dd, demo7_dd_event_handler, LV_EVENT_ALL, NULL);

// #if 1
//     // 通过其他部件打开或关闭下拉列表
//     lv_obj_t *btn = lv_btn_create(lv_scr_act());
//     lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_align_to(btn, dd, LV_ALIGN_OUT_TOP_MID, 0, -10);
//     lv_obj_add_event_cb(btn, demo7_btn_event_handler, LV_EVENT_VALUE_CHANGED, dd);
// #endif // 1
// }

// static lv_obj_t *wifi_state_label;
// static wifi_state_t wifi_switch_state = WIFI_STATE_CLOSED;

// static void demo8_event_handler(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *sw = lv_event_get_target(e);
//     lv_obj_t *dd = (lv_obj_t *)lv_event_get_user_data(e);
//     if (code == LV_EVENT_VALUE_CHANGED)
//     {
//         if (wifi_switch_state == WIFI_STATE_CLOSED)
//         {
//             lv_label_set_text(wifi_state_label, "OPEN");
//             wifi_begin();
//             wifi_switch_state = WIFI_STATE_OPENED;
//         }
//         else if (wifi_switch_state == WIFI_STATE_OPENED)
//         {
//             lv_label_set_text(wifi_state_label, "CLOSED");
//             wifi_stop();
//             wifi_switch_state = WIFI_STATE_CLOSED;
//         }
//     }
// }

// void demo8()
// {

//     /* 创建基础部件(对象) */
//     lv_obj_t *obj = lv_obj_create(lv_scr_act());
//     lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));

//     lv_obj_t *wifi_sw = lv_switch_create(lv_scr_act());
//     lv_obj_set_size(wifi_sw, 200, 100);
//     lv_obj_align(wifi_sw, LV_ALIGN_CENTER, 0, 0);

//     /* 创建label部件(对象) */

//     wifi_state_label = lv_label_create(lv_scr_act());
//     lv_label_set_text(wifi_state_label, "CLOSED");                           // 设置label展示的文字
//     lv_obj_align_to(wifi_state_label, wifi_sw, LV_ALIGN_BOTTOM_RIGHT, 0, 0); // 将对象与其父对象的中心对齐,这里的父对象是屏幕:lv_scr_act()

//     lv_obj_add_event_cb(wifi_sw, demo8_event_handler, LV_EVENT_VALUE_CHANGED, wifi_state_label);
// }

// /* 心知天气 - https://www.seniverse.com */
// const char *SENIVERSE_KEY = "SYQAZcQPIoDp3vOPG"; // 心知天气秘钥,替换成自己的秘钥
// const char *SENIVERSE_LOCATION = "Qingdao";      // 城市列表 https://docs.seniverse.com/api/start/start.html,替换成自己的城市名称

// // void demo9_my_timer(lv_timer_t *timer)
// // {
// //     weather_info_t *user_data = (weather_info_t *)(timer->user_data);
// //     updateWeather(user_data, SENIVERSE_KEY, SENIVERSE_LOCATION);
// //     // usleep(125000); // 定时器的空闲百分比接近 75%
// // }

// // void demo9()
// // {

// //     weather_info_t weatherInfo;
// //     // lv_timer_t *timer = lv_timer_create(demo9_my_timer, 5000, &weatherInfo);
// //     updateWeather(&weatherInfo, SENIVERSE_KEY, SENIVERSE_LOCATION);
// //     lv_obj_t *obj = lv_obj_create(lv_scr_act());
// //     lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));

// //     lv_obj_t *dis = lv_label_create(obj);
// //     lv_obj_t *city = lv_label_create(obj);
// //     lv_obj_t *temperature = lv_label_create(obj);
// //     lv_obj_t *updateDate = lv_label_create(obj);

// //     lv_label_set_text(dis, "WEATHER:");                 // 设置label展示的文字
// //     lv_label_set_text_fmt(city, weatherInfo.city);                 // 设置label展示的文字
// //     lv_label_set_text_fmt(temperature, weatherInfo.temperature); // 设置label展示的文字
// //     lv_label_set_text_fmt(updateDate, weatherInfo.updateDate);     // 设置label展示的文字

// //     lv_obj_align(dis,LV_ALIGN_TOP_MID,0,0);
// //     lv_obj_align(temperature,LV_ALIGN_TOP_RIGHT,0,0);
// //     lv_obj_align(city,LV_ALIGN_CENTER,0,0);
// //     lv_obj_align(updateDate,LV_ALIGN_BOTTOM_MID,0,0);
// // }



// //PCF8563 test


// // RTC BM8563 I2C port
// // I2C pin definition for M5Stick & M5Stick Plus & M5Stack Core2
// #define BM8563_I2C_SDA 47
// #define BM8563_I2C_SCL 48

// typedef struct _lv_clock
// {
//     lv_obj_t *time_label;    // 时间标签
//     lv_obj_t *date_label;    // 日期标签
//     lv_obj_t *weekday_label; // 星期标签
// } lv_clock_t;

// I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire1);

// const char *ntpServer = "ntp.aliyun.com";

// void demo10_my_timer(lv_timer_t *timer)
// {
//     I2C_BM8563_TimeTypeDef timeStruct;
//     I2C_BM8563_DateTypeDef dateStruct;

//     rtc.getTime(&timeStruct);
//     rtc.getDate(&dateStruct);

//     static const char *week_day[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//     int year = dateStruct.year;
//     int month = dateStruct.month;
//     int day = dateStruct.date;
//     int weekday = dateStruct.weekDay;
//     int hour = timeStruct.hours;
//     int minutes = timeStruct.minutes;
//     int second = timeStruct.seconds;

//     if (timer != NULL && timer->user_data != NULL)
//     {
//         lv_clock_t *clock = (lv_clock_t *)(timer->user_data);
//         if (clock->time_label != NULL)
//         {
//             lv_label_set_text_fmt(clock->time_label, "%02d:%02d:%02d", hour, minutes, second);
//             lv_obj_align_to(clock->time_label, lv_obj_get_parent(clock->time_label), LV_ALIGN_CENTER, 0, 0);
//         }

//         if (clock->date_label != NULL)
//         {
//             lv_label_set_text_fmt(clock->date_label, "%d-%02d-%02d", year, month, day);
//             lv_obj_align_to(clock->date_label, lv_obj_get_parent(clock->date_label), LV_ALIGN_TOP_MID, 2, 0);
//         }

//         if (clock->weekday_label != NULL)
//         {
//             lv_label_set_text_fmt(clock->weekday_label, "%s", week_day[weekday]);
//             lv_obj_align_to(clock->weekday_label, lv_obj_get_parent(clock->weekday_label), LV_ALIGN_BOTTOM_MID, -2, 0);
//         }
//     }
// }

// void demo10()
// {
//     WiFi.begin("vivoS12", "7bsgbsg7"); // Or, Connect to the specified access point

//     Serial.print("Connecting to Wi-Fi ");
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println(" CONNECTED");

//     I2C_BM8563_DateTypeDef dateStruct;
//     I2C_BM8563_TimeTypeDef timeStruct;

//     Wire1.begin(BM8563_I2C_SDA, BM8563_I2C_SCL);

//     // Init RTC
//     rtc.begin();

//     configTime(8 * 3600, 0, ntpServer);

//     // Init I2C

//     // Get local time
//     struct tm timeInfo;
//     if (getLocalTime(&timeInfo))
//     {
//         // Set RTC time
//         I2C_BM8563_TimeTypeDef timeStruct;
//         timeStruct.hours = timeInfo.tm_hour;
//         timeStruct.minutes = timeInfo.tm_min;
//         timeStruct.seconds = timeInfo.tm_sec;
//         rtc.setTime(&timeStruct);

//         // Set RTC Date
//         I2C_BM8563_DateTypeDef dateStruct;
//         dateStruct.weekDay = timeInfo.tm_wday;
//         dateStruct.month = timeInfo.tm_mon + 1;
//         dateStruct.date = timeInfo.tm_mday;
//         dateStruct.year = timeInfo.tm_year + 1900;
//         rtc.setDate(&dateStruct);
//     }

//     static lv_style_t date_time_clock_style;                         // 最外层对象的样式
//     lv_style_reset(&date_time_clock_style);                          // 重置样式
//     lv_style_init(&date_time_clock_style);                           // 初始化样式
//     lv_style_set_radius(&date_time_clock_style, 5);                  // 设置样式圆角
//     lv_style_set_bg_opa(&date_time_clock_style, LV_OPA_20);          // 设置样式背景透明度
//     lv_style_set_border_width(&date_time_clock_style, 0);            // 设置样式边框宽度
//     lv_style_set_bg_color(&date_time_clock_style, lv_color_white()); // 设置样式背景颜色，白色
//     lv_style_set_pad_left(&date_time_clock_style, 1);                // 设置样式左边padding填充宽度
//     lv_style_set_pad_right(&date_time_clock_style, 1);               // 设置样式右边padding填充宽度
//     lv_style_set_pad_top(&date_time_clock_style, 0);                 // 设置样式顶部padding填充宽度
//     lv_style_set_pad_bottom(&date_time_clock_style, 0);              // 设置样式底部padding填充宽度

//     static lv_style_t time_style; // 时间对象样式
//     lv_style_reset(&time_style);
//     lv_style_init(&time_style);
//     lv_style_set_bg_opa(&time_style, LV_OPA_COVER);
//     lv_style_set_border_width(&time_style, 0);
//     lv_style_set_radius(&time_style, 5);
//     lv_style_set_bg_color(&time_style, lv_palette_main(LV_PALETTE_BLUE));
//     lv_style_set_pad_left(&time_style, 0);
//     lv_style_set_pad_right(&time_style, 0);
//     lv_style_set_pad_top(&time_style, 0);
//     lv_style_set_pad_bottom(&time_style, 0);

//     static lv_style_t date_style; // 日期对象样式
//     lv_style_reset(&date_style);
//     lv_style_init(&date_style);
//     lv_style_set_bg_opa(&date_style, LV_OPA_COVER);
//     lv_style_set_border_width(&date_style, 0);
//     lv_style_set_radius(&date_style, 5);
//     lv_style_set_bg_color(&date_style, lv_palette_main(LV_PALETTE_BLUE));
//     lv_style_set_pad_left(&date_style, 0);
//     lv_style_set_pad_right(&date_style, 0);

//     /* Time font */
//     static lv_style_t time_label_style;                                // 时间标签样式
//     lv_style_reset(&time_label_style);                                 // 重置样式
//     lv_style_init(&time_label_style);                                  // 初始化样式
//     lv_style_set_text_color(&time_label_style, lv_color_white());      // 设置标签样式文本颜色
//     lv_style_set_text_font(&time_label_style, &lv_font_montserrat_32); // 设置字体风格
//     lv_style_set_text_opa(&time_label_style, LV_OPA_COVER);            // 设置字体透明度
//     lv_style_set_bg_opa(&time_label_style, LV_OPA_0);                  // 设置样式背景透明度

//     /* Date font */
//     static lv_style_t date_label_style; // 日期标签样式
//     lv_style_reset(&date_label_style);
//     lv_style_init(&date_label_style);
//     lv_style_set_text_opa(&date_label_style, LV_OPA_COVER);
//     lv_style_set_bg_opa(&date_label_style, LV_OPA_0);
//     lv_style_set_text_color(&date_label_style, lv_color_white());
//     lv_style_set_text_font(&date_label_style, &lv_font_montserrat_16);

//     /* Week font */
//     static lv_style_t week_lable_style; // 日期标签样式
//     lv_style_reset(&week_lable_style);
//     lv_style_init(&week_lable_style);
//     lv_style_set_text_opa(&week_lable_style, LV_OPA_COVER);
//     lv_style_set_bg_opa(&week_lable_style, LV_OPA_0);
//     lv_style_set_text_color(&week_lable_style, lv_color_white());
//     lv_style_set_text_font(&week_lable_style, &lv_font_montserrat_16);

//     /* Time & Date */
//     lv_obj_t *time_date_obj = lv_obj_create(lv_scr_act()); // 基于屏幕创建时间日期对象
//     if (time_date_obj == NULL)
//     {
//         printf("[%s:%d] time_date_obj create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }

//     lv_obj_set_size(time_date_obj, 320, 100);                                  // 设置对象大小
//     lv_obj_center(time_date_obj);                                              // 对象居屏幕中间显示
//     lv_obj_add_style(time_date_obj, &date_time_clock_style, LV_STATE_DEFAULT); // 给time_date_obj对象添加样式

//     /*Time display*/
//     lv_obj_t *time_obj = lv_obj_create(time_date_obj); // 基于time_date_obj对象创建时间对象
//     if (time_obj == NULL)
//     {
//         printf("[%s:%d] time_obj create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }

//     lv_obj_set_size(time_obj, 158, 100);                               // 设置对象大小
//     lv_obj_align_to(time_obj, time_date_obj, LV_ALIGN_LEFT_MID, 0, 0); // 设置time_obj对象基于time_date_obj对象左边中间对齐
//     lv_obj_add_style(time_obj, &time_style, LV_STATE_DEFAULT);         // 给time_obj对象添加样式

//     static lv_clock_t lv_clock = {0};

//     lv_clock.time_label = lv_label_create(time_obj); // 基于time_obj对象创建时间显示标签对象 lv_clock.time_label
//     if (lv_clock.time_label == NULL)
//     {
//         printf("[%s:%d] time_label create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }

//     lv_obj_add_style(lv_clock.time_label, &time_label_style, LV_STATE_DEFAULT); // 给对象 lv_clock.time_label添加样式

//     /*Date display*/
//     lv_obj_t *date_obj = lv_obj_create(time_date_obj); // 基于time_date_obj对象创建date_obj对象
//     if (date_obj == NULL)
//     {
//         printf("[%s:%d] date_obj create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }
//     lv_obj_set_size(date_obj, 158, 100);                                // 设置对象大小
//     lv_obj_align_to(date_obj, time_date_obj, LV_ALIGN_RIGHT_MID, 0, 0); // 设置date_obj对象基于time_date_obj对象右边中部对齐
//     lv_obj_add_style(date_obj, &date_style, LV_STATE_DEFAULT);          // 给date_obj对象添加样式

//     lv_clock.date_label = lv_label_create(date_obj); // 基于date_obj对象创建lv_clock.date_label日期显示对象
//     if (lv_clock.date_label == NULL)
//     {
//         printf("[%s:%d] date_label create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }
//     lv_obj_add_style(lv_clock.date_label, &date_label_style, LV_STATE_DEFAULT); // 给lv_clock.date_label对象添加样式

//     /*Week display*/
//     lv_clock.weekday_label = lv_label_create(date_obj); // 基于date_obj对象创建星期显示lv_clock.weekday_label对象
//     if (lv_clock.weekday_label == NULL)
//     {
//         printf("[%s:%d] weekday_label create failed\n", __FUNCTION__, __LINE__);
//         return;
//     }
//     lv_obj_add_style(lv_clock.weekday_label, &week_lable_style, LV_STATE_DEFAULT); // 给对象lv_clock.weekday_label添加样式

//     // 设置时间标签lv_clock.time_label对象基于父对象居中对齐
//     lv_obj_align_to(lv_clock.time_label, lv_obj_get_parent(lv_clock.time_label), LV_ALIGN_CENTER, 0, 0);
//     // 设置时间标签lv_clock.date_label对象基于父对象顶部中间对齐
//     lv_obj_align_to(lv_clock.date_label, lv_obj_get_parent(lv_clock.date_label), LV_ALIGN_TOP_MID, 2, 0);
//     // 设置时间标签lv_clock.weekday_label对象基于父对象底部中间对齐
//     lv_obj_align_to(lv_clock.weekday_label, lv_obj_get_parent(lv_clock.weekday_label), LV_ALIGN_BOTTOM_MID, -2, 0);

//     lv_timer_t *task_timer = lv_timer_create(demo10_my_timer, 200, (void *)&lv_clock);
//     if (task_timer == NULL)
//     {
//         printf("[%s:%d] lv_timer_create failed\n", __FUNCTION__, __LINE__);
//     }
// }


// //DHT11 test

// #define DHTPIN 11
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);

// typedef struct _lv_dht11
// {
//     lv_obj_t *temperature_label; // 时间标签
//     lv_obj_t *humidity_label;    // 日期标签
// } lv_dht11_t;

// void demo11_my_timer(lv_timer_t *timer)
// {
//     float humidity = dht.readHumidity();
//     float temperature = dht.readTemperature();
//     Serial.println("humidity");
//     Serial.println(humidity);
//     Serial.println("temperature");
//     Serial.println(temperature);

//     if (timer != NULL && timer->user_data != NULL)
//     {
//         lv_dht11_t *dht11 = (lv_dht11_t *)(timer->user_data);
//         if (dht11->temperature_label != NULL)
//         {
//             lv_label_set_text_fmt(dht11->temperature_label, "%.2f %RH", humidity);
//             lv_obj_align_to(dht11->temperature_label, lv_obj_get_parent(dht11->temperature_label), LV_ALIGN_CENTER, 0, 0);
//         }

//         if (dht11->humidity_label != NULL)
//         {
//             lv_label_set_text_fmt(dht11->humidity_label, "%.2f °C", temperature);
//             lv_obj_align_to(dht11->humidity_label, lv_obj_get_parent(dht11->humidity_label), LV_ALIGN_TOP_MID, 2, 0);
//         }
//     }
// }

// void demo11()
// {
//     dht.begin();

//     static lv_style_t date_time_clock_style;                         // 最外层对象的样式
//     lv_style_reset(&date_time_clock_style);                          // 重置样式
//     lv_style_init(&date_time_clock_style);                           // 初始化样式
//     lv_style_set_radius(&date_time_clock_style, 5);                  // 设置样式圆角
//     lv_style_set_bg_opa(&date_time_clock_style, LV_OPA_20);          // 设置样式背景透明度
//     lv_style_set_border_width(&date_time_clock_style, 0);            // 设置样式边框宽度
//     lv_style_set_bg_color(&date_time_clock_style, lv_color_white()); // 设置样式背景颜色，白色
//     lv_style_set_pad_left(&date_time_clock_style, 1);                // 设置样式左边padding填充宽度
//     lv_style_set_pad_right(&date_time_clock_style, 1);               // 设置样式右边padding填充宽度
//     lv_style_set_pad_top(&date_time_clock_style, 0);                 // 设置样式顶部padding填充宽度
//     lv_style_set_pad_bottom(&date_time_clock_style, 0);              // 设置样式底部padding填充宽度

//     static lv_style_t time_style; // 时间对象样式
//     lv_style_reset(&time_style);
//     lv_style_init(&time_style);
//     lv_style_set_bg_opa(&time_style, LV_OPA_COVER);
//     lv_style_set_border_width(&time_style, 0);
//     lv_style_set_radius(&time_style, 5);
//     lv_style_set_bg_color(&time_style, lv_palette_main(LV_PALETTE_BLUE));
//     lv_style_set_pad_left(&time_style, 0);
//     lv_style_set_pad_right(&time_style, 0);
//     lv_style_set_pad_top(&time_style, 0);
//     lv_style_set_pad_bottom(&time_style, 0);

//     static lv_style_t time_label_style;                                // 时间标签样式
//     lv_style_reset(&time_label_style);                                 // 重置样式
//     lv_style_init(&time_label_style);                                  // 初始化样式
//     lv_style_set_text_color(&time_label_style, lv_color_white());      // 设置标签样式文本颜色
//     lv_style_set_text_font(&time_label_style, &lv_font_montserrat_32); // 设置字体风格
//     lv_style_set_text_opa(&time_label_style, LV_OPA_COVER);            // 设置字体透明度
//     lv_style_set_bg_opa(&time_label_style, LV_OPA_0);                  // 设置样式背景透明度

//     lv_obj_t *dht11_obj = lv_obj_create(lv_scr_act()); // 基于屏幕创建时间日期对象

//     lv_obj_set_size(dht11_obj, 320, 150);
//     lv_obj_center(dht11_obj);
//     lv_obj_add_style(dht11_obj, &time_style, LV_STATE_DEFAULT);

//     static lv_dht11_t lv_dht11 = {0};

//     lv_dht11.temperature_label = lv_label_create(dht11_obj);                           // 基于time_obj对象创建时间显示标签对象 lv_clock.time_label
//     lv_obj_add_style(lv_dht11.temperature_label, &time_label_style, LV_STATE_DEFAULT); // 给对象 lv_clock.time_label添加样式

//     lv_dht11.humidity_label = lv_label_create(dht11_obj);                            
//     lv_obj_add_style(lv_dht11.humidity_label, &time_label_style, LV_STATE_DEFAULT); // 给date_obj对象添加样式

//     lv_obj_align_to(lv_dht11.temperature_label, lv_obj_get_parent(lv_dht11.temperature_label), LV_ALIGN_TOP_MID, 0, 0);
//     lv_obj_align_to(lv_dht11.humidity_label, lv_obj_get_parent(lv_dht11.humidity_label), LV_ALIGN_BOTTOM_MID, 0, 0);

//     lv_timer_t *task_timer = lv_timer_create(demo11_my_timer, 2000, (void *)&lv_dht11);
// }


// //MQ2 test
// #define smokePin 5
// #define analogPin 12
// typedef struct _lv_mq2
// {
//     lv_obj_t *digitalNum; // 时间标签
//     lv_obj_t *analogNum;    // 日期标签
// } lv_mq2_t;

// void demo12_my_timer(lv_timer_t *timer)
// {
//     int mq2Value = digitalRead(smokePin);
//     int anaValue = analogRead(analogPin);
//     Serial.println("D0");
//     Serial.println(mq2Value);
//     Serial.println("ANA");
//     Serial.println(anaValue);

//     if (timer != NULL && timer->user_data != NULL)
//     {

//         lv_obj_t *mq2_label = (lv_obj_t *)(timer->user_data);
//         if (mq2_label != NULL)
//         {
//             lv_label_set_text_fmt(mq2_label, "digitalRead: %01d", mq2Value);
//             lv_obj_align_to(mq2_label, lv_obj_get_parent(mq2_label), LV_ALIGN_CENTER, 0, 0);
//         }
//     }
// }

// void demo12()
// {
//     static lv_style_t date_time_clock_style;                         // 最外层对象的样式
//     lv_style_reset(&date_time_clock_style);                          // 重置样式
//     lv_style_init(&date_time_clock_style);                           // 初始化样式
//     lv_style_set_radius(&date_time_clock_style, 5);                  // 设置样式圆角
//     lv_style_set_bg_opa(&date_time_clock_style, LV_OPA_20);          // 设置样式背景透明度
//     lv_style_set_border_width(&date_time_clock_style, 0);            // 设置样式边框宽度
//     lv_style_set_bg_color(&date_time_clock_style, lv_color_white()); // 设置样式背景颜色，白色
//     lv_style_set_pad_left(&date_time_clock_style, 1);                // 设置样式左边padding填充宽度
//     lv_style_set_pad_right(&date_time_clock_style, 1);               // 设置样式右边padding填充宽度
//     lv_style_set_pad_top(&date_time_clock_style, 0);                 // 设置样式顶部padding填充宽度
//     lv_style_set_pad_bottom(&date_time_clock_style, 0);              // 设置样式底部padding填充宽度

//     static lv_style_t time_style; // 时间对象样式
//     lv_style_reset(&time_style);
//     lv_style_init(&time_style);
//     lv_style_set_bg_opa(&time_style, LV_OPA_COVER);
//     lv_style_set_border_width(&time_style, 0);
//     lv_style_set_radius(&time_style, 5);
//     lv_style_set_bg_color(&time_style, lv_palette_main(LV_PALETTE_BLUE));
//     lv_style_set_pad_left(&time_style, 0);
//     lv_style_set_pad_right(&time_style, 0);
//     lv_style_set_pad_top(&time_style, 0);
//     lv_style_set_pad_bottom(&time_style, 0);

//     static lv_style_t time_label_style;                                // 时间标签样式
//     lv_style_reset(&time_label_style);                                 // 重置样式
//     lv_style_init(&time_label_style);                                  // 初始化样式
//     lv_style_set_text_color(&time_label_style, lv_color_white());      // 设置标签样式文本颜色
//     lv_style_set_text_font(&time_label_style, &lv_font_montserrat_32); // 设置字体风格
//     lv_style_set_text_opa(&time_label_style, LV_OPA_COVER);            // 设置字体透明度
//     lv_style_set_bg_opa(&time_label_style, LV_OPA_0);                  // 设置样式背景透明度

//     lv_obj_t *src = lv_obj_create(lv_scr_act()); // 基于屏幕创建时间日期对象
//     lv_obj_set_size(src, 320, 150);
//     lv_obj_center(src);
//     lv_obj_add_style(src, &time_style, LV_STATE_DEFAULT);

//     static lv_obj_t *lv_mq2_label = lv_label_create(src);                         
//     lv_obj_add_style(lv_mq2_label, &time_label_style, LV_STATE_DEFAULT); // 给date_obj对象添加样式

//     lv_obj_align_to(lv_mq2_label, lv_obj_get_parent(lv_mq2_label), LV_ALIGN_CENTER, 0, 0);

//     lv_timer_t *task_timer = lv_timer_create(demo12_my_timer, 2000, lv_mq2_label);
// }

// void my_lvgl()
// {
//     // demo1();
//     // demo2();
//     // demo3();
//     // demo4();
//     // demo5();
//     // demo6();
//     // demo7();
//     // demo8();
//     // demo9();
//     // demo10();
//     // demo11();
//     demo12();
// }