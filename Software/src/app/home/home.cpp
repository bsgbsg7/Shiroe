#include "home.h"
#include "app/lunar/lunar.h"
#include "app/music/music.h"
#include "app/ota/ota.h"
#include "app/fire/fire.h"
double ampmFlag = 0;         // 0--24；1--AM/PM
double httpFlag = 201;       // http互斥锁
double beginFlag = 0;        // 初始化只执行一次
double alarm_timer_flag = 0; // 闹钟警报标志位
int music_choose_flag = 2;   // 默认播放第一首歌
lv_timer_t *clock_timer;
lv_timer_t *computer_timer;
lv_timer_t *weather_timer;
lv_timer_t *alarm_timer;
lv_timer_t *dht11_timer;
lv_timer_t *mq2_timer;
lv_obj_t *screen_sw_1;
lv_obj_t *dlist_1;
static lv_clock_t lv_clock;
static lv_computer_t lv_computer;
static lv_weather_t lv_weather;
static lv_dht11_t lv_dht11;
// PCF8563
I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire1);
const char *ntpServer = "ntp.aliyun.com";
I2C_BM8563_TimeTypeDef timeStruct = {0}; // 时间存储
I2C_BM8563_DateTypeDef dateStruct = {0};
I2C_BM8563_TimeTypeDef alarmStruct = {18, 48, 0}; // 闹钟存储
// My_Weather_t weatherStruct;// 天气存储
// computer
WiFiClient computer_client;
WiFiClient weather_client;
WiFiClient advice_client;
// 注意两个client不能同时运行，需要使用互斥锁
// const char *ssid = "vivoS12";
// const char *password = "7bsgbsg7";
const char *host = "192.168.118.137";
int cpu_usage = 0;
int gpu_tem = 0;
int gpu_usage = 0;
int cpu_tem = 0;

int year = 0;
int month = 0;
int day = 0;
int weekday = 0;
int hour = 0;
int minutes = 0;
int second = 0;

// weather
const char *results_0_daily_0_low = "25";
const char *results_0_daily_0_high = "35";
int weather_timer_flag = 0;
int tem_now = 34;
String position = "青岛";
String weather_condition = "晴";
String air = "空气优";
String update_time = "20:13:38";

// dht11
DHT dht(DHTPIN, DHTTYPE);

lv_obj_t *time_text;
lv_obj_t *alarm_text;
bool time_changed = 0;
bool alarm_changed = 0;

void save_event(lv_event_t *e)
{
    Serial.println("enter SAVE EVENT");
    if (time_changed)
    {
        String text_temp = lv_textarea_get_text(time_text);
        time_phase(text_temp);
    }
    if (alarm_changed)
    {
        Serial.println("ALARM CHANGED");
        String alarm_temp = lv_textarea_get_text(alarm_text);
        Serial.println(alarm_temp);
        alarm_phase(alarm_temp);
        Serial.println("ALARM DONE");
    }
    if (lv_obj_has_state(screen_sw_1, LV_STATE_CHECKED))
    {
        ampmFlag = 1;
    }
    else if (!lv_obj_has_state(screen_sw_1, LV_STATE_CHECKED))
    {
        ampmFlag = 0;
    }

    music_choose_flag = lv_dropdown_get_selected(dlist_1);
    Serial.println(music_choose_flag);
    app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 50);
}

void pcf8563_timer(lv_timer_t *timer)
{

    rtc.getTime(&timeStruct);
    rtc.getDate(&dateStruct);

    static const char *week_day[7] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};

    year = dateStruct.year;
    month = dateStruct.month;
    day = dateStruct.date;
    weekday = dateStruct.weekDay;
    hour = timeStruct.hours;
    minutes = timeStruct.minutes;
    second = timeStruct.seconds;

    // Serial.println(timer==NULL);
    // Serial.println(timer->user_data == NULL);

    if (timer != NULL && timer->user_data != NULL)
    {
        lv_clock_t *clock = (lv_clock_t *)(timer->user_data);
        if (ampmFlag == 0)
        {
            if (clock->time_label != NULL)
            {
                lv_label_set_text_fmt(clock->time_label, "%02d:%02d:%02d", hour, minutes, second);
            }
        }
        else
        {
            if (hour > 12)
            {
                if (clock->time_label != NULL)
                {
                    lv_label_set_text_fmt(clock->time_label, "%02d:%02dPM", hour - 12, minutes);
                }
            }
            else
            {
                if (clock->time_label != NULL)
                {
                    lv_label_set_text_fmt(clock->time_label, "%02d:%02dAM", hour, minutes);
                }
            }
        }

        if (clock->date_label != NULL)
        {
            lv_label_set_text_fmt(clock->date_label, "%d-%02d-%02d", year, month, day);
        }

        if (clock->weekday_label != NULL)
        {
            lv_label_set_text_fmt(clock->weekday_label, "%s", week_day[weekday]);
        }
    }
}

