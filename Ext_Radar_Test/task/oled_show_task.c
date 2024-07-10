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
#define temperature_threshold 25 // 检测温度阈值
float arr[8][8];                 // 将64个温度数据放在8*8的二维数组中
uint8_t Receive_hand = 0x58;        // 串口接收数据，帧头
uint8_t Receive_end = 0xff;         // 串口接收数据，帧尾
uint8_t Big_date_index = 0;             // 眼睛随动，最大温度值索引
uint8_t Big_date_last = 0;              // 眼睛随动，最小温度值索引
uint8_t hand_label = 0;                 // 手势标签
uint8_t label = 0;                      // 小狗移动标签
uint8_t eye_move_flag = 0;              // 眼睛动作标志
float pixels[64]; // 红外传感器数据
// OLED显示屏显示任务
uint8_t oledShowBuff[20] = {0};
#define OLED_SHOW_TASK_TIME (50 * 1000) // us
uint8_t led_status = 0;
uint8_t buzzer_status = 0;
static char bUffer[256] = {0};           // 串口接收数据
uint8_t Send_date_hand[1] = {0x57};      // 串口发送数据，帧头
uint8_t Send_date_buf[2] = {0x04, 0x55}; // 串口发送数据，
uint8_t Send_date_end[1] = {0xff};       // 串口发送数据，帧尾
#define TCP_SERVER_IP "192.168.43.1"
#define TCP_SERVER_PORT 8090
char *get_CurrentRadarPattern(system_value_t sys)
{
    int socket_fd;
    char *data = NULL;
    switch (label)
        {
        case 0x10: // 开灯
            send(socket_fd, "light:1,", sizeof("light:1,"), 0);
            label = 0;
            break;
        case 0x11: // 关灯
            send(socket_fd, "light:0,", sizeof("light:0,"), 0);
            label = 0;
            break;
        case 0x12: // 浇水
            send(socket_fd, "water:1,", sizeof("light:1,"), 0);
            label = 0;
            break;
        case 0x13: // 关水
            send(socket_fd, "water:0,", sizeof("light:0,"), 0);
            label = 0;
            break;
        default:
            break;
        }

    
    return data;
}

void oled_show_task(void)
{
    readPixels(pixels, 64);
    while(1){
    uint8_t hang;
    uint8_t lie;
    float date_1 = 0;
    float date_2 = 0;
    float date_3 = 0;
    int x = 0;
    float Big_date = 0;

    for (hang = 0; hang < 8; hang++)
    {
        for (lie = 0; lie < 8; lie++)
        {
            x = x + 1;
            arr[hang][lie] = pixels[x];
            if (pixels[x] > temperature_threshold)
            {
                date_1++;
            }
        }
    }
    x = 0;
    // printf("[");
    // for (int i = 1; i <= 64; i++)
    // {
    //     printf("%.2f", pixels[i - 1]);
    //     printf(", ");
    //     if (i % 8 == 0)
    //         printf("\n");
    // }
    // printf("]\r\n");

    for (hang = 0; hang < 8; hang++)
    {
        for (lie = 3; lie < 5; lie++)
        {
            if (arr[hang][lie] >= temperature_threshold)
            {

                date_2++;
            }
        }
    }
    for (hang = 3; hang < 5; hang++)
    {
        for (lie = 0; lie < 8; lie++)
        {
            if (arr[hang][lie] >= temperature_threshold)
            {
                date_3++;
            }
        }
    }
    if (date_1 > 40 || date_2 > 10 || date_3 > 10)
    {
        if (date_1 / 64 >= 0.8)
        {
            printf("biaoqian :1\r\n");

            hand_label = 1;
        }

        else if (date_2 / 16 >= 0.8)
        {

            printf("biaoqian :2\r\n");
            hand_label = 2;
        }

        else if (date_3 / 16 >= 0.8)
        {

            printf("biaoqian :3\r\n");
            hand_label = 3;
        }
    }
    else
    {
        hand_label = 0;
        Big_date = arr[3][0];
        for (hang = 3; hang < 5; hang++)
        {
            for (lie = 0; lie < 8; lie++)
            {
                if (Big_date < arr[hang][lie])
                {
                    Big_date = arr[hang][lie];
                    Big_date_index = lie;
                }
            }
        }

        if (Big_date > temperature_threshold)
        {
            if (Big_date_index != Big_date_last)
            {
                Send_date_buf[1] = Big_date_index * 4;
                uart_send_buff(Send_date_buf,sizeof(Send_date_buf));
                printf("Big_date : %d\r\n", Send_date_buf[1]);
                eye_move_flag = 1;
                Big_date_last = Big_date_index;
            }
                    
        }
        else if (eye_move_flag == 1)
        {
            Send_date_buf[1] = 15;
            eye_move_flag = 0;
            uart_send_buff(Send_date_buf,sizeof(Send_date_buf));
        }
    }
    date_1 = 0;
    date_2 = 0;
    date_3 = 0;
    }
}
