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

#include "uart_recv_task.h"
#include "sys_config.h"
#include "hi_uart.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "stdbool.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "hal_bsp_wifi.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "cmsis_os2.h"
#include "cJSON.h"
#define TCP_SERVER_IP "192.168.43.1"
#define TCP_SERVER_PORT 8090
hi_u8 recvBuff[200] = {0};
hi_u8 *pbuff = recvBuff;
uint8_t Receive_hand = 0x58;        // 串口接收数据，帧头
uint8_t Receive_end = 0xff;         // 串口接收数据，帧尾
uint8_t label = 0; 
void uart_send_buff(unsigned char *str, unsigned short len)
{
    hi_u32 ret = 0;
    ret = hi_uart_write(HI_UART_IDX_1, (uint8_t *)str, len);
    if (ret == HI_ERR_FAILURE)
    {
        printf("uart send buff is failed.\r\n");
    }
}
void rado_control_data(unsigned char *pstr)                      // 雷达控制函数
{
    
    memset_s((char *)pstr, sizeof(pstr), 0, sizeof(pstr));
}

void uart1_recv_task(void)                 //接收数据
{
    hi_u8 rado_buff[20] = {0};
    uint32_t buff_sum;
    while (1)
    {
        
    }
}




void wifi_tcp_task(void)
{
   int socket_fd = 0;
    
    int re = 0;
    char str[40];
    // 连接Wifi
    WiFi_connectHotspots("robot_01", "12345678");
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字（TCP）
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TCP_SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(TCP_SERVER_IP); // 填写服务器的IP地址

    re = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)); // 连接服务器
    if (re == -1) {
        printf("Failed to connect to the server\r\n");
        return;
    }
    printf("Connection to server successful\r\n");

    // 发送第一条数据
    send(socket_fd, "Connection to server successful.", strlen("Connection to server successful."), 0);

    while (1) {
        memset_s(buff, sizeof(buff), 0, sizeof(buff));
        re = recv(socket_fd, buff, sizeof(buff), 0); //	接收客户端发送过来的消息
        if (re <= 0) {
            break;
        } else {
            printf("Receive data received by the server: %s\r\n", buff);
            send(socket_fd, buff, sizeof(buff), 0);
            printf("re = %x\r\n", re);
            for (int i = 0; i < re - 2; i++)
            {
                if (buff[i] == Receive_hand)
                {
                    if (buff[i + 2] == Receive_end)
                    {
                        label = buff[i + 1];
                    }
                }
            }
        }

        send(socket_fd, "Connection to server successful.", strlen("Connection to server successful."), 0);
        sprintf(str[40],"temp:%.1f,humi:%.1f,sun:%d,light:%d,\r\n",systemdata.temperature,systemdata.humidity,systemdata.CO2, systemdata.TVOC);
        send(socket_fd, str, sizeof(str), 0);
 
    }

    close(socket_fd);

}