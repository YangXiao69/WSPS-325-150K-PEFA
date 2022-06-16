#include "./../../main.h"

#include <math.h>

uint8_t Power   = 0;            /* �����ܿ��� ��1Ϊ������*/

uint8_t Power_V = 0;            /* ��ѹ���� */
uint8_t Power_I = 0;            /* �������� */
uint8_t Power_T = 0;            /* �¶ȱ��� */
uint8_t Power_VP = 0;           /* �ܹ��ʱ��� */
uint8_t Power_VF = 0;           /* ���书�ʱ��� */
uint8_t Power_ZB = 0;           /* פ������ */

uint8_t ErrorFlag = 0;         /* ��0�����쳣   0xff�����Ѿ�������쳣*/

uint8_t PowerFlag[2] = {0,0};  /* �쳣λ��ʾ���� */



uint8_t Alarm_Curr_Value = 185 ;  // ���
uint8_t Alarm_Vswr_Value = 50;    // ���
uint16_t Alarm_Pout_Value = 1400; // ���
uint16_t Alarm_Temp_Value = 700;  // ���





__IO uint16_t ADC_AverageValue[7] = {0};	      /* ��β�����ȡƽ��ֵ */

__IO uint16_t Current[6] = {0};                /* ��β�����ȡƽ��ֵ */

uint16_t  VOLT_DEC = 0;

float  Pout = 0.0f;
float  Pfout = 0.0f;
float  ZhuBo = 1.0f;




//The Power Down Protect GaN Timing  Control
void Timing_Protect_Control(void)
{
	if (PPT && VOLT_DEC < 450)
    {
		PPT_SW(OFF);
	}
}



void CURR_calculate(void)
{
    uint8_t i = 0;
    float date = 0.0f, num1 = 0.0f, num2 = 0.0f;
    static uint8_t t = 0;
    static uint16_t max[9] = {0}, min[9] = {0}, sum[9] = {0};

    if(t == 0)
    {
        for(i=0; i<7; i++)
        {
            sum[i] = 0;
            max[i] = ADC_ConvertedValue[i];
            min[i] = ADC_ConvertedValue[i];
        }
    }
    for(i = 0; i < 7; i++)
    {
        sum[i] += ADC_ConvertedValue[i];
        if(max[i] <= ADC_ConvertedValue[i]) 	  /* �������ֵ */
            max[i] = ADC_ConvertedValue[i];

        if(min[i] >= ADC_ConvertedValue[i]) 	  /* ������Сֵ */
            min[i] = ADC_ConvertedValue[i];
    }
    t++;
    if(t == 10)
    {
        t = 0;

        for(i = 0; i < 9; i++)
        {
            sum[i] -= min[i] + max[i];               /* ��ȥ���ֵ����Сֵ */
            ADC_AverageValue[i] = sum[i] >> 3;		   /* ����10�γ�ȥ���ֵ��Сֵ������8 */
        }

        date = (float)ADC_AverageValue[0] * 3.29f/4096;
        VOLT_DEC = date  * 93.15f * 1.835f;

        for(i = 1; i < 5; i++)//��Ӧ����˳��
        {
            date = (float)ADC_AverageValue[i] * 82;
            Current[i-1] = date *3.29f/4096;
        }

        if(ADC_AverageValue[5]>100)		num1 = ADC_AverageValue[5]-100;
        else                            num1 = 0;

        if(ADC_AverageValue[6]>150)		num2 = ADC_AverageValue[6]-150;
        else                            num2 = 0;

//        if(num1 < num2) num2 = num1-1;

        Pout = num1*num1/K_value;
        Pfout= num2*num2/K_value;

        if((Pfout<Pout) && (Pout > 0))
        {           
            ZhuBo = (float)( 1 + sqrt(Pfout/Pout))/( 1 - sqrt(Pfout/Pout));
        }
        
        if(Pfout  > Pout) 
        {
             ZhuBo = 9.99;
        }
        
        if((Pfout  == Pout) && (Pout == 0))
        {
            ZhuBo = 1;
        }
                           
        if(ZhuBo > 9.99f) ZhuBo = 9.99f;
        ZhuBo*=10;
    }
}







void set_data(uint8_t *paTxbuff)
{
    uint8_t i = 0;

    paTxbuff[0] = PowerFlag[0];
    paTxbuff[1] = PowerFlag[1];              //�쳣״̬
    paTxbuff[2] = VOLT_DEC >> 8;
    paTxbuff[3] = VOLT_DEC;		              //���ŵ�ѹ
    paTxbuff[4] = g_usBoardTemp >> 8;
    paTxbuff[5] = g_usBoardTemp;		                  //�����¶�
    for(i=0; i<6; i++)
    {
        paTxbuff[6+2*i] = Current[i] >> 8;
        paTxbuff[7+2*i] = Current[i];
    }

    paTxbuff[18] = (uint16_t)Pout >>8;              //�������
    paTxbuff[19] = (uint16_t)Pout & 0xFF ;

    paTxbuff[20] = (uint16_t)Pfout >>8;             //���书��
    paTxbuff[21] = (uint16_t)Pfout & 0xFF;


    paTxbuff[22] = (uint16_t)ZhuBo >>8;             //פ����
    paTxbuff[23] = (uint16_t)ZhuBo & 0xFF;

    paTxbuff[24] = K_value >>8;
    paTxbuff[25] = K_value;
}





void error_serve(void)
{
    uint8_t i = 0;
    static uint16_t num[10] = {0,0,0,0,0,0,0,0,0,0};
    static uint16_t num2[2] = {0,0};                                        /* ���ʣ�פ�� */

    if(Power)
    {
        if((VOLT_DEC >= 550 || VOLT_DEC <= 0) && Power_V)                       //���ŵ�ѹ24��32V
        {
            num[0]++;
            if(num[0]>=100)
            {
                num[0]=0;
                PowerFlag[0] = 0;
                PowerFlag[1] = 0x01;
            }
        } else num[0] = 0;

        if(g_usBoardTemp >= Alarm_Temp_Value && Power_T)                                    /* �¶ȳ���60�� */
        {
            num[1]++;
            if(num[1]>=5000)
            {
                num[1]=0;
                //LED_ON(TEMP_LED);
                PowerFlag[0] = 0x00;
                PowerFlag[1] = 0x02;
            }
        } else num[1] = 0;

        if(Power_I)
        {
            for(i=0; i<6; i++)
            {
                if(Current[i] > Alarm_Curr_Value)       /* ����18.5A */
                {
                    num[2+i]++;
                    if(num[2+i]>=50)
                    {
                        num[2+i]=0;
                        //LED_ON(CURR_LED);
                        PowerFlag[0] = 0;
                        PowerFlag[1] = 0x01<<(2+i);
                    }
                } else num[2+i] = 0;
            }
        }
        if(Pout>Alarm_Pout_Value && Power_VP)
        {
            num2[0]++;
            if(num2[0]>=500)
            {
                num2[0]=0;
                PowerFlag[0] = 0x01;
                PowerFlag[1] = 0;
            }
        } else num2[0]=0;

        if(ZhuBo > Alarm_Vswr_Value && Power_ZB && Pout > 100 && Pfout>100)
        {
            num2[1]++;
            if(num2[1]>=500)
            {
                num2[1]=0;
                //LED_ON(VSWR_LED);
                PowerFlag[0] = 0x04;
                PowerFlag[1] = 0;
            }
        } else num2[1]=0;
    }
}







