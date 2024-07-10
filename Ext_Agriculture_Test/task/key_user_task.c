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

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "hal_bsp_pwm_rgb.h"
#include "hal_bsp_sgp30.h"
#include "hi_pwm.h"
#include "hi_adc.h"

#include "hal_bsp_pcf8574.h"
#include "hal_bsp_aw2013.h"
#include "hi_uart.h"
#include "sys_config.h"

#include "hal_bsp_pcf8574.h"
#include "hal_bsp_wifi.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"
#include "hal_bsp_sht20.h"
#include "uart_recv_task.h"
#define KEY HI_IO_NAME_GPIO_14    // WiFi模组的IO14引脚
extern system_value_t systemdata; // 系统全局变量
osThreadId_t Task1_ID = 0;        //  任务1 ID
hi_gpio_value val, val_last;      // GPIO的状态值
#define TASK_STACK_SIZE 1024
#define TASK_DELAY_TIME (200 * 1000)
uint16_t light_set[4]={0,0,0,0 };
uint16_t pwm1_arr,pwm2_arr;
uint32_t i;
uint8_t dblclick;

uint16_t sun=0;
// extern tn_pcf8574_io_t tmp_io = {0}; // IO扩展芯片的引脚
char buff[256];
uint16_t munValue,dayValue,houValue,minValue,decValue,weekValue,lightValue;
uint16_t tempValue, humiValue,sunValue;
#define TCP_SERVER_IP "192.168.43.1"
#define TCP_SERVER_PORT 8090



/**
 * @description: 任务1
 * @param {*}
 * @return {*}
 */
void flame_SGP30_test_task(void)     //可燃气体传感器函数
{
  while (1)
  {
    uint8_t data = 0;
    PCF8574_Read(&data);           //读取PCF8574IO扩展芯片
    if (data & 0x80)
    {
      systemdata.combustible_status = FLAMMABLE_STATUS_OFF;
    }
    else
    {
      systemdata.combustible_status = FLAMMABLE_STATUS_ON;
    }

    SGP30_ReadData(&systemdata.CO2, &systemdata.TVOC);    //获取CO2和TVOC数据
    sleep(2);
  }
}

hi_void gpio_callback(void)
{
  hi_gpio_get_input_val(KEY, &val); 
  if(val==HI_GPIO_VALUE0)
  {
    dblclick++;
  }
}
/**
 * @description: 任务1
 * @param {*}
 * @return {*}
 */
