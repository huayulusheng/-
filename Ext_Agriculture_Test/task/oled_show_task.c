/*
 * Copyright (c) 2023 Beijing HuaQing YuanJian Education Technology Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "oled_show_task.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hi_uart.h"
#include "hi_io.h"
#include "hi_gpio.h"

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "hal_bsp_wifi.h"
#include "hal_bsp_ssd1306.h"
#include "oled_show_log.h"
#include "sys_config.h"
// OLED显示屏显示任务
uint8_t oledShowBuff[50] = {0};
uint8_t led_status = 0;
uint8_t buzzer_status = 0;
extern system_value_t systemdata; // 系统全局变量
extern int a;

// 可燃气体传感器函数
char *get_Flammable_Status(system_value_t sys)
{
    char *data = NULL;
    switch (sys.combustible_status) {
        case FLAMMABLE_STATUS_ON:
            data = "ON  ";
            break;
        case FLAMMABLE_STATUS_OFF:
            data = "OFF";
            break;
             default:
            data = "    ";
            break;
    }
    return data;
}


void oled_show_task(void)
{

    uint8_t test[20]="Farming_Test!";

    while (1) {
            if (sprintf_s((char *)oledShowBuff, sizeof(oledShowBuff), "page2.n23.val=%d\xff\xff\xff",
                          systemdata.CO2) > 0) {
               uart_send_buff(oledShowBuff,sizeof(oledShowBuff)); 
            }
            if (sprintf_s((char *)oledShowBuff, sizeof(oledShowBuff), "page2.n23.val=%d\xff\xff\xff",
                          systemdata.TVOC) > 0) {
              uart_send_buff(oledShowBuff,sizeof(oledShowBuff)); 
            }
             if (sprintf_s((char *)oledShowBuff, sizeof(oledShowBuff), "page2.n20.val=%d\xff\xff\xff ",
                          systemdata.temperature) > 0) {
             uart_send_buff(oledShowBuff,sizeof(oledShowBuff)); 
            }
             if (sprintf_s((char *)oledShowBuff, sizeof(oledShowBuff), "page2.n21.val=%d\xff\xff\xff ",
                          systemdata.humidity) > 0) {
             uart_send_buff(oledShowBuff,sizeof(oledShowBuff)); 
            }


            hi_watchdog_feed();
         
     }
}
