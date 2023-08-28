#include <lvgl.h>
#include "common.h"
#include <Arduino.h>
#include "driver/lv_port_disp.h"
#include "driver/lv_port_indev.h"
#include "driver/lv_port_fatfs.h"

void fatfs_test();
void my_error_handling();

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    
    lv_init();
    sd_init();
    lv_fs_fatfs_init();

    // fatfs_test();       //测试文件系统读取SD

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}

void my_error_handling()
{
    Serial.println("my_error_handling");
}

void fatfs_test()
{
    lv_fs_file_t f;
    lv_fs_res_t res;
    res = lv_fs_open(&f, "S:test/test.txt", LV_FS_MODE_RD);
    if(res != LV_FS_RES_OK) my_error_handling();

    uint32_t read_num;
    uint8_t buf[20];
    res = lv_fs_read(&f, buf, 20, &read_num);
    if(res != LV_FS_RES_OK || read_num != 20) my_error_handling();

    lv_fs_close(&f);

    Serial.print("read test.txt:");
    for(int i = 0 ; i < 20 && buf[i] != '\0'; i++)
    {
        Serial.write(buf[i]);
    }
    
    Serial.print("\r\n");
}






