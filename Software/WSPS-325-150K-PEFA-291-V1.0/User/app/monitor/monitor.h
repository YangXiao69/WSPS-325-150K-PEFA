#ifndef __MONITOR_H
#define __MONITOR_H



#include "stm32l4xx.h"




extern uint8_t Power;            /* 报警总开关 （1为启动）*/
 
extern uint8_t Power_V;            /* 电压报警 */
extern uint8_t Power_I;            /* 电流报警 */
extern uint8_t Power_T;            /* 温度报警 */
extern uint8_t Power_VP;           /* 总功率报警 */
extern uint8_t Power_VF;           /* 反射功率报警 */
extern uint8_t Power_ZB;           /* 驻波报警 */

extern uint8_t ErrorFlag;         /* 非0代表异常   0xff代表已经处理过异常*/

extern uint8_t PowerFlag[2];  /* 异常位表示数组 */



extern uint8_t Alarm_Curr_Value;           // 插件
extern uint8_t Alarm_Vswr_Value;           // 插件
extern uint16_t Alarm_Pout_Value;          // 插件
extern uint16_t Alarm_Temp_Value;          // 插件



void Timing_Protect_Control(void);

void set_data(uint8_t *paTxbuff);

void error_serve(void);

void CURR_calculate(void);

#endif

