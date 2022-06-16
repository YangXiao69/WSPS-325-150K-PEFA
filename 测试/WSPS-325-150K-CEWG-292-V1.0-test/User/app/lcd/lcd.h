#ifndef __LCD_H
#define	__LCD_H

#include "stm32f4xx.h"


extern u8 Reset_Flag;
extern u8 waring_clear_flag;
extern u8 RestFlag;
extern u8 Alarm;              /* 报警总开关 （1为启动）*/
extern u8 Alarm_Volt;   
extern u8 Alarm_Curr;      
extern u8 Alarm_Temp;            /* 温度报警 */
extern u8 Alarm_Pout;           /* 总功率报警 */    
extern u8 Alarm_Pfout;           /* 反射功率报警 */
extern u8 Alarm_ZB;           /* 驻波报警 */


extern u16 AlarmFlag;


extern u16 Alarm_Pout_Value       ;  //功率保护门限
extern u16 Alarm_Pfout_Value       ;  //功率保护门限
 
extern u16 Alarm_RPD_MODE         ;  //反射保护模式
extern u16 Alarm_RPD_Value        ;  //反射保护门限
extern u16 Alarm_Vswr_MODE        ;  //驻波保护模式
extern u8 Alarm_Vswr_Value       ;  //驻波保护门限
extern u16 Alarm_Flow_Value ;
extern u16 Alarm_FlowTemp_Value;
 
extern u16 Alarm_Temp2_Value       ;  //温度保护门限
extern u8 Alarm_Curr2_Value       ;  //电流保护门限
extern u8 Alarm_Vswr2_Value ;   // 插件                               
extern u16 Alarm_Pout2_Value;   // 插件

extern u8 erroradr;

extern u8 Power ;              /* 电源状态 开 1  关 0 */
extern u8 RF ;                 /* 射频     开 1  关0  */
extern u8 WorkMode ;           /* 工作模式 连续 1   脉冲 0*/
extern u8 Duty ;               /* 占空比 */
extern u8 Ttime ;              /* 周期 */
extern u16 Pset ;              /* 链路衰减 */
extern u16 Phase ;             /* 相位 */
extern u16 PulseWidth ;        /* 脉宽 */

extern u8 Power0;                 /* 电源状态 开 1  关 0 */
extern u8 RF0;                    /* 射频     开 1  关0  */
extern u8 WorkMode0;              /* 工作模式 连续 1   脉冲 0*/
	
extern u8 operate;                /* 操作 */
extern u8 Adr;
extern u32 bound;

extern u8 mtset;
extern u8 Source;
extern u8 PWMS;
extern u8 Source0;            /* 源 */
extern u8 PWMS0;              /* pwm源 */

extern u16 Freq;

extern u16 RegisterAddr;



extern u8 time[5];                /* 读取的时间暂存 */
extern u8 time0[5];               /* 返回的BCD时间值 */
extern u16 user_settings[2];


void lcd_receive(void);



void LCD_DisPA(uint8_t IPnum,uint8_t * Rx_b);
void LCD_DisErr(uint8_t adr_H, uint8_t adr_L);



void lcd_send_one(u16 data,u16 adrss) ;
void display_power(void);
void Wait_USART1_Send(void);
void lcd_send_pageone(void) ;
void lcd_send_pagetwo(void) ;
void lcd_send_pagemain(void);
void clear_lcd_display(u8 num);

void Send_for_waring_clear(void) ;



#endif 
