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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "hi_io.h"
#include "hi_gpio.h"
#include "hi_uart.h"
#include "stdbool.h"

#include "hal_bsp_ssd1306.h"
#include "hal_bsp_nfc.h"
#include "hal_bsp_nfc_to_wifi.h"
#include "hal_bsp_wifi.h"
#include "hal_bsp_pcf8574.h"
#include "hal_bsp_aw2013.h"
#include "hal_bsp_Amg8833.h"
#include "sys_config.h"
#include "oled_show_log.h"
#include "oled_show_task.h"
#include "uart_recv_task.h"

#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"

osThreadId_t oled_show_task_id;
osThreadId_t uart_recv_task_id;
osThreadId_t uart1_recv_task_id;
osThreadId_t udp_send_task_id;
osThreadId_t udp_recv_task_id;
osThreadId_t wifi_tcp_task_id;
tn_pcf8574_io_t pcf8574_io;




uint8_t i = 0;

float pixels[64]; // 红外传感器数据

void Amg8833_discern_label(void); // 红外手势识别函数
void Send_date(uint8_t *data);    // 发送数据函数

static char bUffer[256] = {0};           // 串口接收数据
uint8_t Send_date_hand[1] = {0x57};      // 串口发送数据，帧头
uint8_t Send_date_buf[2] = {0x04, 0x55}; // 串口发送数据，
uint8_t Send_date_end[1] = {0xff};       // 串口发送数据，帧尾

uint8_t Receive_date_buf[1] = {0};  // 串口接收数据
uint8_t Receive_date_hand_flag = 0; // 串口接收数据，帧头标志位
uint8_t Receive_date_end_flag = 0;  // 串口接收数据，帧尾标志位


static int client_connect_socket = 0;
char buff[50] = "hello, I am tcp_client!";

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1




system_value_t systemValue = {0}; // 系统全局变量


#define TASK_STACK_SIZE (1024 * 5)

/**
 * @brief  串口初始化
 * @note   与STM32单片机之间的串口通信
 * @retval None
 */
void uart_init(void)
{
    uint32_t ret = 0,ret1 = 0;
    // 初始化串口
    hi_io_set_func(HI_IO_NAME_GPIO_5, HI_IO_FUNC_GPIO_5_UART1_RXD);
    hi_io_set_func(HI_IO_NAME_GPIO_6, HI_IO_FUNC_GPIO_6_UART1_TXD);

    hi_io_set_func(HI_IO_NAME_GPIO_11, HI_IO_FUNC_GPIO_11_UART2_TXD);
    hi_io_set_func(HI_IO_NAME_GPIO_12, HI_IO_FUNC_GPIO_12_UART2_RXD);

    hi_uart_attribute uart_param = {
        .baud_rate = 115200,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,
    };
    ret = hi_uart_init(HI_UART_IDX_2, &uart_param, NULL);
    if (ret != HI_ERR_SUCCESS) {
        printf("hi uart init is faild.\r\n");
    }

     ret1 = hi_uart_init(HI_UART_IDX_1, &uart_param, NULL);
    if (ret1 != HI_ERR_SUCCESS) {
        printf("hi uart init is faild.\r\n");
    }
}

void SC_peripheral_init(void)
{
    /********************************** 外设初始化 **********************************/

    amg8833_init();

    uart_init(); // 串口初始化
}
/**
 * @brief  人体雷达的入口函数
 * @note
 * @retval None
 */
static void smartRadar_example(void)
{
     SC_peripheral_init();
    /********************************** 创建线程 **********************************/
    osThreadAttr_t options;
    options.attr_bits = 0;
    options.cb_mem = NULL;
    options.cb_size = 0;
    options.stack_mem = NULL;
    options.stack_size = TASK_STACK_SIZE;
    /********************************** 小狗脸脚任务 **********************************/
    options.name = "uart1_recv_task";
    options.priority = osPriorityNormal;
    uart1_recv_task_id = osThreadNew((osThreadFunc_t)uart1_recv_task, NULL, &options);
    if (uart1_recv_task_id != NULL) {
        printf("ID = %d, Create uart1_recv_task_id is OK!\r\n", uart1_recv_task_id);
    }
    /********************************** 红外热成像任务 **********************************/
    options.name = "oled_show_task";
    options.priority = osPriorityNormal;
    oled_show_task_id = osThreadNew((osThreadFunc_t)oled_show_task, NULL, &options);
    if (oled_show_task_id != NULL) {
        printf("ID = %d, Create oled_show_task_id is OK!\r\n", oled_show_task_id);
    }
     /********************************** 联网获取信息**********************************/
    options.name = "wifi_user_task";
    options.priority = osPriorityNormal;
    wifi_tcp_task_id = osThreadNew((osThreadFunc_t)wifi_tcp_task, NULL, &options);
    if (wifi_tcp_task_id != NULL) {
        printf("ID = %d, Create wifi_tcp_id is OK!\r\n", wifi_tcp_task_id);
    }

}
SYS_RUN(smartRadar_example);
