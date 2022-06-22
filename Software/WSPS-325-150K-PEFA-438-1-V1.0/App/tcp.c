#include "tcp.h"
#include "../../W5500/w5500_conf.h"
#include "task.h"
#include "../Core/Inc/bsp_i2c_ee.h"

uint8_t RJ45_Rx_buff[128];
uint8_t RJ45_Tx_buff[128];
uint8_t RestFlag;
void work_RJ45send_buff(uint8_t sadr,uint16_t addr, uint8_t len, uint8_t way);


void Task_TCP(void)
{
    static uint32_t Keptime = 0;
    uint8_t status = 0;
    uint16_t len = 0;

    if(RestFlag == 2)
    {
        RestFlag = 0;
        close_TOP(0);
        close_TOP(1);
        mac2[5] = local_ip2[3];

        wiz_write_buf_top(SHAR0, mac2, 6); /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/
        wiz_write_buf_top(SUBR0, subnet2, 4);
        wiz_write_buf_top(GAR0, gateway2, 4);
        wiz_write_buf_top(SIPR0, local_ip2, 4);
    }

    status = IINCHIP_READ_TOP(Sn_SR(0));

    switch (status) /*��ȡsocket��״̬*/
    {
    case SOCK_CLOSED:                                    /*socket���ڹر�״̬*/
        socket_top(0, Sn_MR_TCP, local_port2, Sn_MR_ND); /*��socket*/
        break;

    case SOCK_INIT:    /*socket�ѳ�ʼ��״̬*/
        listen_TOP(0); /*socket��������*/
        break;

    case SOCK_ESTABLISHED: /*socket�������ӽ���״̬*/
        if (IINCHIP_READ_TOP(Sn_IR(0)) & Sn_IR_CON)
        {
            IINCHIP_WRITE_TOP(Sn_IR(0), Sn_IR_CON); /*��������жϱ�־λ*/
        }
        len = getSn_RX_RSR_TOP(0); /*����lenΪ�ѽ������ݵĳ���*/
        if (len > 0)
        {
            recv_TOP(0, RJ45_Rx_buff, len);                                /*��������Server������*/
            recvfrom_top(0, RJ45_Rx_buff, len, remote_ip2, &remote_port2); /*W5500���ռ����������������*/

            remote_RJ45_reseive(0);
            Keptime = 0;
        }
        else
        {
            Keptime++;
            if(Keptime == 20000)
            {
                close_TOP(0);  
                Keptime = 0;            
            }
        }
        break;

    case SOCK_CLOSE_WAIT: /*socket���ڵȴ��ر�״̬*/
        close_TOP(0);
        break;
    }



}


void remote_RJ45_reseive(uint8_t sa)
{
    uint16_t Data = 0, ADDR = 0,i,mNum,mData;
    
    if (RJ45_Rx_buff[6] == System.Modbus_Addr || RJ45_Rx_buff[6] == 0x00)
    {
        if (RJ45_Rx_buff[7] == 0x06) /* дָ�� */
        {
            ADDR = ((uint16_t)RJ45_Rx_buff[8] << 8) + RJ45_Rx_buff[9];   //���ʵ�ַ
            Data = ((uint16_t)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11]; //д���� ���� ��ȡ����
            Modbus_Progress(ADDR,Data);
            work_RJ45send_buff(sa,ADDR, 1, 0x06);
        }
        else if (RJ45_Rx_buff[7] == 0x03)
        {
            ADDR = ((uint16_t)RJ45_Rx_buff[8] << 8) + RJ45_Rx_buff[9];   //���ʵ�ַ
            Data = ((uint16_t)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11]; //д���� ���� ��ȡ����
            work_RJ45send_buff(sa,ADDR, Data, 0x03);
        }
        else if (RJ45_Rx_buff[7] == 0x10) //����д
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
    case 0x0011:
        if(System.Modbus_Addr == Data)
        return;
        
        System.Modbus_Addr = Data;
        I2C_EE_ByteWrite(&buffer[1], 0);
        delay_ms(5);
        break;    
    
    case 0x0080:
        System.Power = Data; /* ��Դ */
        break;

    case 0x0081:
        System.RF = Data; /* ��Ƶ */
        break;

    /******************************************����Դ�澯��У׼��������**********************************************/

    case 0x00E2:
        if(System.fPout_factor == Data)
            return;
        System.fPout_factor = Data; /* ���ؼ첨΢��׼ */
        I2C_EE_ByteWrite(&buffer[0], 37);
        delay_ms(5);
        I2C_EE_ByteWrite(&buffer[1], 38);
        delay_ms(5);
        break;

    case 0x00E0:
        if(System.Pout_factor[System.Mode] == Data)
            return;
        System.Pout_factor[System.Mode] = Data; /* ���ؼ첨΢��׼ */
        if(System.Mode == 0)
        {
            I2C_EE_ByteWrite(&buffer[0], 39);
            delay_ms(5);
            I2C_EE_ByteWrite(&buffer[1], 40);
        }
        else
        {
            I2C_EE_ByteWrite(&buffer[0], 49);
            delay_ms(5);
            I2C_EE_ByteWrite(&buffer[1], 50);  
        }
        break;
    
    case 0x00E1:
        if(System.PRout_factor == Data)
            return;            
    
        System.PRout_factor = Data; /* ���ؼ첨΢��׼ ���书�� */
        I2C_EE_ByteWrite(&buffer[0], 41);
        delay_ms(5);
        I2C_EE_ByteWrite(&buffer[1], 42);
        delay_ms(5);
        break;

//    case 0x00e3: /*Զ������*/
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

    if (way == 3) //��
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
    else if (way == 6) //д
    {
        for (i = 0; i < 12; i++)
        {
            RJ45_Tx_buff[i] = RJ45_Rx_buff[i];
        }

        sendto_top(sadr, RJ45_Tx_buff, 12, remote_ip2, remote_port2);
    }
    else if (way == 16) //д
    {
        RJ45_Tx_buff[4] = 0x00;
        RJ45_Tx_buff[5] = 6;
        RJ45_Tx_buff[8] = addr >> 8;
        RJ45_Tx_buff[9] = addr;
        RJ45_Tx_buff[11] = len;

        sendto_top(sadr, RJ45_Tx_buff, 12, remote_ip2, remote_port2);
    }
}






