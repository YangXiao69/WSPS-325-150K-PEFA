#include "udp.h"
#include "../../W5500/w5500_conf.h"
#include "task.h"
#include "led.h"

uint16_t dataflag[25];
uint8_t UDP_Tx_buff[128];
uint8_t UDP_Rx_buff[128];
uint8_t Reseve_IP[4] = {0};

void UDP_Revice(uint8_t addr,uint8_t *buffer);
void Modbus_Data_Clean(uint8_t addr);



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
        if (dataflag[24] >= 1000)
        {
            dataflag[24] = 0;
            UDP_Tx_buff[0] = System.RF;
            UDP_Tx_buff[13] = System.LED_Alarm_Power;
            UDP_Tx_buff[14] = System.LED_Alarm_Curr;
            UDP_Tx_buff[15] = System.LED_Alarm_Temp;
            
            sendto(0, UDP_Tx_buff, 16, remote_ip, remote_port);
            LED_Comm_Toggle();
        }   
        
        len = getSn_RX_RSR(0);
        if (len > 0) /*接收到数据*/
        {
            recvfrom(0, UDP_Rx_buff, len, Reseve_IP, &remote_port); /*W5500接收计算机发送来的数据*/
            
            if(Reseve_IP[3] > 113 || len >100)
                return;
 
            UDP_Revice(Reseve_IP[3] - 101, UDP_Rx_buff);
            dataflag[Reseve_IP[3] - 101] = 0;            /* 收到数据，清除未收到数据标志位 */
        }
        else /* 没有接收到数据 */
        {
            for (n = 0; n < 16; n++)
            {
                dataflag[n]++;
                if (dataflag[n] >= 2000)
                {
                    Modbus_Data_Clean(n);
                    dataflag[n] = 0;
                }
            }
        }
        break;
    }


}


void UDP_Revice(uint8_t addr,uint8_t *buffer)       /*00 01 02 03 04 05 06 07 */
{
    if(addr >= 12)
    {
        return;
    }
    AmplifierModule[addr].Voltage = (uint16_t)(buffer[11]<<8)+buffer[12];
    AmplifierModule[addr].Powerout = (uint16_t)(buffer[15]<<8)+buffer[16];
    AmplifierModule[addr].PowerR = (uint16_t)(buffer[17]<<8)+buffer[18];
    AmplifierModule[addr].Current[0] = (uint16_t)(buffer[19]<<8)+buffer[20];
    AmplifierModule[addr].Current[1] = (uint16_t)(buffer[21]<<8)+buffer[22];
    AmplifierModule[addr].Current[2] = (uint16_t)(buffer[23]<<8)+buffer[24];
    AmplifierModule[addr].Current[3] = (uint16_t)(buffer[25]<<8)+buffer[26];
    AmplifierModule[addr].Temp = (uint16_t)(buffer[27]<<8)+buffer[28];
}


void Modbus_Data_Clean(uint8_t addr)
{
    if(addr >= 12)
    {
        return;
    }
    AmplifierModule[addr].Voltage = 0;
    AmplifierModule[addr].Powerout = 0;
    AmplifierModule[addr].PowerR = 0;
    AmplifierModule[addr].Current[0] = 0;
    AmplifierModule[addr].Current[1] = 0;
    AmplifierModule[addr].Current[2] = 0;
    AmplifierModule[addr].Current[3] = 0;
    AmplifierModule[addr].Temp = 0;


}




