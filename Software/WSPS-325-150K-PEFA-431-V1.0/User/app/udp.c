#include "udp.h"
#include "system.h"


uint8_t PowerFlag = 0;  /* 电源电压告警 */
uint8 Rx_buff[32]; /*定义一个2KB的缓存*/
uint8 Tx_buff[40];

void UDP_Receive(void);
void set_data(void);

void do_udp(void)
{
    uint16 len = 0;
    static uint16_t time = 0;
    switch (getSn_SR(0)) /*获取socket的状态*/
    {
        case SOCK_CLOSED:                        /*socket处于关闭状态*/
            socket(0, Sn_MR_UDP, local_port, 0); /*初始化socket*/
            break;

        case SOCK_UDP: /* socket初始化完成*/
            if (getSn_IR(0) & Sn_IR_RECV)
                setSn_IR(0, Sn_IR_RECV); /*清接收中断*/

            time++;
            if (time >= 2000) /* 250 * 2ms = 500ms */
            {
                time = 0;
                set_data();
                sendto(0, Tx_buff, 20, remote_ip, remote_port);
                LED_COML();
            }

            if ((len = getSn_RX_RSR(0)) > 0) /*接收到数据*/
            {
                recvfrom(0, Rx_buff, len, remote_ip, &remote_port); /*W5500接收计算机发送来的数据*/
                UDP_Receive();

            }
            break;
    }
}

void UDP_Receive(void)
{
    if (Rx_buff[0] != 0xFF)     /*01 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 0f 00 00 00 */
    {
        System.RF = Rx_buff[0];
        if (Rx_buff[0] == 0x01)
            LED_ON(RFOUT_LED);
        else
            LED_OFF(RFOUT_LED);

        if (Rx_buff[System.Addr] & 0x08)
            GPIO_SetBits(GPIOA, GPIO_Pin_9); //模块4
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);

        if (Rx_buff[System.Addr] & 0x04)
            GPIO_SetBits(GPIOA, GPIO_Pin_10); //模块3
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_10);

        if (Rx_buff[System.Addr] & 0x02)
            GPIO_SetBits(GPIOA, GPIO_Pin_11); //模块2
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);

        if (Rx_buff[System.Addr] & 0x01)
            GPIO_SetBits(GPIOA, GPIO_Pin_12); //模块1
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        
        if(Rx_buff[13] == System.Addr)      /*功率*/
            LED_ON(VSWR_LED);
        else
            LED_OFF(VSWR_LED);            
        
        if(Rx_buff[14] == System.Addr)      /*电流*/
            LED_ON(CURR_LED);
        else
            LED_OFF(CURR_LED);     
        
        if(Rx_buff[15] == System.Addr)      /*温度*/
            LED_ON(TEMP_LED);
        else
            LED_OFF(TEMP_LED);     
    }
}


void set_data(void)
{
    uint16_t AlarmFlag;


    Tx_buff[0] = AlarmFlag >> 8;
    Tx_buff[1] = AlarmFlag;
    Tx_buff[2] = System.Voltage >> 8;
    Tx_buff[3] = System.Voltage; //功放电压

    Tx_buff[4] = System.Pin>> 8;
    Tx_buff[5] = System.Pin; //输入功率
    Tx_buff[6] = System.Pout >> 8;
    Tx_buff[7] = System.Pout; //输出功率
    Tx_buff[8] = System.Pfout >> 8;
    Tx_buff[9] = System.Pfout; //反射功率

    Tx_buff[10] = System.Current[0] >> 8;
    Tx_buff[11] = System.Current[0]; //电流
    Tx_buff[12] = System.Current[1] >> 8;
    Tx_buff[13] = System.Current[1];
    Tx_buff[14] = System.Current[2] >> 8;
    Tx_buff[15] = System.Current[2];
    Tx_buff[16] = System.Current[3] >> 8;
    Tx_buff[17] = System.Current[3];

    Tx_buff[18] = System.Temp >> 8;
    Tx_buff[19] = System.Temp; //功放温度
}
