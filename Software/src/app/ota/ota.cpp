#include "app/ota/ota.h"

const char *MQTT_SERVER = "bemfa.com";
const int MQTT_PORT = 9501;
const char *MQTT_ID = "7ac43bc501d1d99d4f09fe6e6c5ed0f9";
const char *TOPIC = "esp32s3";
WiFiClient espClient;
PubSubClient client(espClient);
lv_timer_t *mqtt_timer;
static lv_obj_t *g_kb_label_1;
lv_obj_t *label_1_label_2;
static void kb_label_1_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_target(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

int fun_detect(char payload)
{
    if (payload == 'T') // time
        return 1;
    else if (payload == 'W') // weather
        return 2;
    else if (payload == 'A') // alarm
        return 3;
    else if (payload == 'P') // am/pm
        return 4;
    else if (payload == 'M') // alarm
        return 5;
}

void music_change(String temp)
{
    int a = temp.toInt();
    music_choose_flag = a;
    Serial.println("MUSIC");
    Serial.println(a);
}

void time_phase(String temp)
{
    struct tm timeChange;

    int16_t dataStart = 0;
    int16_t dataEnd = 0;
    String dataStr;

    char yearTemp[] = "year:";
    dataStart = temp.indexOf(yearTemp) + strlen(yearTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_year = dataStr.toInt();

    char monthTemp[] = "month:";
    dataStart = temp.indexOf(monthTemp) + strlen(monthTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_mon = dataStr.toInt();

    char dayTemp[] = "day:";
    dataStart = temp.indexOf(dayTemp) + strlen(dayTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_mday = dataStr.toInt();

    char hourTemp[] = "hour:";
    dataStart = temp.indexOf(hourTemp) + strlen(hourTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_hour = dataStr.toInt();

    char minTemp[] = "min:";
    dataStart = temp.indexOf(minTemp) + strlen(minTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_min = dataStr.toInt();

    char secTemp[] = "sec:";
    dataStart = temp.indexOf(secTemp) + strlen(secTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    timeChange.tm_sec = dataStr.toInt();

    Serial.println("TIME");
    Serial.println(timeChange.tm_year);
    Serial.println(timeChange.tm_mon);
    Serial.println(timeChange.tm_mday);
    Serial.println(timeChange.tm_hour);
    Serial.println(timeChange.tm_min);
    Serial.println(timeChange.tm_sec);

    // Set RTC time
    I2C_BM8563_TimeTypeDef timeStruct;
    timeStruct.hours = timeChange.tm_hour;
    timeStruct.minutes = timeChange.tm_min;
    timeStruct.seconds = timeChange.tm_sec;
    rtc.setTime(&timeStruct);

    // Set RTC Date
    I2C_BM8563_DateTypeDef dateStruct;
    dateStruct.weekDay = timeChange.tm_wday;
    dateStruct.month = timeChange.tm_mon;
    dateStruct.date = timeChange.tm_mday;
    dateStruct.year = timeChange.tm_year + 1900;
    rtc.setDate(&dateStruct);
}

void alarm_phase(String temp)
{
    // Ahour:20;min:15;sec:00;
    int16_t dataStart = 0;
    int16_t dataEnd = 0;
    String dataStr;

    char hourTemp[] = "hour:";
    dataStart = temp.indexOf(hourTemp) + strlen(hourTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    int8_t hour_temp = dataStr.toInt();

    char minTemp[] = "min:";
    dataStart = temp.indexOf(minTemp) + strlen(minTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    int8_t min_temp = dataStr.toInt();

    char secTemp[] = "sec:";
    dataStart = temp.indexOf(secTemp) + strlen(secTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    int8_t sec_temp = dataStr.toInt();

    Serial.println("ALARM");
    Serial.println(hour_temp);
    Serial.println(min_temp);
    Serial.println(sec_temp);

    alarmStruct = {hour_temp, min_temp, sec_temp};
}

void weather_phase(String temp)
{
    // time2023-06-30T12:06+08:00;tem30;weather晴;wind东北风;
    int16_t dataStart = 0;
    int16_t dataEnd = 0;
    String dataStr;

    char hourTemp[] = "tem";
    dataStart = temp.indexOf(hourTemp) + strlen(hourTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    tem_now = dataStr.toInt();

    char minTemp[] = ";weather";
    dataStart = temp.indexOf(minTemp) + strlen(minTemp);
    dataEnd = temp.indexOf(";", dataStart);
    dataStr = temp.substring(dataStart, dataEnd);
    weather_condition = dataStr;

    // char secTemp[] = ";wind";
    // dataStart = temp.indexOf(secTemp) + strlen(secTemp);
    // dataEnd = temp.indexOf(";", dataStart);
    // dataStr = temp.substring(dataStart, dataEnd);
    // int8_t sec_temp = dataStr.toInt();

}

void am_pm_phase(String temp)
{
    ampmFlag = !ampmFlag;
    Serial.println("AM/PM");
    Serial.println(ampmFlag);
}

void phase_mqtt(int fun_choose, String message)
{
    if (fun_choose == 1)
    {
        time_phase(message);
    }
    else if (fun_choose == 2)
    {
        weather_phase(message);
    }
    else if (fun_choose == 3)
    {
        alarm_phase(message);
    }
    else if (fun_choose == 4)
    {
        am_pm_phase(message);
    }
    else if (fun_choose == 5)
    {
        music_change(message);
    }
}

void mqtt_reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(MQTT_ID))
        {
            Serial.println("connected");
            // 连接成功时订阅主题
            client.subscribe(TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic); // 打印主题信息
    Serial.print("] ");
    String message;
    for (int i = 1; i < length; i++)
    {
        message += (char)payload[i]; // 将每个字节转换为字符并添加到字符串中
    }
    
    //注意：此处如果用fm格式化，传递稍微长一点的参数就会出现乱码，只能用lv_label_set_text，.c_str()

    lv_label_set_text(label_1_label_2, message.c_str());
    delay(10);
    int temp = fun_detect((char)payload[0]);
    Serial.println((char)payload[0]); // 打印接收到的字符串
    phase_mqtt(temp, message);
    Serial.println(message); // 打印接收到的字符串
}

lv_obj_t *ota_load()
{

    // Wire1.begin(BM8563_I2C_SDA, BM8563_I2C_SCL);
    // rtc.begin();

    client.setServer(MQTT_SERVER, MQTT_PORT); // 设定MQTT服务器与使用的端口，1883是默认的MQTT端口
    client.setCallback(callback);             // 设定回调方式，当ESP8266收到订阅消息时会调用此方法

    now_screen = lv_obj_create(NULL);

    lv_obj_set_scrollbar_mode(now_screen, LV_SCROLLBAR_MODE_OFF);

    // Set style for now_screen. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(now_screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(now_screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(now_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes label_1_label_1
    lv_obj_t *label_1_label_1 = lv_label_create(now_screen);
    lv_obj_set_pos(label_1_label_1, 55, 89);
    lv_obj_set_size(label_1_label_1, 371, 142);
    lv_obj_set_scrollbar_mode(label_1_label_1, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(label_1_label_1, "请使用APP操控或\n远程更新固件");
    lv_label_set_long_mode(label_1_label_1, LV_LABEL_LONG_WRAP);

    // Set style for label_1_label_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(label_1_label_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(label_1_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(label_1_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(label_1_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(label_1_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_1_label_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label_1_label_1, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label_1_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(label_1_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(label_1_label_1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(label_1_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes label_1_spinner_1
    lv_obj_t *label_1_spinner_1 = lv_spinner_create(now_screen, 10000, 20);
    lv_obj_set_pos(label_1_spinner_1, 190, 170);
    lv_obj_set_size(label_1_spinner_1, 100, 100);

    // Set style for label_1_spinner_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(label_1_spinner_1, lv_color_make(0xee, 0xee, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(label_1_spinner_1, lv_color_make(0xee, 0xee, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(label_1_spinner_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label_1_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(label_1_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(label_1_spinner_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(label_1_spinner_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(label_1_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(label_1_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(label_1_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(label_1_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(label_1_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(label_1_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(label_1_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(label_1_spinner_1, lv_color_make(0xd5, 0xd6, 0xde), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(label_1_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for label_1_spinner_1. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(label_1_spinner_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(label_1_spinner_1, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write codes label_1_btn_1
    lv_obj_t *label_1_btn_1 = lv_btn_create(now_screen);
    lv_obj_set_pos(label_1_btn_1, 363, 18);
    lv_obj_set_size(label_1_btn_1, 100, 50);
    lv_obj_set_scrollbar_mode(label_1_btn_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for label_1_btn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(label_1_btn_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(label_1_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(label_1_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(label_1_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label_1_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(label_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(label_1_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(label_1_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(label_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(label_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(label_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(label_1_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(label_1_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(label_1_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_1_btn_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label_1_btn_1, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(label_1_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *label_1_btn_1_label = lv_label_create(label_1_btn_1);
    lv_label_set_text(label_1_btn_1_label, "返回");
    lv_obj_set_style_pad_all(label_1_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_align(label_1_btn_1_label, LV_ALIGN_CENTER, 0, 0);

    // Write codes label_1_label_2
    label_1_label_2 = lv_label_create(now_screen);
    lv_obj_set_pos(label_1_label_2, 21, 18);
    lv_obj_set_size(label_1_label_2, 333, 52);
	lv_obj_set_scrollbar_mode(label_1_label_2, LV_SCROLLBAR_MODE_ON);
	lv_label_set_text(label_1_label_2, "Message");
	lv_label_set_long_mode(label_1_label_2, LV_LABEL_LONG_SCROLL);

    // Set style for label_1_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(label_1_label_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(label_1_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(label_1_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label_1_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(label_1_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(label_1_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(label_1_label_2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_1_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label_1_label_2, &lv_chinese_font_16_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label_1_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(label_1_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(label_1_label_2, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(label_1_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(label_1_btn_1, mqtt_gesture, LV_EVENT_CLICKED, NULL);

    mqtt_timer = lv_timer_create(mqtt_timer_cb, 500, NULL);

    // gesture(0, 1, 1, 0);

    return now_screen;
}

void mqtt_gesture(lv_event_t *e)
{
    lv_timer_del(mqtt_timer);
    client.disconnect();
    Serial.println("MQTT通信已停止");
    app_return(LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 20);
}

void mqtt_timer_cb(lv_timer_t *timer)
{
    if (!client.connected())
    {
        mqtt_reconnect();
    }
    client.loop();
}