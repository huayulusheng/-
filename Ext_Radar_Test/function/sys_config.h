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

#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H
#include <stdint.h>

#include "hal_bsp_structAll.h"

// 雷达的当前状态值
typedef enum {
    Radar_STATUS_ON = 0x01,   // 开启
    Radar_STATUS_OFF,        // 关闭

} te_radar_status_t;

/*********************************** 系统的全局变量 ***********************************/
typedef struct _system_value {
    te_radar_status_t radar_status; //雷达的状态
    uint32_t distance;
} system_value_t;
extern system_value_t systemValue; // 系统全局变量

extern tn_pcf8574_io_t pcf8574_io;
extern uint8_t Receive_hand;        // 串口接收数据，帧头
extern uint8_t Receive_end;         // 串口接收数据，帧尾
#define IO_FAN pcf8574_io.bit.p0
#define IO_BUZZER pcf8574_io.bit.p1
#define IO_LED pcf8574_io.bit.p2

#endif