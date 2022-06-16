#include "udp.h"
#include "system.h"


uint8_t PowerFlag = 0;  /* ��Դ��ѹ�澯 */
uint8 Rx_buff[32]; /*����һ��2KB�Ļ���*/
uint8 Tx_buff[40];


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
            if (time >= 800) /* 250 * 2ms = 500ms */
            {
                time = 0;
                set_data();
                sendto(0, Tx_buff, 39, remote_ip, remote_port);
                LED_COML();
            }

            if ((len = getSn_RX_RSR(0)) > 0) /*���յ�����*/
            {
                recvfrom(0, Rx_buff, len, remote_ip, &remote_port); /*W5500���ռ����������������*/

                if (Rx_buff[0] != 0xFF)
                {
                    if (Rx_buff[0] == 0x01)
                        LED_ON(RFOUT_LED);
                    if (Rx_buff[0] == 0x00)
                        LED_OFF(RFOUT_LED);

                    if (Rx_buff[System.Addr] & 0x08)
                        GPIO_SetBits(GPIOB, GPIO_Pin_5); //ģ��4
                    else
                        GPIO_ResetBits(GPIOB, GPIO_Pin_5);

                    if (Rx_buff[System.Addr] & 0x04)
                        GPIO_SetBits(GPIOB, GPIO_Pin_8); //ģ��3
                    else
                        GPIO_ResetBits(GPIOB, GPIO_Pin_8);

                    if (Rx_buff[System.Addr] & 0x02)
                        GPIO_SetBits(GPIOB, GPIO_Pin_9); //ģ��2
                    else
                        GPIO_ResetBits(GPIOB, GPIO_Pin_9);

                    if (Rx_buff[System.Addr] & 0x01)
                        GPIO_SetBits(GPIOE, GPIO_Pin_0); //ģ��1
                    else
                        GPIO_ResetBits(GPIOE, GPIO_Pin_0);
                }
                else
                {
                    if (Rx_buff[1] == System.Addr)
                    {
                        if (Rx_buff[2] == 1) //У׼�������
                        {
                            System.K_value = ((uint16_t)Rx_buff[3] << 8) + Rx_buff[4];
                            AT24CXX_WriteLenByte(0, System.K_value, 2);
                        }
                        else if (Rx_buff[2] == 2) //У׼���빦��
                        {
                            System.K_value2 = ((uint16_t)Rx_buff[3] << 8) + Rx_buff[4];
                            AT24CXX_WriteLenByte(2, System.K_value2, 2);
                        }
                        else if (Rx_buff[2] == 3) //У׼���书��
                        {
                            System.K_value3 = ((uint16_t)Rx_buff[3] << 8) + Rx_buff[4];
                            AT24CXX_WriteLenByte(4, System.K_value3, 2);
                        }
                    }
                }
            }
            break;
    }
}


void set_data(void)
{
    uint16_t CurrNum;
    CurrNum = System.Current[0] + System.Current[1] + System.Current[2] + System.Current[3];

    Tx_buff[0] = PowerFlag;
//    Tx_buff[1] = AlarmFlag >> 8;
//    Tx_buff[2] = AlarmFlag;
    Tx_buff[3] = System.Voltage >> 8;
    Tx_buff[4] = System.Voltage; //���ŵ�ѹ

    Tx_buff[5] = System.Pin>> 8;
    Tx_buff[6] = System.Pin; //���빦��
    Tx_buff[7] = System.Pout >> 8;
    Tx_buff[8] = System.Pout; //�������
    Tx_buff[9] = System.Pfout >> 8;
    Tx_buff[10] = System.Pfout; //���书��

    Tx_buff[11] = System.Current[0] >> 8;
    Tx_buff[12] = System.Current[0]; //����
    Tx_buff[13] = System.Current[1] >> 8;
    Tx_buff[14] = System.Current[1];
    Tx_buff[15] = System.Current[2] >> 8;
    Tx_buff[16] = System.Current[2];
    Tx_buff[17] = System.Current[3] >> 8;
    Tx_buff[18] = System.Current[3];

    Tx_buff[19] = System.temp[1] >> 8;
    Tx_buff[20] = System.temp[1]; //�����¶�
    Tx_buff[21] = System.temp[3] >> 8;
    Tx_buff[22] = System.temp[3];
    Tx_buff[23] = System.temp[5] >> 8;
    Tx_buff[24] = System.temp[5];
    Tx_buff[25] = System.temp[7] >> 8;
    Tx_buff[26] = System.temp[7];

    Tx_buff[27] = System.temp[0] >> 8;
    Tx_buff[28] = System.temp[0]; //�������¶�
    Tx_buff[29] = System.temp[2] >> 8;
    Tx_buff[30] = System.temp[2];
    Tx_buff[31] = System.temp[4] >> 8;
    Tx_buff[32] = System.temp[4];
    Tx_buff[33] = System.temp[6] >> 8;
    Tx_buff[34] = System.temp[6];
    Tx_buff[35] = System.temp[8] >> 8;
    Tx_buff[36] = System.temp[8]; //�ϳ����¶�

    Tx_buff[37] = CurrNum >> 8;
    Tx_buff[38] = CurrNum; //�ܵ���
}
