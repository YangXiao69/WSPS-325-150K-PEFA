#include "task.h"
#include "../../W5500/w5500_conf.h"
#include "../../App/led.h"
#include "bsp_i2c_ee.h"



uint16_t Modbus[1024];
struct sSystem System;

struct sAmplifierModule AmplifierModule[12];
struct sDC_Power DC_Power[3];


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




void Device_Init(void)
{
    I2C_EE_Init();
    
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
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_SET);       
    }
    else
    {
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_RESET); 
        System.RF = 0; 
        System.Pset = 0;
    }
    
    LED_DC_Power(sw);
}

void Task_Control(void)
{
    DC_Power_Control(System.Power);
    Ptt_Control(System.RF);
}

void Task_Write_Modbus(void)
{
    static uint8_t i = 0 ,pA = 0;
    uint8_t Addr;
    
    Modbus[0x0011] = System.Modbus_Addr;
    
    Modbus[0x0080] = System.Power;                  /* ��Դ */
    Modbus[0x0081] = System.RF ;                      /* ��Ƶ */
    Modbus[0x0082] = System.Pset ;                 

    Modbus[0x0086] = System.Duty;
    Modbus[0x0087] = System.PulseWidth;

    Modbus[0x008C] = System.Phase_Dfrc ;        //��λ��    


    Modbus[0x00A1] = System.PowerOut;
    Modbus[0x00A2] = System.PowerR;
    Modbus[0x00A3] = System.PowerZB;
    Modbus[0x00A4] = System.PfoutOut;        /*ǻѹ*/

//    Modbus[0x00A5] = System.Power;                  /* ��Դ */
//    Modbus[0x00A6] = System.RF ;                      /* ��Ƶ */
//    Modbus[0x00A7] = System.Pset ;                 /* ��Ƶ */
//    Modbus[0x00A8] = System.Freq;
//    Modbus[0x00A9] = System.Duty;
//    Modbus[0x00AA] = System.PulseWidth;
    Modbus[0x00AC] = System.Temp.HCQ;
    Modbus[0x00AD] = System.Temp.Cold_Plate_1;
    Modbus[0x00AE] = System.Temp.Cold_Plate_2;
    Modbus[0x00AF] = System.Temp.PCB_Borad;
    
    Modbus[0x00B0] = System.FlowTemp;       /*����*/
    Modbus[0x00B1] = System.Flow;
    Modbus[0x00B2] = System.Mode;
    
    Modbus[0x00E0] = System.Pout_factor[System.Mode];
    Modbus[0x00E1] = System.PRout_factor;
    Modbus[0x00E2] = System.fPout_factor;
    
      
    
    Addr = i * 0x05;
    Modbus[0x0100 + Addr] = AmplifierModule[i].error;   //�澯
    Modbus[0x0101 + Addr] = AmplifierModule[i].Voltage; //��ѹ 
    Modbus[0x0102 + Addr] = AmplifierModule[i].Temp[0]; //�¶� 
    Modbus[0x0103 + Addr] = AmplifierModule[i].Temp[1]; //�¶� 
    Modbus[0x0104 + Addr] = AmplifierModule[i].Current; //����
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


void Task_Ads8411_Receive_Data(void)
{
    System.PowerOut = GPIOG->IDR;

}



