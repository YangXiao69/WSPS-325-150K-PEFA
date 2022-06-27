#include "udp.h"
#include "system.h"


uint8_t PowerFlag = 0;  /* ��Դ��ѹ�澯 */
uint8 Rx_buff[32]; /*����һ��2KB�Ļ���*/
uint8 Tx_buff[40];

void UDP_Receive(void);
void set_data(void);

void do_udp(void)
{
    uint16 len = 0;
    static uint16_t time = 0;
    switch (getSn_SR(0)) /*��ȡsocket��״̬*/
    {
        case SOCK_CLOSED:                        /*socket���ڹر�״̬*/
            socket(0, Sn_MR_UDP, local_port, 0); /*��ʼ��socket*/
            break;

        case SOCK_UDP: /* socket��ʼ�����*/
            if (getSn_IR(0) & Sn_IR_RECV)
                setSn_IR(0, Sn_IR_RECV); /*������ж�*/

            time++;
            if (time >= 2000) /* 250 * 2ms = 500ms */
            {
                time = 0;
                set_data();
                sendto(0, Tx_buff, 20, remote_ip, remote_port);
                LED_COML();
            }

            if ((len = getSn_RX_RSR(0)) > 0) /*���յ�����*/
            {
                recvfrom(0, Rx_buff, len, remote_ip, &remote_port); /*W5500���ռ����������������*/
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
            GPIO_SetBits(GPIOA, GPIO_Pin_9); //ģ��4
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);

        if (Rx_buff[System.Addr] & 0x04)
            GPIO_SetBits(GPIOA, GPIO_Pin_10); //ģ��3
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_10);

        if (Rx_buff[System.Addr] & 0x02)
            GPIO_SetBits(GPIOA, GPIO_Pin_11); //ģ��2
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);

        if (Rx_buff[System.Addr] & 0x01)
            GPIO_SetBits(GPIOA, GPIO_Pin_12); //ģ��1
        else
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        
        if(Rx_buff[13] == System.Addr)      /*����*/
            LED_ON(VSWR_LED);
        else
            LED_OFF(VSWR_LED);            
        
        if(Rx_buff[14] == System.Addr)      /*����*/
            LED_ON(CURR_LED);
        else
            LED_OFF(CURR_LED);     
        
        if(Rx_buff[15] == System.Addr)      /*�¶�*/
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
    Tx_buff[3] = System.Voltage; //���ŵ�ѹ

    Tx_buff[4] = System.Pin>> 8;
    Tx_buff[5] = System.Pin; //���빦��
    Tx_buff[6] = System.Pout >> 8;
    Tx_buff[7] = System.Pout; //�������
    Tx_buff[8] = System.Pfout >> 8;
    Tx_buff[9] = System.Pfout; //���书��

    Tx_buff[10] = System.Current[0] >> 8;
    Tx_buff[11] = System.Current[0]; //����
    Tx_buff[12] = System.Current[1] >> 8;
    Tx_buff[13] = System.Current[1];
    Tx_buff[14] = System.Current[2] >> 8;
    Tx_buff[15] = System.Current[2];
    Tx_buff[16] = System.Current[3] >> 8;
    Tx_buff[17] = System.Current[3];

    Tx_buff[18] = System.Temp >> 8;
    Tx_buff[19] = System.Temp; //�����¶�
}
