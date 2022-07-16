#include "task.h"
#include "../../W5500/w5500_conf.h"
#include "../../App/led.h"
#include "bsp_i2c_ee.h"
#include "bsp_ds18b20.h"
#include "ds18b20.h"

uint16_t Modbus[1024];
struct sSystem System;

struct sAmplifierModule AmplifierModule[13];
struct sDC_Power DC_Power[3];
KalmanInfo KFP_height={0,0,1,1,0.1,2,0};


//��ʱnus
// nusΪҪ��ʱ��us��.
void delay_us(uint32_t nus)
{
    uint32_t temp = 0;
    if (nus > 39000)
    {
        nus = 39000;
    }

    SysTick->LOAD = nus * 480;                //ʱ�����
    SysTick->VAL = 0x00;                      //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL = 0x00;                       //��ռ�����
}

//��ʱnms
//ע��nms�ķ�Χ
// SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(uint16_t nms)
{
  uint32_t temp = 0;
  if (nms > 39)
  {
    nms = 39;
  }

  SysTick->LOAD = (uint32_t)nms * 48000;    //ʱ�����(SysTick->LOADΪ24bit)
  SysTick->VAL = 0x00;                      //��ռ�����
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //�رռ�����
  SysTick->VAL = 0x00;                            //��ռ�����
}



double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//Ԥ����һʱ�̵�ֵ
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x�������������һ��ʱ���ĺ������ֵ��������Ϣ�������˴���Ҫ���ݻ�վ�߶���һ���޸�
	
	//��Э����
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //������������� p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //��¼�ϴ�ʵ�������ֵ
 
	//����kalman����
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H�� / (H P(k|k-1) H�� + R)
	//����������������˲�ֵ
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//���º������
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//������������  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}//

void Device_Init(void)
{
    I2C_EE_Init();
    
    DS18B20_GPIO_Config();
    
    w5500_init();       /*spi4  udp*/
    socket_buf_init(txsize, rxsize); /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

    w5500_top_init();   /*spi2   tcp*/
    socket_buf_init_top(txsize, rxsize); /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
    
}

void Ptt_Control(uint8_t sw)
{
    if(sw == ON)
    {
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_SET);        
    }
    else
    {
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_RESET); 
    }

}

void DC_Power_Control(uint8_t sw)           /*һ��ػ��Ƿֿ���*/
{
    if(sw == ON)
    {
        HAL_GPIO_WritePin(Power_SW_GPIO_Port,Power_SW_Pin,GPIO_PIN_RESET);       
    }
    else
    {
        HAL_GPIO_WritePin(Power_SW_GPIO_Port,Power_SW_Pin,GPIO_PIN_SET); 
        System.RF = 0; 
    }
    
    LED_DC_Power(sw);
}

void Error_Control(void)
{
    if(System.LED_Alarm_Power != 0 ||System.LED_Alarm_Curr != 0 || System.LED_Alarm_Temp)
    {
        LED_Alarm(1);
    }
    else
    {
        LED_Alarm(0);
    }
}

void Task_Control(void)
{
    DC_Power_Control(System.Power);
    Ptt_Control(System.RF);
    Error_Control();
}

void Task_Write_Modbus(void)
{
    static uint8_t i = 0 ,pA = 0;
    
    uint8_t Addr;
        
    Modbus[0x0080] = System.Power;                  /* ��Դ */
    Modbus[0x0081] = System.RF ;                    /* ��Ƶ */
    Modbus[0x0082] = System.LED_Alarm_Power ;        
    Modbus[0x0083] = System.LED_Alarm_Curr ;     
    Modbus[0x0084] = System.LED_Alarm_Temp ;       
    Modbus[0x0085] = System.temp;
    Modbus[0x0086] = System.PowerOut;
    Modbus[0x0086] = System.PowerR;
    
    Addr = i * 0x09;
    Modbus[0x0100 + Addr] = AmplifierModule[i].error;   //�澯
    Modbus[0x0101 + Addr] = AmplifierModule[i].Voltage; //��ѹ
    Modbus[0x0102 + Addr] = AmplifierModule[i].Powerout;
    Modbus[0x0103 + Addr] = AmplifierModule[i].PowerR;
    Modbus[0x0104 + Addr] = AmplifierModule[i].Current[0];
    Modbus[0x0105 + Addr] = AmplifierModule[i].Current[1];
    Modbus[0x0106 + Addr] = AmplifierModule[i].Current[2];
    Modbus[0x0107 + Addr] = AmplifierModule[i].Current[3];
    Modbus[0x0108 + Addr] = AmplifierModule[i].Temp;
    i++;
    if(i >= 12)
        i = 0;
    
    Addr = pA * 0x03;
    Modbus[0x300 + Addr] = DC_Power[pA].Voltage;
    Modbus[0x301 + Addr] = DC_Power[pA].Current; 
    Modbus[0x302 + Addr] = DC_Power[pA].Temp;
    pA++;
    if(pA >= 3)
        pA = 0;
}


void Task_Ads8411_Send_Data(void)
{
    if(HAL_GPIO_ReadPin(S_CLK_GPIO_Port, S_CLK_Pin))
    {
        System.PowerOut= GPIOG->IDR;
    }
    else
    {
        System.PowerR = GPIOG->IDR;
    }

}


void Task_Get_Temp(void)
{
    static uint16_t t_num;
    
    t_num++;
    if(t_num >= 10000)
    {
        System.temp = DS18B20_Get_Temp(1);
        t_num = 0;
    }
}
