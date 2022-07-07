#include "system.h"

struct sSystem System = {0};

uint16_t AlarmFlag = 0; /* 位定义：0-15  依次： 输入功率   输出功率  ........ 合成器温度 */

void adc_data(uint16_t num1, uint16_t num2, uint16_t num3)
{
    float data1, data2, data3;
    

        if (num1 > 54)
            data1 = num1 - 54;
        else
            data1 = 0;
        
        if (num2 > 54)
            data2 = num2 - 54;
        else
            data2 = 0;
        
        if (num3 > 180)
            data3 = num3 - 180;
        else
            data3 = 0;

        System.Pout = (0.0117 * data1 * data1 + 0.4095 * data1 + 1.967);
        
        System.Pfout = (0.0117 * data2 * data2 + 0.4095 * data2 + 1.967);
        
        if (data3 > 1000)
            System.Pin = (0.00036 * data3 * data3 + 0.113 * data3) * 1000 / 1000;
        else
            System.Pin = (0.00042 * data3 * data3 + 0.053 * data3) * 1000 / 1000;
        
        if(data1 == 0)
        {
            System.Pout = 0;
            System.Pfout = 0;
        }
    
}

void POWER_calculate(void)
{
    uint16_t data1 = 0, data2 = 0, data3 = 0;
    static uint16_t t = 0,i = 0,c = 0,f = 0;
    static uint16_t power_max = 0, powerR_max = 0;
    static uint32_t powerR_add = 0,power_add = 0;
    float date;
    static uint16_t max[5] = {0};
    

    if(power_max < ADC_Value_Power[1])
    {
        power_max = ADC_Value_Power[1];
    }
    if(powerR_max < ADC_Value_Power[0])
    {
        powerR_max = ADC_Value_Power[0];
    }
    t++;
    if(t == 2000)
    {   
        f++;
          
        power_add += power_max;
        powerR_add += powerR_max;        
        if(f >= 8)
        {                               
            data1 = power_add/8;	/*正向功率*/                                               
            data2 = powerR_add/8;	/*内部反射功率*/
            data3 = 0;			    /*外部环形器检波，此型号不支持*/
            
            adc_data(data1, data2, data3);
            
            power_max = 0;
            powerR_max = 0;
            powerR_add = 0;
            power_add = 0;

            f = 0; 
        }
        t = 0;
    }

    for (i = 0; i < 5; i++)
    {
        if (max[i] <= ADC_Value_Power[i+4]) /* 保存最大值 */
            max[i] = ADC_Value_Power[i+4];
    }
    
    c++;
    if (c == 2000)
    {
        date = max[0];
        System.Voltage = (date / 4096 * 56.52f)*10;

        for (i = 1; i < 5; i++)
        {
            date = max[i];
            System.Current[i - 1] = date * 3.3f / 4096 * 80;
        }
        
        c = 0;
        for(i=0;i<5;i++)
        {
            max[i] = 0;
        }

    }    
}


void Get_Temp(void)
{
    static uint16_t Num = 0;

    Num++;
    if(Num >= 2000)
    {
        System.Temp = DS18B20_Get_Temp(1);
        Num = 0;
    }
}