void computer_monitor(lv_timer_t *timer)
{
    if (httpFlag <= 200)
    {
        String getUrl = "/sse";
        computer_client.print(String("GET ") + getUrl + " HTTP/1.1\r\n" + "Content-Type=application/json;charset=utf-8\r\n" + "Host: " + host + "\r\n" + "User-Agent=ESP32\r\n" + "Connection: close\r\n\r\n");

        delay(10);

        char endOfHeaders[] = "\n\n";
        bool ok = computer_client.find(endOfHeaders);
        if (!ok)
        {
            Serial.println("No response or invalid response!");
        }

        String line = "";

        line += computer_client.readStringUntil('\n');
        Serial.println(line);

        int16_t dataStart = 0;
        int16_t dataEnd = 0;
        String dataStr;

        char cpuUsage[] = "CPU_usage";
        dataStart = line.indexOf(cpuUsage) + strlen(cpuUsage);
        dataEnd = line.indexOf("%", dataStart);
        dataStr = line.substring(dataStart, dataEnd);
        cpu_usage = dataStr.toInt();

        char gpuUsage[] = "GPU1_usage";
        dataStart = line.indexOf(gpuUsage) + strlen(gpuUsage);
        dataEnd = line.indexOf("%", dataStart);
        dataStr = line.substring(dataStart, dataEnd);
        gpu_usage = dataStr.toInt();

        char gpuTem[] = "GPU_temperature";
        dataStart = line.indexOf(gpuTem) + strlen(gpuTem);
        dataEnd = line.indexOf("C", dataStart);
        dataStr = line.substring(dataStart, dataEnd);
        gpu_tem = dataStr.toInt();

        char cpuTem[] = "CPU_temperature";
        dataStart = line.indexOf(cpuTem) + strlen(cpuTem);
        dataEnd = line.indexOf("C", dataStart);
        dataStr = line.substring(dataStart, dataEnd);
        cpu_tem = dataStr.toInt();

        // Serial.print("cpu_usage :");
        // Serial.println(cpu_usage);
        // Serial.print("gpu_usage :");
        // Serial.println(gpu_usage);
        // Serial.print("gpu_tem :");
        // Serial.println(gpu_tem);
        // Serial.print("cpu_tem :");
        // Serial.println(cpu_tem);

        if (timer != NULL && timer->user_data != NULL)
        {
            lv_computer_t *computer = (lv_computer_t *)(timer->user_data);
            if (computer->cpu_temperature_label != NULL)
            {
                lv_label_set_text_fmt(computer->cpu_temperature_label, "%02d℃", cpu_tem);
            }

            if (computer->gpu_temperature_label != NULL)
            {
                lv_label_set_text_fmt(computer->gpu_temperature_label, "%02d℃", gpu_tem);
            }

            if (computer->gpu_occupation_label != NULL)
            {
                lv_label_set_text_fmt(computer->gpu_occupation_label, "%02d%", gpu_usage);
            }

            if (computer->cpu_occupation_label != NULL)
            {
                lv_label_set_text_fmt(computer->cpu_occupation_label, "%02d%", cpu_usage);
            }

            if (computer->cpu_label1 != NULL)
            {
                lv_arc_set_angles(computer->cpu_label1, 90, (cpu_tem * 360.0 / 100) + 90);
            }

            if (computer->cpu_label2 != NULL)
            {
                lv_arc_set_angles(computer->cpu_label2, 90, (gpu_tem * 360.0 / 100) + 90);
            }

            if (computer->gpu_label1 != NULL)
            {
                lv_arc_set_angles(computer->gpu_label1, 90, (cpu_usage * 360.0 / 100) + 90);
            }

            if (computer->gpu_label2 != NULL)
            {
                lv_arc_set_angles(computer->gpu_label2, 90, (gpu_usage * 360.0 / 100) + 90);
            }
        }
        httpFlag += 1;
    }
}

void weather_timer_cb(lv_timer_t *timer)
{
    weather_init();

    if (timer != NULL && timer->user_data != NULL)
    {
        lv_weather_t *weather = (lv_weather_t *)(timer->user_data);
        if (weather->tem_high_low != NULL)
        {
            lv_label_set_text_fmt(weather->tem_high_low, "%s/%s℃", results_0_daily_0_high, results_0_daily_0_low);
        }
        if (weather->position != NULL)
        {
            lv_label_set_text_fmt(weather->position, "%s", position);
        }
        if (weather->tem_now != NULL)
        {
            lv_label_set_text_fmt(weather->tem_now, "%02d℃", tem_now);
        }
        if (weather->weather_air != NULL)
        {
            // lv_label_set_text_fmt(weather->weather_air, "%s|空气%s", weather_condition, air);
            lv_label_set_text_fmt(weather->weather_air, "%s|空气优", weather_condition);
        }
        if (weather->update_time != NULL)
        {
            lv_label_set_text_fmt(weather->update_time, "更新时间：%s", update_time);
        }
    }
}

String extractTimeFromDateTime(String dateTimeString)
{
    // 找到 'T' 的位置
    int tIndex = dateTimeString.indexOf('T');

    // 从 'T' 后面截取时间部分
    String timeString = dateTimeString.substring(tIndex + 1);

    // 找到时区偏移部分的起始位置
    int offsetIndex = timeString.indexOf('+');

    if (offsetIndex != -1)
    {
        // 去除时区偏移部分
        timeString = timeString.substring(0, offsetIndex);
    }

    // 返回时间部分
    return timeString;
}

void dht11_get(lv_timer_t *timer)
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    // Serial.println("humidity");
    // Serial.println(humidity);
    // Serial.println("temperature");
    // Serial.println(temperature);

    if (timer != NULL && timer->user_data != NULL)
    {
        lv_dht11_t *dht11 = (lv_dht11_t *)(timer->user_data);
        if (dht11->temperature != NULL)
        {
            lv_label_set_text_fmt(dht11->temperature, "湿度:%.1f", humidity);
        }

        if (dht11->humidity != NULL)
        {
            lv_label_set_text_fmt(dht11->humidity, "温度:%.1f", temperature);
        }

        if (dht11->humidity_arc != NULL)
        {
            lv_arc_set_angles(dht11->humidity_arc, 90, (humidity * 360.0 / 100) + 90);
        }

        if (dht11->temperature_arc != NULL)
        {
            lv_arc_set_angles(dht11->temperature_arc, 90, (temperature * 360.0 / 100) + 90);
        }
    }
}

void alarm_timer_cb(lv_timer_t *timer)
{
    if (timeStruct.hours == alarmStruct.hours &&
        timeStruct.minutes == alarmStruct.minutes)
    {
        if (alarm_timer_flag == 0)
        {
            lv_timer_del(clock_timer);
            // lv_timer_del(computer_timer);
            lv_timer_del(weather_timer);
            lv_timer_del(alarm_timer);
            lv_timer_del(dht11_timer);
            lv_timer_del(mq2_timer);
            app_open(alarm_load, LV_SCR_LOAD_ANIM_FADE_IN, 200, 10);
            alarm_timer_flag = 1;
        }
    }
    else
    {
        alarm_timer_flag = 0;
    }
}

void mq2_timer_cb(lv_timer_t *timer)
{
    if (digitalRead(5) == 0)
    {
        lv_timer_del(clock_timer);
        // lv_timer_del(computer_timer);
        lv_timer_del(weather_timer);
        lv_timer_del(alarm_timer);
        lv_timer_del(dht11_timer);
        lv_timer_del(mq2_timer);
        app_open(fire_load, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 10);
    }
}

