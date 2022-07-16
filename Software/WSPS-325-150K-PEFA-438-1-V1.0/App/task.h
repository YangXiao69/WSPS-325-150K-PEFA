#ifndef __TASK_H
#define __TASK_H
#include "main.h"

#define OFF 0
#define ON  1



struct sDC_Power
{
    uint16_t Temp;
    uint16_t Voltage;
    uint16_t Current; 
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
    uint8_t Modbus_Addr;
    uint8_t Power;    /* ��Դ״̬ �� 1  �� 0 */
    uint8_t RF;
    uint16_t temp;
    uint16_t PowerOut;
    uint16_t PowerR;
    uint8_t LED_Alarm_Power;
    uint8_t LED_Alarm_Curr;
    uint8_t LED_Alarm_Temp;
};


typedef  struct{
	double filterValue;  //k-1ʱ�̵��˲�ֵ������k-1ʱ�̵�ֵ
	double kalmanGain;   //   Kalamn����
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //Ԥ���������ƫ��ķ���
	double R;   //��������ƫ�(ϵͳ����Ժ�ͨ������ͳ��ʵ����)
	double P;   //�������Э����
}  KalmanInfo;

void Device_Init(void);
void delay_ms(uint16_t i);
void Task_Control(void);
void Task_Write_Modbus(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
extern struct sSystem System;
extern struct sAmplifierModule AmplifierModule[13];
extern struct sDC_Power DC_Power[3];
extern uint16_t Modbus[1024];
void Task_Ads8411_Send_Data(void);
void Task_Get_Temp(void);
#endif








