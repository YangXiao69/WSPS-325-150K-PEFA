#include "udp.h"
#include "../../W5500/w5500_conf.h"
#include "task.h"
#include "led.h"

uint16_t dataflag[25];
uint8_t UDP_Tx_buff[128];
uint8_t UDP_Rx_buff[128];
uint8_t Reseve_IP[4] = {0};



void Task_UDP(void)
{
    uint8_t status = 0;
    uint16_t n = 0;
    static uint16_t len;
        
    status = IINCHIP_READ(Sn_SR(0));
    switch (status) /*获取socket的状态*/
    {
    case SOCK_CLOSED:                        /*socket处于关闭状态*/
        socket(0, Sn_MR_UDP, local_port, 0); /*初始化socket*/
        break;

    case SOCK_UDP: /*socket初始化完成*/
        if (IINCHIP_READ(Sn_IR(0)) & Sn_IR_RECV)
            IINCHIP_WRITE(Sn_IR(0), Sn_IR_RECV); /*清接收中断*/
        
        dataflag[24]++;
        if (dataflag[24] >= 600)
        {
            dataflag[24] = 0;
            UDP_Tx_buff[0] = System.Power;
            UDP_Tx_buff[1] = System.RF;

            sendto(0, UDP_Tx_buff, 3, remote_ip, remote_port);
            LED_Comm_Toggle();
        }   
        
        len = getSn_RX_RSR(0);
        if (len > 0) /*接收到数据*/
        {
            System.UDP_Ready = 1;
            recvfrom(0, UDP_Rx_buff, len, Reseve_IP, &remote_port); /*W5500接收计算机发送来的数据*/
            
            if(Reseve_IP[3] > 116 || len >100)
                return;
            
//            UDP_Revice(Reseve_IP[3] - 101, UDP_Rx_buff);
            dataflag[Reseve_IP[3] - 101] = 0;            /* 收到数据，清除未收到数据标志位 */
        }
        else /* 没有接收到数据 */
        {
            for (n = 0; n < 16; n++)
            {
                dataflag[n]++;
                if (dataflag[n] >= 2000)
                {
//                    Modbus_Data_Clean(n);
                    dataflag[n] = 0;
                }
            }
        }
        break;
    }


}




void UDP_Revice(uint8_t addr,uint8_t *buffer)
{
    if(addr >= 16)
    {
        return;
    }
    AmplifierModule[addr].error = (uint16_t)(buffer[0]<<8)+buffer[1];
    AmplifierModule[addr].Voltage = (uint16_t)(buffer[2]<<8)+buffer[3];
    AmplifierModule[addr].Current = (uint16_t)(buffer[8]<<8)+buffer[9];
    AmplifierModule[addr].Temp[0] = (uint16_t)(buffer[4]<<8)+buffer[5];
    AmplifierModule[addr].Temp[1] = (uint16_t)(buffer[6]<<8)+buffer[7];
}