lv_obj_t *home_load()
{
    now_screen = lv_obj_create(NULL);
    if (beginFlag == 0)
    {
        pinMode(5, INPUT_PULLUP);
        Wire1.begin(BM8563_I2C_SDA, BM8563_I2C_SCL);
        rtc.begin();
        // dht11
        dht.begin();
        // Init RTC
        configTime(8 * 3600, 0, ntpServer);
        // // Get local time
        struct tm timeInfo;
        if (getLocalTime(&timeInfo))
        {
            // Set RTC time
            I2C_BM8563_TimeTypeDef timeStruct;
            timeStruct.hours = timeInfo.tm_hour;
            timeStruct.minutes = timeInfo.tm_min;
            timeStruct.seconds = timeInfo.tm_sec;
            rtc.setTime(&timeStruct);

            // Set RTC Date

            I2C_BM8563_DateTypeDef dateStruct;
            dateStruct.weekDay = timeInfo.tm_wday;
            dateStruct.month = timeInfo.tm_mon + 1;
            dateStruct.date = timeInfo.tm_mday;
            dateStruct.year = timeInfo.tm_year + 1900;
            rtc.setDate(&dateStruct);
        }
        weather_init();
        Serial.println("HERE");
    }

    // Create keyboard on screen
    // g_kb_screen = lv_keyboard_create(now_screen);
    // lv_obj_add_event_cb(g_kb_screen, kb_screen_event_cb, LV_EVENT_ALL, NULL);
    // lv_obj_add_flag(g_kb_screen, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_set_style_text_font(g_kb_screen, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_scrollbar_mode(now_screen, LV_SCROLLBAR_MODE_OFF);
    // Set style for screen. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(now_screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(now_screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(now_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes now_screen
    // lv_obj_t *now_screen = lv_obj_create(now_screen);
    // lv_obj_set_pos(now_screen, 0, 0);
    // lv_obj_set_size(now_screen, 480, 320);
    // lv_obj_set_scrollbar_mode(now_screen, LV_SCROLLBAR_MODE_OFF);

    // Set style for now_screen. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(now_screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(now_screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(now_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(now_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(now_screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(now_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(now_screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(now_screen, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(now_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_arc_1
    lv_obj_t *screen_arc_1 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_1, 191, 50);
    lv_obj_set_size(screen_arc_1, 50, 50);
    lv_obj_set_scrollbar_mode(screen_arc_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_1, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_1. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_1, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_1. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_1, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_1, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_1, 0, 100);
    lv_arc_set_bg_angles(screen_arc_1, 0, 360);
    lv_arc_set_angles(screen_arc_1, 90, 180);
    lv_arc_set_rotation(screen_arc_1, 0);
    lv_obj_set_style_arc_rounded(screen_arc_1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_1, 0, LV_STATE_DEFAULT);

    // Write codes screen_label_6
    lv_obj_t *screen_label_6 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_6, 13, 40);
    lv_obj_set_size(screen_label_6, 161, 36);
    lv_obj_set_scrollbar_mode(screen_label_6, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_6, "2023-06-03");
    lv_label_set_long_mode(screen_label_6, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_6, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_6, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_6, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_line_1
    lv_obj_t *screen_line_1 = lv_line_create(now_screen);
    lv_obj_set_pos(screen_line_1, 4, 5);
    lv_obj_set_size(screen_line_1, 177, 164);
    lv_obj_set_scrollbar_mode(screen_line_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_line_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_line_color(screen_line_1, lv_color_make(0x75, 0x75, 0x75), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(screen_line_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(screen_line_1, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_point_t temp_screen_line_1[] = {
        {0, 0},
        {172, 0},
        {172, 152},
        {0, 152},
        {0, 0},
    };
    lv_line_set_points(screen_line_1, temp_screen_line_1, 5);

    // Write codes screen_label_8
    lv_obj_t *screen_label_8 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_8, 44, 115);
    lv_obj_set_size(screen_label_8, 101, 35);
    lv_obj_set_scrollbar_mode(screen_label_8, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_8, "星期四");
    lv_label_set_long_mode(screen_label_8, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_8. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_8, lv_color_make(0x5d, 0x40, 0x82), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_8, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_8, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_8, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_line_3
    lv_obj_t *screen_line_3 = lv_line_create(now_screen);
    lv_obj_set_pos(screen_line_3, 181, 161);
    lv_obj_set_size(screen_line_3, 345, 178);
    lv_obj_set_scrollbar_mode(screen_line_3, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_line_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_line_color(screen_line_3, lv_color_make(0x75, 0x75, 0x75), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(screen_line_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(screen_line_3, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_point_t temp_screen_line_3[] = {
        {0, 0},
        {291, 0},
        {291, 151},
        {0, 151},
        {0, 0},
    };
    lv_line_set_points(screen_line_3, temp_screen_line_3, 5);

    // Write codes screen_line_2
    lv_obj_t *screen_line_2 = lv_line_create(now_screen);
    lv_obj_set_pos(screen_line_2, 4, 161);
    lv_obj_set_size(screen_line_2, 176, 157);
    lv_obj_set_scrollbar_mode(screen_line_2, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_line_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_line_color(screen_line_2, lv_color_make(0x75, 0x75, 0x75), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(screen_line_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(screen_line_2, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_point_t temp_screen_line_2[] = {
        {0, 0},
        {172, 0},
        {172, 151},
        {0, 151},
        {0, 0},
    };
    lv_line_set_points(screen_line_2, temp_screen_line_2, 5);

    // Write codes screen_line_4
    lv_obj_t *screen_line_4 = lv_line_create(now_screen);
    lv_obj_set_pos(screen_line_4, 181, 5);
    lv_obj_set_size(screen_line_4, 124, 155);
    lv_obj_set_scrollbar_mode(screen_line_4, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_line_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_line_color(screen_line_4, lv_color_make(0x75, 0x75, 0x75), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(screen_line_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(screen_line_4, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_point_t temp_screen_line_4[] = {
        {0, 0},
        {120, 0},
        {120, 152},
        {0, 152},
        {0, 0},
    };
    lv_line_set_points(screen_line_4, temp_screen_line_4, 5);

    // Write codes screen_label_16
    lv_obj_t *screen_label_16 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_16, 260, 71);
    lv_obj_set_size(screen_label_16, 27, 9);
    lv_obj_set_scrollbar_mode(screen_label_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_16, "湿度");
    lv_label_set_long_mode(screen_label_16, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_16. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_16, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_16, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_16, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_16, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_16, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_16, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_16, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_12
    lv_obj_t *screen_label_12 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_12, 64, 249);
    lv_obj_set_size(screen_label_12, 61, 15);
    lv_obj_set_scrollbar_mode(screen_label_12, LV_SCROLLBAR_MODE_OFF);
    // lv_label_set_text(screen_label_12, "32/21℃");
    lv_label_set_text_fmt(screen_label_12, "%s/%s℃", results_0_daily_0_high, results_0_daily_0_low);
    lv_label_set_long_mode(screen_label_12, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_12. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_12, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_12, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_12, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_12, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_12, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_12, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_12, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_3
    lv_obj_t *screen_label_3 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_3, 52, 15);
    lv_obj_set_size(screen_label_3, 80, 20);
    lv_obj_set_scrollbar_mode(screen_label_3, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_3, "系统时间");
    lv_label_set_long_mode(screen_label_3, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_3, lv_color_make(0xf6, 0x8c, 0x05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_3, 251, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_3, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_3, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_7
    lv_obj_t *screen_label_7 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_7, 18, 76);
    lv_obj_set_size(screen_label_7, 153, 33);
    lv_obj_set_scrollbar_mode(screen_label_7, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_7, "15:59:32");
    lv_label_set_long_mode(screen_label_7, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_7. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_7, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_7, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_7, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_7, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_7, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_line_5
    lv_obj_t *screen_line_5 = lv_line_create(now_screen);
    lv_obj_set_pos(screen_line_5, 306, 5);
    lv_obj_set_size(screen_line_5, 167, 153);
    lv_obj_set_scrollbar_mode(screen_line_5, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_line_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_line_color(screen_line_5, lv_color_make(0x75, 0x75, 0x75), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(screen_line_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(screen_line_5, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_point_t temp_screen_line_5[] = {
        {0, 0},
        {166, 0},
        {166, 152},
        {0, 152},
        {0, 0},
    };
    lv_line_set_points(screen_line_5, temp_screen_line_5, 5);

    // Write codes screen_label_2
    lv_obj_t *screen_label_2 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_2, 204, 15);
    lv_obj_set_size(screen_label_2, 85, 20);
    lv_obj_set_scrollbar_mode(screen_label_2, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_2, "环境检测");
    lv_label_set_long_mode(screen_label_2, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_2, lv_color_make(0x14, 0x45, 0x6d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_2, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_10
    lv_obj_t *screen_label_10 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_10, 62, 198);
    lv_obj_set_size(screen_label_10, 65, 16);
    lv_obj_set_scrollbar_mode(screen_label_10, LV_SCROLLBAR_MODE_OFF);
    // lv_label_set_text(screen_label_10, "即墨区");
    lv_label_set_text_fmt(screen_label_10, "%s", position);
    lv_label_set_long_mode(screen_label_10, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_10. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_10, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_10, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_10, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_10, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_4
    lv_obj_t *screen_label_4 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_4, 54, 171);
    lv_obj_set_size(screen_label_4, 80, 20);
    lv_obj_set_scrollbar_mode(screen_label_4, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_4, "实时天气");
    lv_label_set_long_mode(screen_label_4, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_4, lv_color_make(0xb4, 0x32, 0x45), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_4, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_4, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_15
    lv_obj_t *screen_label_15 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_15, 203, 70);
    lv_obj_set_size(screen_label_15, 27, 9);
    lv_obj_set_scrollbar_mode(screen_label_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_15, "温度");
    lv_label_set_long_mode(screen_label_15, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_15. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_15, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_15, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_15, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_15, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_15, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_15, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_15, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_5
    lv_obj_t *screen_label_5 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_5, 283, 171);
    lv_obj_set_size(screen_label_5, 80, 20);
    lv_obj_set_scrollbar_mode(screen_label_5, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_5, "状态监测器");
    lv_label_set_long_mode(screen_label_5, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_5, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_13
    lv_obj_t *screen_label_13 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_13, 47, 267);
    lv_obj_set_size(screen_label_13, 94, 15);
    lv_obj_set_scrollbar_mode(screen_label_13, LV_SCROLLBAR_MODE_OFF);
    // lv_label_set_text(screen_label_13, "多云|空气优");
    lv_label_set_text_fmt(screen_label_13, "%s|空气优", weather_condition); // 空气的api暂时不想写了
    lv_label_set_long_mode(screen_label_13, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_13. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_13, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_13, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_13, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_13, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_13, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_11
    lv_obj_t *screen_label_11 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_11, 41, 220);
    lv_obj_set_size(screen_label_11, 106, 21);
    lv_obj_set_scrollbar_mode(screen_label_11, LV_SCROLLBAR_MODE_OFF);
    // lv_label_set_text(screen_label_11, "28℃");
    lv_label_set_text_fmt(screen_label_11, "%02d℃", tem_now);
    lv_label_set_long_mode(screen_label_11, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_11. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_11, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_11, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_11, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_11, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_11, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_11, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_11, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_14
    lv_obj_t *screen_label_14 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_14, 15, 286);
    lv_obj_set_size(screen_label_14, 159, 17);
    lv_obj_set_scrollbar_mode(screen_label_14, LV_SCROLLBAR_MODE_OFF);
    // lv_label_set_text(screen_label_14, "更新时间：15:23:30");
    lv_label_set_text_fmt(screen_label_14, "更新时间：%s", update_time);
    lv_label_set_long_mode(screen_label_14, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_14. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_14, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_14, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_14, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_14, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_14, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_arc_2
    lv_obj_t *screen_arc_2 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_2, 248, 50);
    lv_obj_set_size(screen_arc_2, 50, 50);
    lv_obj_set_scrollbar_mode(screen_arc_2, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_2, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_2, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_2, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_2. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_2, lv_color_make(0x87, 0xb4, 0xda), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_2, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_2. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_2, lv_color_make(0x87, 0xb4, 0xda), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_2, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_2, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_2, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_2, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_2, 0, 100);
    lv_arc_set_bg_angles(screen_arc_2, 0, 360);
    lv_arc_set_angles(screen_arc_2, 90, 180);
    lv_arc_set_rotation(screen_arc_2, 0);
    lv_obj_set_style_arc_rounded(screen_arc_2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_2, 0, LV_STATE_DEFAULT);

    // Write codes screen_arc_6
    lv_obj_t *screen_arc_6 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_6, 404, 205);
    lv_obj_set_size(screen_arc_6, 55, 55);
    lv_obj_set_scrollbar_mode(screen_arc_6, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_6, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_6, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_6, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_6, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_6. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_6, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_6. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_6, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_6, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_6, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_6, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_6, 0, 100);
    lv_arc_set_bg_angles(screen_arc_6, 0, 360);
    lv_arc_set_angles(screen_arc_6, 90, 180);
    lv_arc_set_rotation(screen_arc_6, 0);
    lv_obj_set_style_arc_rounded(screen_arc_6, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_6, 0, LV_STATE_DEFAULT);

    // Write codes screen_label_26
    lv_obj_t *screen_label_26 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_26, 281, 270);
    lv_obj_set_size(screen_label_26, 33, 13);
    lv_obj_set_scrollbar_mode(screen_label_26, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_26, "30℃");
    lv_label_set_long_mode(screen_label_26, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_26. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_26, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_26, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_26, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_26, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_26, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_26, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_26, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_arc_5
    lv_obj_t *screen_arc_5 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_5, 336, 205);
    lv_obj_set_size(screen_arc_5, 55, 55);
    lv_obj_set_scrollbar_mode(screen_arc_5, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_5, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_5, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_5, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_5, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_5. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_5, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_5. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_5, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_5, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_5, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_5, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_5, 0, 100);
    lv_arc_set_bg_angles(screen_arc_5, 0, 360);
    lv_arc_set_angles(screen_arc_5, 90, 180);
    lv_arc_set_rotation(screen_arc_5, 0);
    lv_obj_set_style_arc_rounded(screen_arc_5, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_5, 0, LV_STATE_DEFAULT);

    // Write codes screen_arc_4
    lv_obj_t *screen_arc_4 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_4, 268, 203);
    lv_obj_set_size(screen_arc_4, 55, 55);
    lv_obj_set_scrollbar_mode(screen_arc_4, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_4, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_4, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_4, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_4, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_4. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_4, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_4. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_4, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_4, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_4, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_4, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_4, 0, 100);
    lv_arc_set_bg_angles(screen_arc_4, 0, 360);
    lv_arc_set_angles(screen_arc_4, 90, 180);
    lv_arc_set_rotation(screen_arc_4, 0);
    lv_obj_set_style_arc_rounded(screen_arc_4, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_4, 0, LV_STATE_DEFAULT);

    // Write codes screen_label_17
    lv_obj_t *screen_label_17 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_17, 196, 116);
    lv_obj_set_size(screen_label_17, 94, 15);
    lv_obj_set_scrollbar_mode(screen_label_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_17, "温度：30.7℃");
    lv_label_set_long_mode(screen_label_17, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_17. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_17, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_17, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_17, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_17, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_17, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_18
    lv_obj_t *screen_label_18 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_18, 195, 135);
    lv_obj_set_size(screen_label_18, 94, 15);
    lv_obj_set_scrollbar_mode(screen_label_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_18, "湿度：30.7%");
    lv_label_set_long_mode(screen_label_18, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_18. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_18, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_18, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_18, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_18, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_18, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_arc_3
    lv_obj_t *screen_arc_3 = lv_arc_create(now_screen);
    lv_obj_set_pos(screen_arc_3, 200, 203);
    lv_obj_set_size(screen_arc_3, 55, 55);
    lv_obj_set_scrollbar_mode(screen_arc_3, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_arc_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_arc_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_arc_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_3, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_arc_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_arc_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_arc_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(screen_arc_3, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_3, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_arc_3. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
    lv_obj_set_style_arc_color(screen_arc_3, lv_color_make(0xf5, 0xa1, 0xe9), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(screen_arc_3, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Set style for screen_arc_3. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(screen_arc_3, lv_color_make(0xf5, 0xa1, 0xe9), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_arc_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_arc_3, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_arc_3, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_arc_3, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_arc_set_mode(screen_arc_3, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(screen_arc_3, 0, 100);
    lv_arc_set_bg_angles(screen_arc_3, 0, 360);
    lv_arc_set_angles(screen_arc_3, 90, 180);
    lv_arc_set_rotation(screen_arc_3, 0);
    lv_obj_set_style_arc_rounded(screen_arc_3, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(screen_arc_3, 0, LV_STATE_DEFAULT);

    // Write codes screen_label_21
    lv_obj_t *screen_label_21 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_21, 281, 223);
    lv_obj_set_size(screen_label_21, 29, 21);
    lv_obj_set_scrollbar_mode(screen_label_21, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_21, "GPU\n温度");
    lv_label_set_long_mode(screen_label_21, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_21. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_21, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_21, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_21, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_21, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_21, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_21, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_19
    lv_obj_t *screen_label_19 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_19, 213, 221);
    lv_obj_set_size(screen_label_19, 29, 21);
    lv_obj_set_scrollbar_mode(screen_label_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_19, "CPU\n温度");
    lv_label_set_long_mode(screen_label_19, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_19. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_19, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_19, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_19, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_19, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_19, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_19, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_19, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_20
    lv_obj_t *screen_label_20 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_20, 349, 223);
    lv_obj_set_size(screen_label_20, 29, 21);
    lv_obj_set_scrollbar_mode(screen_label_20, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_20, "CPU\n占用");
    lv_label_set_long_mode(screen_label_20, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_20. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_20, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_20, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_20, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_20, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_20, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_20, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_20, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_22
    lv_obj_t *screen_label_22 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_22, 417, 221);
    lv_obj_set_size(screen_label_22, 29, 21);
    lv_obj_set_scrollbar_mode(screen_label_22, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_22, "GPU\n占用");
    lv_label_set_long_mode(screen_label_22, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_22. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_22, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_22, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_22, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_22, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_22, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_22, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_22, &lv_chinese_font_10_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_22, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_22, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_22, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_23
    lv_obj_t *screen_label_23 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_23, 213, 268);
    lv_obj_set_size(screen_label_23, 33, 13);
    lv_obj_set_scrollbar_mode(screen_label_23, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_23, "30℃");
    lv_label_set_long_mode(screen_label_23, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_23. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_23, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_23, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_23, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_23, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_23, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_23, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_23, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_24
    lv_obj_t *screen_label_24 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_24, 349, 270);
    lv_obj_set_size(screen_label_24, 33, 13);
    lv_obj_set_scrollbar_mode(screen_label_24, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_24, "30℃");
    lv_label_set_long_mode(screen_label_24, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_24. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_24, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_24, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_24, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_24, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_24, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_24, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_24, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_24, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_24, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_24, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_25
    lv_obj_t *screen_label_25 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_25, 417, 270);
    lv_obj_set_size(screen_label_25, 33, 13);
    lv_obj_set_scrollbar_mode(screen_label_25, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_25, "30℃");
    lv_label_set_long_mode(screen_label_25, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_25. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_25, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_25, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_25, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_25, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_25, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_25, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_25, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_25, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_27
    lv_obj_t *screen_label_27 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_27, 264, 286);
    lv_obj_set_size(screen_label_27, 130, 13);
    lv_obj_set_scrollbar_mode(screen_label_27, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_27, host);
    lv_label_set_long_mode(screen_label_27, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_27. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_27, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_27, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_27, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_27, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_27, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_27, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_27, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_imgbtn_1
    lv_obj_t *screen_imgbtn_1 = lv_imgbtn_create(now_screen);
    lv_obj_set_pos(screen_imgbtn_1, 342, 31);
    lv_obj_set_size(screen_imgbtn_1, 100, 100);
    lv_obj_set_scrollbar_mode(screen_imgbtn_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_imgbtn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(screen_imgbtn_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(screen_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(screen_imgbtn_1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(screen_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    // Set style for screen_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(screen_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(screen_imgbtn_1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(screen_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(screen_imgbtn_1, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(screen_imgbtn_1, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_imgbtn_set_src(screen_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, &_1_alpha_100x100, NULL);
    lv_imgbtn_set_src(screen_imgbtn_1, LV_IMGBTN_STATE_PRESSED, NULL, &_1_alpha_100x100, NULL);
    lv_imgbtn_set_src(screen_imgbtn_1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_1_alpha_100x100, NULL);
    lv_imgbtn_set_src(screen_imgbtn_1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_1_alpha_100x100, NULL);
    lv_obj_add_flag(screen_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);

    // Write codes screen_label_28
    lv_obj_t *screen_label_28 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_28, 260, 71);
    lv_obj_set_size(screen_label_28, 27, 9);
    lv_obj_set_scrollbar_mode(screen_label_28, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_28, "湿度");
    lv_label_set_long_mode(screen_label_28, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_28. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_28, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_28, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_28, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_28, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_28, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_28, &lv_chinese_font_12_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_28, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_clock = {
        screen_label_7,
        screen_label_6,
        screen_label_8};

    lv_computer = {
        screen_label_23,
        screen_label_26,
        screen_label_24,
        screen_label_25,
        screen_arc_3,
        screen_arc_4,
        screen_arc_5,
        screen_arc_6};

    lv_weather = {
        screen_label_10,
        screen_label_11,
        screen_label_12,
        screen_label_13,
        screen_label_14};

    lv_dht11 = {
        screen_label_17,
        screen_label_18,
        screen_arc_1,
        screen_arc_2};

    lv_obj_add_event_cb(now_screen, gesture_home, LV_EVENT_GESTURE, NULL);

    clock_timer = lv_timer_create(pcf8563_timer, 200, (void *)&lv_clock);
    // computer_timer = lv_timer_create(computer_monitor, 3000, (void *)&lv_computer);
    weather_timer = lv_timer_create(weather_timer_cb, 600000, (void *)&lv_weather);
    alarm_timer = lv_timer_create(alarm_timer_cb, 1000, NULL);
    dht11_timer = lv_timer_create(dht11_get, 3000, (void *)&lv_dht11);
    mq2_timer = lv_timer_create(mq2_timer_cb, 1000, NULL);

    return now_screen;
}

static lv_obj_t *kb;
// static void alarm_change(lv_event_t *e)
static void time_change(lv_event_t *e)
{
    int code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_FOCUSED)
    {
        time_changed = 1;
        // 检查当前输入设备类型是否为非键盘
        if (lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_move_foreground(kb);
            // lv_textarea_set_accepted_chars(time_text, "0123456789");`
            // lv_textarea_set_text(time_text, bilibili.UID.c_str());
            // 将键盘对象kb关联到目标对象target，以便在键盘上输入的文本直接显示在目标对象上
            lv_keyboard_set_textarea(kb, target);
            // 清除键盘对象kb的隐藏标志，使其可见
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if (code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_move_background(kb);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
    else if (code == LV_EVENT_CANCEL)
    {
        lv_obj_move_background(kb);
        // lv_textarea_set_text(time_text, bilibili.UID.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}

static void alarm_change(lv_event_t *e)
{
    int code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_FOCUSED)
    {
        alarm_changed = 1;
        // 检查当前输入设备类型是否为非键盘
        if (lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_move_foreground(kb);
            // lv_textarea_set_accepted_chars(time_text, "0123456789");`
            // lv_textarea_set_text(alarm_text, bilibili.UID.c_str());
            // 将键盘对象kb关联到目标对象target，以便在键盘上输入的文本直接显示在目标对象上
            lv_keyboard_set_textarea(kb, target);
            // 清除键盘对象kb的隐藏标志，使其可见
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if (code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_move_background(kb);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
    else if (code == LV_EVENT_CANCEL)
    {
        lv_obj_move_background(kb);
        // lv_textarea_set_text(time_text, bilibili.UID.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}

static void kb_screen_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_target(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void weather_init()
{
    // if (httpFlag > 200)
    // {
    const size_t capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(14) + 390;
    const size_t capacity2 = 2 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(14) + 390;
    String now_https = String("GET /v3/weather/now.json?key=SYQAZcQPIoDp3vOPG&location=qingdao&language=zh-Hans&unit=c&unit=c&start=-1&days=1") + " HTTP/1.1\r\n" + // 请求的是首页信息，HTTP版本号是1.1
                       "Host: " + "api.seniverse.com" + "\r\n" +                                                                                                    // 请求的网址信息
                       "Connection: close\r\n" +                                                                                                                    // 信息发送完毕后断开连接
                       "\r\n";

    String highlow_https = String("GET /v3/weather/daily.json?key=SYQAZcQPIoDp3vOPG&location=qingdao&language=zh-Hans&unit=c&start=0&days=1") + " HTTP/1.1\r\n" + // 请求的是首页信息，HTTP版本号是1.1
                           "Host: " + "api.seniverse.com" + "\r\n" +                                                                                              // 请求的网址信息
                           "Connection: close\r\n" +                                                                                                              // 信息发送完毕后断开连接
                           "\r\n";

    if (weather_client.connect("api.seniverse.com", 80))
    {
        weather_client.print(now_https); // 向服务器发送HTTP请求
        while (weather_client.connected() || weather_client.available())
        { // 服务器保持连接
            if (weather_client.available())
            {
                String line = weather_client.readStringUntil('\n'); // 等待响应并且读取响应信息
                // Serial.println(line);
                DynamicJsonDocument doc(capacity);
                deserializeJson(doc, line);
                JsonObject now = doc["results"][0]["now"];
                tem_now = now["temperature"].as<int>(); // "33"
                update_time = doc["results"][0]["last_update"].as<String>();
                position = doc["results"][0]["location"]["name"].as<String>();
                weather_condition = now["text"].as<String>();
            }
        }
        weather_client.stop();
    }
    // Serial.println(tem_now);
    // Serial.println(weather_condition);
    // // Serial.println(air);
    // Serial.println(update_time);

    // 在你的代码中使用
    update_time = extractTimeFromDateTime(update_time);
    Serial.println(update_time);
    // Serial.println("httpFlag");
    // Serial.println(httpFlag);
    //     httpFlag = 0;
    // }
}

lv_obj_t *home_config_load()
{
    time_changed = 0;
    alarm_changed = 0;
    // Write codes screen
    now_screen = lv_obj_create(NULL);

    // Create keyboard on screen
    kb = lv_keyboard_create(now_screen);
    lv_obj_add_event_cb(kb, kb_screen_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_font(kb, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(now_screen, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_bg_color(now_screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(now_screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(now_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(now_screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes time_text
    time_text = lv_textarea_create(now_screen);
    lv_obj_set_pos(time_text, 222, 30);
    lv_obj_set_size(time_text, 208, 77);
    lv_obj_set_scrollbar_mode(time_text, LV_SCROLLBAR_MODE_OFF);

    // Set style for time_text. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(time_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(time_text, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(time_text, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(time_text, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(time_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(time_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(time_text, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(time_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(time_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(time_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(time_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(time_text, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(time_text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(time_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(time_text, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(time_text, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(time_text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(time_text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(time_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(time_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(time_text, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(time_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for time_text. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(time_text, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(time_text, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(time_text, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    String temp = "";
    temp += "year:" + String(year) + ";month:" + String(month) + ";day:" + String(day) + ";hour:" + String(hour) + ";min:" + String(minutes) + ";sec:" + String(second);
    // Serial.println("time");
    // Serial.println(temp);
    lv_textarea_set_text(time_text, temp.c_str());

    // use keyboard on time_text
    lv_obj_add_event_cb(time_text, time_change, LV_EVENT_ALL, kb);

    // Write codes screen_label_1
    lv_obj_t *screen_label_1 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_1, 73, 56);
    lv_obj_set_size(screen_label_1, 100, 32);
    lv_obj_set_scrollbar_mode(screen_label_1, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_1, "时间");
    lv_label_set_long_mode(screen_label_1, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_1, &lv_chinese_font_16_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_sw_1
    screen_sw_1 = lv_switch_create(now_screen);
    if (ampmFlag == 1)
        lv_obj_add_state(screen_sw_1, LV_STATE_CHECKED);
    else
        lv_obj_clear_state(screen_sw_1, LV_STATE_CHECKED);
    lv_obj_set_pos(screen_sw_1, 244, 124);
    lv_obj_set_size(screen_sw_1, 157, 37);
    lv_obj_set_scrollbar_mode(screen_sw_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_sw_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_sw_1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_sw_1, lv_color_make(0xe6, 0xe2, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_sw_1, lv_color_make(0xe6, 0xe2, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_sw_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_sw_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_sw_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_sw_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_sw_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_sw_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_sw_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(screen_sw_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for screen_sw_1. Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED
    lv_obj_set_style_bg_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(screen_sw_1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(screen_sw_1, 0, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(screen_sw_1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    // Set style for screen_sw_1. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_sw_1, 100, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_sw_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_sw_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_sw_1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(screen_sw_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_sw_1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(screen_sw_1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes screen_label_2
    lv_obj_t *screen_label_2 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_2, 73, 124);
    lv_obj_set_size(screen_label_2, 100, 32);
    lv_obj_set_scrollbar_mode(screen_label_2, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_2, "AM/PM");
    lv_label_set_long_mode(screen_label_2, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_2, &lv_chinese_font_16_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_3
    lv_obj_t *screen_label_3 = lv_label_create(now_screen);
    lv_obj_set_pos(screen_label_3, 73, 187);
    lv_obj_set_size(screen_label_3, 100, 32);
    lv_obj_set_scrollbar_mode(screen_label_3, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(screen_label_3, "闹钟");
    lv_label_set_long_mode(screen_label_3, LV_LABEL_LONG_WRAP);

    // Set style for screen_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_label_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_label_3, &lv_chinese_font_16_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_label_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(screen_label_3, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes alarm_text
    alarm_text = lv_textarea_create(now_screen);
    lv_obj_set_pos(alarm_text, 222, 179);
    lv_obj_set_size(alarm_text, 209, 77);
    lv_obj_set_scrollbar_mode(alarm_text, LV_SCROLLBAR_MODE_OFF);

    // Set style for alarm_text. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(alarm_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(alarm_text, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(alarm_text, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(alarm_text, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(alarm_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(alarm_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(alarm_text, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(alarm_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(alarm_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(alarm_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(alarm_text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(alarm_text, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(alarm_text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(alarm_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(alarm_text, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(alarm_text, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(alarm_text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(alarm_text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(alarm_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(alarm_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(alarm_text, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(alarm_text, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style for alarm_text. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(alarm_text, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(alarm_text, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(alarm_text, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    String temp1 = "";
    temp1 += "hour:" + String(alarmStruct.hours) + ";min:" + String(alarmStruct.minutes) + ";sec:" + String(alarmStruct.seconds);
    // Serial.println("temp");
    // Serial.println(temp1);
    lv_textarea_set_text(alarm_text, temp1.c_str());

    // use keyboard on alarm_text
    lv_obj_add_event_cb(alarm_text, alarm_change, LV_EVENT_ALL, kb);

    // Write codes screen_btn_1
    lv_obj_t *screen_btn_1 = lv_btn_create(now_screen);
    lv_obj_set_pos(screen_btn_1, 356, 256);
    lv_obj_set_size(screen_btn_1, 100, 50);
    lv_obj_set_scrollbar_mode(screen_btn_1, LV_SCROLLBAR_MODE_OFF);

    // Set style for screen_btn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(screen_btn_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(screen_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(screen_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(screen_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(screen_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(screen_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(screen_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(screen_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(screen_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(screen_btn_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(screen_btn_1, &lv_chinese_font_16_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *screen_btn_1_label = lv_label_create(screen_btn_1);
    lv_label_set_text(screen_btn_1_label, "确定");
    lv_obj_set_style_pad_all(screen_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_align(screen_btn_1_label, LV_ALIGN_CENTER, 0, 0);

    // Write codes msgbox_1_ddlist_1
    dlist_1 = lv_dropdown_create(now_screen);
    lv_obj_set_pos(dlist_1, 73, 256);
    lv_obj_set_size(dlist_1, 249, 50);
    lv_obj_set_scrollbar_mode(dlist_1, LV_SCROLLBAR_MODE_OFF);
    lv_dropdown_set_options(dlist_1, "Back in business\nSing\nBeautiful life");
    if (music_choose_flag == 0)
        lv_dropdown_set_selected(dlist_1, 0);
    if (music_choose_flag == 1)
        lv_dropdown_set_selected(dlist_1, 1);
    if (music_choose_flag == 2)
        lv_dropdown_set_selected(dlist_1, 2);
    lv_dropdown_set_symbol(dlist_1, NULL);

    // Set style for msgbox_1_ddlist_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(dlist_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(dlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(dlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(dlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(dlist_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(dlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(dlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(dlist_1, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(dlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(dlist_1, lv_color_make(0xe1, 0xe6, 0xee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dlist_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(dlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(dlist_1, lv_color_make(0x0D, 0x30, 0x55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dlist_1, &lv_chinese_font_24_all, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(dlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(dlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(dlist_1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style state: LV_STATE_CHECKED for style_msgbox_1_ddlist_1_extra_list_selected_checked
    static lv_style_t style_msgbox_1_ddlist_1_extra_list_selected_checked;
    lv_style_init(&style_msgbox_1_ddlist_1_extra_list_selected_checked);
    lv_style_set_radius(&style_msgbox_1_ddlist_1_extra_list_selected_checked, 3);
    lv_style_set_bg_color(&style_msgbox_1_ddlist_1_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
    lv_style_set_bg_grad_color(&style_msgbox_1_ddlist_1_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
    lv_style_set_bg_grad_dir(&style_msgbox_1_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_style_set_bg_opa(&style_msgbox_1_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_msgbox_1_ddlist_1_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
    lv_style_set_border_width(&style_msgbox_1_ddlist_1_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_msgbox_1_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_text_color(&style_msgbox_1_ddlist_1_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_msgbox_1_ddlist_1_extra_list_selected_checked, &lv_chinese_font_12_all);
    lv_obj_add_style(lv_dropdown_get_list(dlist_1), &style_msgbox_1_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Set style state: LV_STATE_DEFAULT for style_msgbox_1_ddlist_1_extra_list_main_default
    static lv_style_t style_msgbox_1_ddlist_1_extra_list_main_default;
    lv_style_init(&style_msgbox_1_ddlist_1_extra_list_main_default);
    lv_style_set_radius(&style_msgbox_1_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_color(&style_msgbox_1_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_grad_color(&style_msgbox_1_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_grad_dir(&style_msgbox_1_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_bg_opa(&style_msgbox_1_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_color(&style_msgbox_1_ddlist_1_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
    lv_style_set_border_width(&style_msgbox_1_ddlist_1_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_msgbox_1_ddlist_1_extra_list_main_default, 255);
    lv_style_set_text_color(&style_msgbox_1_ddlist_1_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
    lv_style_set_text_font(&style_msgbox_1_ddlist_1_extra_list_main_default, &lv_chinese_font_12_all);
    lv_style_set_max_height(&style_msgbox_1_ddlist_1_extra_list_main_default, 90);
    lv_obj_add_style(lv_dropdown_get_list(dlist_1), &style_msgbox_1_ddlist_1_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set style state: LV_STATE_DEFAULT for style_msgbox_1_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_msgbox_1_ddlist_1_extra_list_scrollbar_default;
    lv_style_init(&style_msgbox_1_ddlist_1_extra_list_scrollbar_default);
    lv_style_set_radius(&style_msgbox_1_ddlist_1_extra_list_scrollbar_default, 3);
    lv_style_set_bg_color(&style_msgbox_1_ddlist_1_extra_list_scrollbar_default, lv_color_make(0x00, 0xff, 0x00));
    lv_style_set_bg_opa(&style_msgbox_1_ddlist_1_extra_list_scrollbar_default, 255);
    lv_obj_add_style(lv_dropdown_get_list(dlist_1), &style_msgbox_1_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(screen_btn_1, save_event, LV_EVENT_CLICKED, NULL);

    gesture(0, 1, 1, 0);
    return now_screen;
}

void gesture_home(lv_event_t *event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
        beginFlag = 1;
        Serial.println("OPEN LUNAR CONFIG LOAD ");
        lv_timer_del(clock_timer);
        // lv_timer_del(computer_timer);
        lv_timer_del(weather_timer);
        lv_timer_del(alarm_timer);
        lv_timer_del(dht11_timer);
        lv_timer_del(mq2_timer);
        app_open(lunar_load, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 10);
    }
    else if (dir == LV_DIR_RIGHT)
    {
        beginFlag = 1;
        Serial.println("OPEN HOME CONFIG LOAD ");
        lv_timer_del(clock_timer);
        // lv_timer_del(computer_timer);
        lv_timer_del(weather_timer);
        lv_timer_del(alarm_timer);
        lv_timer_del(dht11_timer);
        lv_timer_del(mq2_timer);
        app_open(home_config_load, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 10);
    }
    else if (dir == LV_DIR_TOP)
    {
        beginFlag = 1;
        lv_timer_del(clock_timer);
        // lv_timer_del(computer_timer);
        lv_timer_del(weather_timer);
        lv_timer_del(alarm_timer);
        lv_timer_del(dht11_timer);
        lv_timer_del(mq2_timer);

        app_open(home_config_load, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 10);
    }
    else if (dir == LV_DIR_BOTTOM)
    {
        beginFlag = 1;
        lv_timer_del(clock_timer);
        // lv_timer_del(computer_timer);
        lv_timer_del(weather_timer);
        lv_timer_del(alarm_timer);
        lv_timer_del(dht11_timer);
        lv_timer_del(mq2_timer);
        app_open(ota_load, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 10);
    }
}