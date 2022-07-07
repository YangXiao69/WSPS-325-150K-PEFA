#include "tcp.h"
#include "../../W5500/w5500_conf.h"
#include "task.h"
#include "../Core/Inc/bsp_i2c_ee.h"

uint8_t RJ45_Rx_buff[128];
uint8_t RJ45_Tx_buff[128];
uint8_t RestFlag;
void work_RJ45send_buff(uint8_t sadr,uint16_t addr, uint8_t len, uint8_t way);


void Task_TCP_TOP(void)
{
    uint8_t status = 0;
    uint16_t len = 0;

    status = IINCHIP_READ_TOP(Sn_SR(0));

    switch (status) /*获取socket的状态*/
    {
    case SOCK_CLOSED:                                    /*socket处于关闭状态*/
        socket_top(0, Sn_MR_UDP, local_port2, 0); /*打开socket*/
        break;


    case SOCK_UDP: /*socket处于连接建立状态*/
        if (IINCHIP_READ_TOP(Sn_IR(0)) & Sn_IR_RECV)
        {
            IINCHIP_WRITE_TOP(Sn_IR(0), Sn_IR_RECV); /*清除接收中断标志位*/
        }
        
        len = getSn_RX_RSR_TOP(0); /*定义len为已接收数据的长度*/
        if (len > 0)
        {
            recvfrom_top(0, RJ45_Rx_buff, len, remote_ip2, &remote_port2); /*W5500接收计算机发送来的数据*/
            remote_RJ45_reseive(0);
        }

        break;

    case SOCK_CLOSE_WAIT: /*socket处于等待关闭状态*/
        close_TOP(0);
        break;
    }



}


void remote_RJ45_reseive(uint8_t sa)
{
    uint16_t Data = 0, ADDR = 0,i,mNum,mData;
    
    if (RJ45_Rx_buff[6] == System.Modbus_Addr || RJ45_Rx_buff[6] == 0x00)
    {
        if (RJ45_Rx_buff[7] == 0x06) /* 写指令 */
        {
            ADDR = ((uint16_t)RJ45_Rx_buff[8] << 8) + RJ45_Rx_buff[9];   //访问地址
            Data = ((uint16_t)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11]; //写数据 或者 读取长度
            Modbus_Progress(ADDR,Data);
            work_RJ45send_buff(sa,ADDR, 1, 0x06);
        }
        else if (RJ45_Rx_buff[7] == 0x03)
        {
            ADDR = ((uint16_t)RJ45_Rx_buff[8] << 8) + RJ45_Rx_buff[9];   //访问地址
            Data = ((uint16_t)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11]; //写数据 或者 读取长度
            work_RJ45send_buff(sa,ADDR, Data, 0x03);
        }
        else if (RJ45_Rx_buff[7] == 0x10) //连续写
        {
            ADDR = ((uint16_t)RJ45_Rx_buff[8] << 8) + RJ45_Rx_buff[9];
            mNum = ((uint16_t)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11];
            for(i=0;i<mNum;i++)
            {
                mData = ((uint16_t)RJ45_Rx_buff[13+i*2] << 8) + RJ45_Rx_buff[14+i*2];
                Modbus_Progress(ADDR,mData);
            }
            work_RJ45send_buff(sa,ADDR, RJ45_Rx_buff[12], 0x10);
        }
    }
}


void Modbus_Progress(uint16_t Addr,uint16_t Data)
{
    uint8_t buffer[2];
    uint8_t iap_buf = 0x06;
    buffer[0] = Data >> 8;
    buffer[1] = Data;
    
    switch (Addr)
    {
        case 0x0080:
            System.Power = Data; /* 电源 */
            break;

        case 0x0081:
            System.RF = Data; /* 射频 */
            break;

        case 0x0082:
            System.LED_Alarm_Power = Data; /* 告警功率 */
            break;

        case 0x0083:
            System.LED_Alarm_Curr = Data; /* 告警电流 */
            break;

        case 0x0084:
            System.LED_Alarm_Temp = Data; /* 告警温度 */
            break;        
        
//    case 0x00e3: /*远程升级*/
//        if (Data == 0x3274)
//        {
//            FLASH_If_Erase(ADDRESS_START_FLAG, ADDRESS_END_FLAG);
//            send_TOP(0,&iap_buf,1);
//            NVIC_SystemReset();
//        }
//        break;       
        
    default:
        break;
    }    
}

void work_RJ45send_buff(uint8_t sadr,uint16_t addr, uint8_t len, uint8_t way)
{
    uint16_t i = 0;
    
    if (len > 120 || addr > 1024)
    {
        return;
    }
    
    RJ45_Tx_buff[0] = RJ45_Rx_buff[0];
    RJ45_Tx_buff[1] = RJ45_Rx_buff[1];
    RJ45_Tx_buff[2] = RJ45_Rx_buff[2];
    RJ45_Tx_buff[3] = RJ45_Rx_buff[3];

    RJ45_Tx_buff[6] = System.Modbus_Addr;
    RJ45_Tx_buff[7] = way;

    if (way == 3) //读
    {
        RJ45_Tx_buff[4] = 0x00;
        RJ45_Tx_buff[5] = len * 2 + 3;
        RJ45_Tx_buff[8] = len * 2;
        for (i = 0; i < len; i++)
        {
            RJ45_Tx_buff[2 * i + 9] = Modbus[i + addr] >> 8;
            RJ45_Tx_buff[2 * i + 10] = Modbus[i + addr];
        }
        
        sendto_top(sadr, RJ45_Tx_buff, len * 2 + 9, remote_ip2, remote_port2);
    }
    else if (way == 6) //写
    {
        for (i = 0; i < 12; i++)
        {
            RJ45_Tx_buff[i] = RJ45_Rx_buff[i];
        }

        sendto_top(sadr, RJ45_Tx_buff, 12, remote_ip2, remote_port2);
    }
    else if (way == 16) //写
    {
        RJ45_Tx_buff[4] = 0x00;
        RJ45_Tx_buff[5] = 6;
        RJ45_Tx_buff[8] = addr >> 8;
        RJ45_Tx_buff[9] = addr;
        RJ45_Tx_buff[11] = len;

        sendto_top(sadr, RJ45_Tx_buff, 12, remote_ip2, remote_port2);
    }
}






