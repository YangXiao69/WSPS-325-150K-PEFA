#include "main.h"




void do_udp(void)
{
    uint16_t len=0;
    static uint16_t time=0;
    
    uint8_t Rx_buff[96] = {0};                                                           /*定义一个2KB的缓存*/
    uint8_t Tx_buff[96] = {0};    
    
    
    //static u16 time2=0;
    switch(getSn_SR(0))                                                 /*获取socket的状态*/
    {
    case SOCK_CLOSED:                                                 /*socket处于关闭状态*/
        socket(0,Sn_MR_UDP,local_port,0);                            /*初始化socket*/
        break;

    case SOCK_UDP:                                                    /* socket初始化完成*/
        if(getSn_IR(0) & Sn_IR_RECV)
            setSn_IR(0, Sn_IR_RECV);                                 /*清接收中断*/

        time++;

        if(PowerFlag[0]||PowerFlag[1])
        {
            if(time >= 40)
            {
                time = 0;
                set_data(Tx_buff);
                sendto(0,Tx_buff,24, remote_ip, remote_port);
            }
        }
        if(time >= 400)                                                /* 400ms */
        {
            time = 0;
            set_data(Tx_buff);
            sendto(0,Tx_buff,26, remote_ip, remote_port);
        }

        if((len=getSn_RX_RSR(0))>0)                                    /*接收到数据*/
        {
            recvfrom(0,Rx_buff, len, remote_ip,&remote_port);          /*W5500接收计算机发送来的数据*/

            if(Rx_buff[0] == 0xFF)
            {
                if(Rx_buff[1] == NumName)//校准功率
                {
                    K_value = (Rx_buff[2] << 8) + Rx_buff[3];
                    I2C_EE_ByteWrite(&Rx_buff[2],252);
                    I2C_EE_ByteWrite(&Rx_buff[3],253);
                    
                }
                if(Rx_buff[1] == 0xEE)//清除异常标志位
                {
                    PowerFlag[0] = 0x00;
                    PowerFlag[1] = 0x00;
                }
                if(Rx_buff[1] == 0xAA)//重新启动
                {
//                    delay_ms(200);
//                    NVIC_SeytemReset();
                }
            }
            else
            {
                Power = Rx_buff[0];
                if(Rx_buff[1] == 0x01) {
                    PPT_SW(ON);
                }//{	PPT_SW(ON);  LED_ON(RFOUT_LED);  }
                if(Rx_buff[1] == 0x00) {
                    PPT_SW(OFF);
                }//{	PPT_SW(OFF); LED_OFF(RFOUT_LED); }
                Power_V  = Rx_buff[2];
                Power_T  = Rx_buff[3];
                Power_I  = Rx_buff[4];
                Power_VP = Rx_buff[5];
                Power_VF = Rx_buff[6];
                Power_ZB = Rx_buff[7];

                Alarm_Temp_Value = ((uint16_t)Rx_buff[8]<<8) + Rx_buff[9];
                Alarm_Curr_Value  = Rx_buff[10];
                Alarm_Vswr_Value  = Rx_buff[11];
                Alarm_Pout_Value = ((uint16_t)Rx_buff[12]<<8) + Rx_buff[13];
            }
            //LED_COML();
        }
        //if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9))	time2++;
        //else time2=0;
        //if(time2>=250){ time2=0;LED_OFF(COMM_LED);}

        break;
    }
}