void key_test_task(void)                    //控制电机函数
{
  printf("enter Task 1.......\r\n");
  hi_gpio_init();                                            // GPIO初始化
  hi_io_set_pull(KEY, HI_IO_PULL_UP);                        // 设置GPIO上拉
  hi_io_set_func(KEY, HI_IO_FUNC_GPIO_14_GPIO);              // 设置IO14为GPIO功能
  hi_gpio_set_dir(KEY, HI_GPIO_DIR_IN);                      // 设置GPIO为输入模式
  hi_gpio_register_isr_function(KEY,                         // KEY按键引脚
                                HI_INT_TYPE_EDGE,            // 下降沿检测
                                HI_GPIO_EDGE_FALL_LEVEL_LOW, // 低电平时触发
                                &gpio_callback,              // 触发后调用的回调函数
                                NULL);                       // 回调函数的传参值
    
  
  while (1)
  {


        hi_pwm_stop(HI_PWM_PORT_PWM0);
        hi_pwm_stop(HI_PWM_PORT_PWM1);
        hi_pwm_start(HI_PWM_PORT_PWM0, 1000-sun, 1000);
        hi_pwm_start(HI_PWM_PORT_PWM1, 1000-sun, 1000);
        // IoTGpioSetOutputVal(9, 1);
        // usleep(300000);
        // IoTGpioSetOutputVal(9, 0);
        // usleep(300000);
        if(recedata==1)
				{
					if(recvBuff[1]== 0x50)
					{
						light_set[1]=recvBuff[2];
					}
					
					if(recvBuff[1]== 0x53)
					{
						if(recvBuff[2]==0x01)
						{
							light_set[0]=1;
						}
						else
						{
							light_set[0]=0;
						}
					}
					if(recvBuff[1]== 0x51)
					{
						uint16_t k = 999*(100/light_set[1]);
						if(recvBuff[2]==0x01)
						{
							
							if(light_set[0]==0)
							{
								
							pwm1_arr= k;
							pwm2_arr=	999-k;
							}
							if(light_set[0]==1)
							{
								for(uint16_t i=0;i<k;i++)
								{
									
									hi_pwm_stop(HI_PWM_PORT_PWM0);
                  hi_pwm_start(HI_PWM_PORT_PWM0, 1000-i, pwm1_arr);
                  osDelay(1);

								}
								for(uint16_t i=0;i<(999-k);i++)
								{
									
									
									hi_pwm_stop(HI_PWM_PORT_PWM1);
                  hi_pwm_start(HI_PWM_PORT_PWM1, 1000-i, pwm2_arr);
                  osDelay(1);
								}
								
								hi_pwm_stop(HI_PWM_PORT_PWM0);
                hi_pwm_stop(HI_PWM_PORT_PWM1);
                hi_pwm_start(HI_PWM_PORT_PWM0, 1000-sun, pwm1_arr);
                hi_pwm_start(HI_PWM_PORT_PWM1, 1000-sun, pwm2_arr);
							}
							
						}
						else 
						{
							if(light_set[0]==0)
							{
							pwm1_arr= 0;
							pwm2_arr=	0;
							}
							if(light_set[0]==1)
							{
								for(uint16_t i=k;i>0;i--)
								{
									
									hi_pwm_stop(HI_PWM_PORT_PWM0);
                  hi_pwm_start(HI_PWM_PORT_PWM0, 1000-i, pwm1_arr);

									osDelay(1);
								}
								for(uint16_t i=(999-k);i>0;i--)
								{
									hi_pwm_stop(HI_PWM_PORT_PWM1);
                  hi_pwm_start(HI_PWM_PORT_PWM1, 1000-i, pwm2_arr);
                  osDelay(1);
								}
								hi_pwm_stop(HI_PWM_PORT_PWM0);
                hi_pwm_stop(HI_PWM_PORT_PWM1);
                hi_pwm_start(HI_PWM_PORT_PWM0, 0, pwm1_arr);
                hi_pwm_start(HI_PWM_PORT_PWM1, 0, pwm2_arr);
							}
						}
					}

					recedata=0;
				}



  }
}
void gas_test_task(void)         //温度，光敏
{
  hi_u32 ret;
  hi_u16 data;
  hi_float GAS;
  float gas_mic;
  float temperature = 0, humidity = 0;
  while (1)
  {
    char* munPtr = strstr(buff, "mun:"); // 查找月标识符
			char* dayPtr = strstr(buff, "day:"); // 查找日标识符
			char* houPtr = strstr(buff, "hou:"); // 查找时标识符
			char* minPtr = strstr(buff, "min:"); // 查找分标识符
			char* decPtr = strstr(buff, "dec:"); // 查找秒标识符
			char* weekPtr = strstr(buff, "week:"); // 查找秒标识符
			char* lightPtr = strstr(buff, "light:"); // 查找秒标识符
			sscanf(munPtr+strlen("mun:"),"%d", &munValue); // 提取月份
			sscanf(dayPtr+strlen("day:"),"%d", &dayValue); // 提取日份
			sscanf(houPtr+strlen("hou:"),"%d", &houValue); // 提取时份
			sscanf(minPtr+strlen("min:"),"%d", &minValue); // 提取分份
			sscanf(decPtr+strlen("dec:"),"%d", &decValue); // 提取秒份
			sscanf(weekPtr+strlen("mun:"),"%d", &weekValue); // 提取周份
			sscanf(lightPtr+strlen("light:"),"%d", &lightValue); // 提取灯
      if(lightValue==1)
			{
        hi_pwm_start(HI_PWM_PORT_PWM0, 1000, 1000);
        hi_pwm_start(HI_PWM_PORT_PWM1, 1000, 1000);
      }
      else 
      {
        hi_pwm_start(HI_PWM_PORT_PWM0, 0, 1000);
        hi_pwm_start(HI_PWM_PORT_PWM1, 0, 1000);
      }

    char* tempPtr = strstr(recvBuff, "temp:"); // 查找温度标识符
    char* humiPtr = strstr(recvBuff, "humi:"); // 查找湿度标识符
		char* sunPtr = strstr(recvBuff, "sun:"); // 查找湿度标识符
     if (tempPtr && humiPtr) {
        
        sscanf(tempPtr + strlen("temp:"), "%d", &tempValue); // 提取温度值
        sscanf(humiPtr + strlen("humi:"), "%d", &humiValue); // 提取湿度值
			  sscanf(sunPtr + strlen("sun:"), "%d", &sunValue); // 提取光照值
		if(sun<=sunValue)
		{
        hi_pwm_stop(HI_PWM_PORT_PWM0);
        hi_pwm_stop(HI_PWM_PORT_PWM1);
        hi_pwm_start(HI_PWM_PORT_PWM0, 1000-sun, 1000);
        hi_pwm_start(HI_PWM_PORT_PWM1, 1000-sun, 1000);
			
		}
		if(humidity >humiValue)
		{

			set_fan(true); // 风扇打开
      // IoTGpioSetOutputVal(9, 1);
		}
    else
    {
      set_fan(false); // 风扇关
      // IoTGpioSetOutputVal(9, 0);
    }
    if(temperature <tempValue)
		{
      IoTGpioSetOutputVal(11, 1);
			// set_D2(true); // 加热丝打开
		}
    else
    {
      IoTGpioSetOutputVal(11, 0);
      // set_D2(false); // 加热丝关
    }
        
    }

        SHT20_ReadData(&temperature, &humidity);
        printf("temperature = %.2f      humidity = %.2f\r\n", temperature, humidity);


    ret = hi_adc_read(HI_ADC_CHANNEL_6, &data, HI_ADC_EQU_MODEL_4,   //读取ADC数值
                      HI_ADC_CUR_BAIS_DEFAULT, 0);

    if (ret != HI_ERR_SUCCESS)
    {
      printf("ADC Read Fail\n");
    }
    GAS = hi_adc_convert_to_voltage(data);    //将ADC数值转换为电压
    gas_mic = GAS / 3.3 * 1000;
    sun=gas_mic;

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
        }

        send(socket_fd, "Connection to server successful.", strlen("Connection to server successful."), 0);
        sprintf(str[40],"temp:%.1f,humi:%.1f,sun:%d,light:%d,\r\n",systemdata.temperature,systemdata.humidity,systemdata.CO2, systemdata.TVOC);
        send(socket_fd, str, sizeof(str), 0);
 
    }

    close(socket_fd);

}