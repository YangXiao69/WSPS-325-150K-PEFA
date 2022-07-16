#ifndef __TASK_H
#define __TASK_H
#include "main.h"

#define OFF 0
#define ON  1



struct sError_Num
{
    uint16_t Powerout;
    uint16_t PfoutOut;
    uint16_t Vswr;
    uint16_t Temp;
    uint16_t Flow;
    uint16_t FlowTemp;
    uint16_t Module[16];
    uint16_t Lighter;
    
};

struct sAlarm
{
    uint8_t Vswr_Value; // 整机
    uint16_t Pout_Value;     // 整机
    uint16_t Pfout_Value;    // 整机
    uint16_t Flow_Value;
    uint16_t FlowTemp_Value;
    uint16_t HightPlay_Switch;
    uint16_t HightPlay_Value;
    uint16_t HightPlay_Num;
    uint16_t Voltage_Value;
    uint16_t Current_Value;
    uint16_t Temp_Value;
    uint8_t Magnetic;
    uint8_t Interlock;
    uint8_t Dev_EN;
    
    uint16_t Reset_Flag;
    uint16_t AlarmError; //告警
    uint8_t AM_Alarm;
  
    struct sError_Num Error_Num;
    uint8_t Flag_timer_SE;
    uint8_t flag_stop;
};

struct sDC_Power
{
    uint16_t Temp;
    uint16_t Voltage;
    uint16_t Current; 
};

struct sSlave_Device
{
    uint16_t temp;
    uint16_t PowerOut;
    uint16_t PowerR;
};

struct sAmplifierModule
{
    uint8_t Addr;
    uint16_t Powerout;
    uint16_t PowerR;
    uint16_t Voltage;
    uint16_t Current[4];
    uint16_t Temp;
    uint16_t error;
};

struct sSystem
{
    uint8_t Power;    /* 电源状态 开 1  关 0 */
    uint8_t RF;
    uint8_t Duty;     /* 占空比 */
    uint16_t Pset;        /* 设定功率 */
    uint16_t PulseWidth; /* 脉宽 */    
    uint16_t Mode;
    uint16_t Temp;
    
    float PowerOut;
    float PfoutOut;
    float PowerR;
    float PowerZB;

    uint16_t Pout_factor[2];
    uint16_t PRout_factor;
    uint16_t fPout_factor;
    uint8_t Modbus_Addr;
        
};

void Device_Init(void);
void delay_ms(uint16_t i);
void Task_Control(void);
void Task_Write_Modbus(void);

extern struct sSystem System;
extern struct sAmplifierModule AmplifierModule[48];
extern struct sDC_Power DC_Power[12];
extern struct sSlave_Device Slave_Device[4];

extern uint16_t Modbus[1024];
void Task_Ads8411_Receive_Data(void);
void Task_Get_temp(void);

void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif








