#include "system.h"


int main(void)
{
    systick_init(72); /*��ʼ��Systick����ʱ��*/

    PPT_SW_init();
    LED_Init();
    ADC1_Init(); /* ADC���� */
    AT24CXX_Init();
    DS18B20_Init();
    Addr_Init();
    
    w5500_init();                    /*��ʼ��ͨ�ţ����������ַ*/
    socket_buf_init(txsize, rxsize); /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

    while (1) 
    {
        POWER_calculate();
        do_udp();           /*UDP ���ݻػ�����*/
        Get_Temp();
    }
}


