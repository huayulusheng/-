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

#include "cmsis_os2.h"
#include "cJSON.h"

hi_u8 recvBuff[200] = {0};
hi_u8 *pbuff = recvBuff;
hi_u8 recedata=0;
void uart_send_buff(unsigned char *str, unsigned short len)
{
    hi_u32 ret = 0;
    ret = hi_uart_write(HI_UART_IDX_2, (uint8_t *)str, len);
    if (ret == HI_ERR_FAILURE)
    {
        printf("uart send buff is failed.\r\n");
    }
}



void uart2_recv_task(void)
{
    hi_u8 uart_buff[20] = {0};

    hi_u8 last_len = 0;
    while (1)
    {
        // //阻塞接收串口2
        if (memset_s((char *)uart_buff, sizeof(recvBuff), 0, sizeof(uart_buff)) == 0)
        {
            hi_u32 len = hi_uart_read(HI_UART_IDX_2, uart_buff, sizeof(uart_buff));
            if (len > 0||recedata==0)
            {
                memcpy_s((char *)pbuff, len, (char *)uart_buff, len);
                pbuff += len;
                if (len < last_len)
                {
                    pbuff = recvBuff; // 回到recvBuff的头位置
                    printf("buff: %s\n", recvBuff);

                    recedata=1;
                    memset_s((char *)recvBuff, sizeof(recvBuff), 0, sizeof(recvBuff));
                }
                last_len = len;
            }
        }
    }
}
