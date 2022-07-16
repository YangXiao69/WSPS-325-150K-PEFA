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


//延时nus
// nus为要延时的us数.
void delay_us(uint32_t nus)
{
    uint32_t temp = 0;
    if (nus > 39000)
    {
        nus = 39000;
    }

    SysTick->LOAD = nus * 480;                //时间加载
    SysTick->VAL = 0x00;                      //清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
    SysTick->VAL = 0x00;                       //清空计数器
}

//延时nms
//注意nms的范围
// SysTick->LOAD为24位寄存器,所以,最大延时为:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(uint16_t nms)
{
  uint32_t temp = 0;
  if (nms > 39)
  {
    nms = 39;
  }

  SysTick->LOAD = (uint32_t)nms * 48000;    //时间加载(SysTick->LOAD为24bit)
  SysTick->VAL = 0x00;                      //清空计数器
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
  do
  {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //关闭计数器
  SysTick->VAL = 0x00;                            //清空计数器
}



double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//预测下一时刻的值
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
	
	//求协方差
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //记录上次实际坐标的值
 
	//计算kalman增益
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
	//修正结果，即计算滤波值
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//更新后验估计
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}//

void Device_Init(void)
{
    I2C_EE_Init();
    
    DS18B20_GPIO_Config();
    
    w5500_init();       /*spi4  udp*/
    socket_buf_init(txsize, rxsize); /*初始化8个Socket的发送接收缓存大小*/

    w5500_top_init();   /*spi2   tcp*/
    socket_buf_init_top(txsize, rxsize); /*初始化8个Socket的发送接收缓存大小*/
    
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

void DC_Power_Control(uint8_t sw)           /*一起控还是分开控*/
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
        
    Modbus[0x0080] = System.Power;                  /* 电源 */
    Modbus[0x0081] = System.RF ;                    /* 射频 */
    Modbus[0x0082] = System.LED_Alarm_Power ;        
    Modbus[0x0083] = System.LED_Alarm_Curr ;     
    Modbus[0x0084] = System.LED_Alarm_Temp ;       
    Modbus[0x0085] = System.temp;
    Modbus[0x0086] = System.PowerOut;
    Modbus[0x0086] = System.PowerR;
    
    Addr = i * 0x09;
    Modbus[0x0100 + Addr] = AmplifierModule[i].error;   //告警
    Modbus[0x0101 + Addr] = AmplifierModule[i].Voltage; //电压
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
