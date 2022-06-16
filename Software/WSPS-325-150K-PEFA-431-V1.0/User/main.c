#include "system.h"


int main(void)
{
    systick_init(72); /*初始化Systick工作时钟*/

    PPT_SW_init();
    LED_Init();
    ADC1_Init(); /* ADC配置 */
    AT24CXX_Init();
    DS18B20_Init();
    Addr_Init();
    
    w5500_init();                    /*初始化通信，包含插件地址*/
    socket_buf_init(txsize, rxsize); /*初始化8个Socket的发送接收缓存大小*/

    while (1) 
    {
        POWER_calculate();
        do_udp();           /*UDP 数据回环测试*/
        Get_Temp();
    }
}


