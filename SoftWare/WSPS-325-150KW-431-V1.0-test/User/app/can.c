/*
* ˵�������ݸ�ʽ������11λ��׼��ʶ�������3λ���ڼ�������֡�ò��ý��գ�
*       �м�4λ���ڱ�ʾ�ڵ㣬����λ���ڱ�ʶ�������ݣ���������������ݣ�
*/
#include "system.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;
u8 Can_Rseve_Flag = 0;

/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����,PB8�������룬PB9�������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����ʱ������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /*IO����*/
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    /* Configure CAN pin: RX */ // PA11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure CAN pin: TX */ // PA12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 */
void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure one bit for preemption priority */

    /*CANͨ���ж�ʹ��*/
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //FIFO0��Ϣ�Һ��ж�����.

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //�ж����ȼ� ��ռ��ģʽ����
    /*�ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; //CAN1 RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 */
static void CAN_Mode_Config(void)
{
    CAN_InitTypeDef CAN_InitStructure;
    /************************CANͨ�Ų�������**********************************/
    /*CAN�Ĵ�����ʼ��*/
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    /*CAN��Ԫ��ʼ��*/
    CAN_InitStructure.CAN_TTCM = DISABLE;         //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
    CAN_InitStructure.CAN_ABOM = ENABLE;          //MCR-ABOM  �Զ����߹���
    CAN_InitStructure.CAN_AWUM = ENABLE;          //MCR-AWUM  ʹ���Զ�����ģʽ
    CAN_InitStructure.CAN_NART = DISABLE;         //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
    CAN_InitStructure.CAN_RFLM = DISABLE;         //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���
    CAN_InitStructure.CAN_TXFP = DISABLE;         //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ��
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //��������ģʽ
                                                  //	  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;      //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;      //BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;      //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_Prescaler = 36;         //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.8Mbps
    CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ˵����������Ϊ���ι��ˣ�
 */
static void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    /*CAN��������ʼ��*/
    CAN_FilterInitStructure.CAN_FilterNumber = 0;                    //��������0
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  //�����ڱ�ʶ������λģʽ
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit; //������λ��Ϊ����16λ��
    /* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */
    /*
    *  ������ʹ��˵����16λ���ˣ���һ��32λ�Ĵ����ֳ�������������(ÿһλ�ֱ��Ӧ��fxr2��fxr1ͬ��)
       ID��    fxr1[15:8]               fxr1[7:0]
    	 ���Σ�	 fxr1[31:24]              fxr1[23:16]
    	 ӳ��  StdId[10:3]     StdId[2:0] RTR IDE ExtId[17��15]

    	 32λ���ˣ�
       ID:     fxr1[31:24]         fxr1[23:16]              fxr1[15:8]        fxr1[7:0]
    	 ����:   fxr2[31:24]          fxr2[23:16]             fxr2[15:8]        fxr2[7:0]
    	 ӳ��  StdId[10:3]      StdId[2:0] ExtId[17��13]	    ExtId[12��5]	    ExtId[4��0]	IDE RTR 0
    */
    /*
       ID��   fxr1[15:0]  (0x0700<<5)   1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0

    	 ӳ��                              StdId[10 : 0]            RTR  IDE  ExtId[17��15]

    	 ���Σ� fxr1[31:16] (0xE000)       1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0
    */
    CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)0xFFE0;              //fxr1 ��16λ
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)0x0701 << 5;          //fxr1 ��16λ  Ҫ���յ����ݵ� ID
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)0xFFFF;          //fxr2 ��16λ  �������ã�
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)0xFFFF;           //fxr2 ��16λ  �������ã�
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //��������������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               //ʹ�ܹ�����
    CAN_FilterInit(&CAN_FilterInitStructure);
}

/*
 * ��������CAN_Config
 */
void CAN_Config(void)
{
    CAN_GPIO_Config();
    //  CAN_NVIC_Config();
    CAN_Mode_Config();
    CAN_Filter_Config();
}

/*
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű����������� ���ݼ���
 */
void CAN_SetMsg(void)
{
    TxMessage.StdId = 0x701;      //��׼ID 11λ
                                  //  TxMessage.ExtId=0x131A;				 //��չID 29λ
    TxMessage.IDE = CAN_ID_STD;   //��׼��ʶ��ģʽ
    TxMessage.RTR = CAN_RTR_DATA; //���͵�������
    TxMessage.DLC = 8;            //���ݳ���Ϊ2�ֽ�
    TxMessage.Data[0] = 0xDC;
    TxMessage.Data[1] = 0xBA;
    TxMessage.Data[2] = 0x22;
    TxMessage.Data[3] = 0x33;
    TxMessage.Data[4] = 0x44;
    TxMessage.Data[5] = 0x55;
    TxMessage.Data[6] = 0x66;
    TxMessage.Data[7] = 0x77;
}

void can_reseve(void)
{
    if (CAN_MessagePending(CAN1, CAN_FIFO0)) //�н��յ�����
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

        /* �Ƚ��Ƿ��Ƿ��͵����ݺ�ID */
        if (RxMessage.StdId == 0x705)
        {
            Can_Rseve_Flag = RxMessage.DLC;
            ; //���ճɹ�
        }
        else
        {
            Can_Rseve_Flag = 0; //����ʧ��
        }
    }
}

/**************************END OF FILE************************************/
