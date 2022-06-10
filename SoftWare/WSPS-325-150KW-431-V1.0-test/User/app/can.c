/*
* 说明：数据格式，采用11位标准标识符，最高3位用于鉴别数据帧该不该接收，
*       中间4位用于标示节点，低四位用于标识数据内容（重新组合数据内容）
*/
#include "system.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;
u8 Can_Rseve_Flag = 0;

/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*外设时钟设置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /*IO设置*/
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    /* Configure CAN pin: RX */ // PA11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure CAN pin: TX */ // PA12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 */
void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure one bit for preemption priority */

    /*CAN通信中断使能*/
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //FIFO0消息挂号中断允许.

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //中断优先级 抢占级模式配置
    /*中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 */
static void CAN_Mode_Config(void)
{
    CAN_InitTypeDef CAN_InitStructure;
    /************************CAN通信参数设置**********************************/
    /*CAN寄存器初始化*/
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    /*CAN单元初始化*/
    CAN_InitStructure.CAN_TTCM = DISABLE;         //MCR-TTCM  关闭时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM = ENABLE;          //MCR-ABOM  自动离线管理
    CAN_InitStructure.CAN_AWUM = ENABLE;          //MCR-AWUM  使用自动唤醒模式
    CAN_InitStructure.CAN_NART = DISABLE;         //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM = DISABLE;         //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文
    CAN_InitStructure.CAN_TXFP = DISABLE;         //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //正常工作模式
                                                  //	  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;      //BTR-SJW 重新同步跳跃宽度 2个时间单元
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;      //BTR-TS1 时间段1 占用了6个时间单元
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;      //BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitStructure.CAN_Prescaler = 36;         //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+6+3)/4=0.8Mbps
    CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 说明：过滤器为屏蔽过滤，
 */
static void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    /*CAN过滤器初始化*/
    CAN_FilterInitStructure.CAN_FilterNumber = 0;                    //过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  //工作在标识符屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit; //过滤器位宽为单个16位。
    /* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
    /*
    *  过滤器使用说明：16位过滤，将一个32位寄存器分成了两个过滤器(每一位分别对应，fxr2与fxr1同理)
       ID：    fxr1[15:8]               fxr1[7:0]
    	 屏蔽：	 fxr1[31:24]              fxr1[23:16]
    	 映像：  StdId[10:3]     StdId[2:0] RTR IDE ExtId[17：15]

    	 32位过滤：
       ID:     fxr1[31:24]         fxr1[23:16]              fxr1[15:8]        fxr1[7:0]
    	 屏蔽:   fxr2[31:24]          fxr2[23:16]             fxr2[15:8]        fxr2[7:0]
    	 映像：  StdId[10:3]      StdId[2:0] ExtId[17：13]	    ExtId[12：5]	    ExtId[4：0]	IDE RTR 0
    */
    /*
       ID：   fxr1[15:0]  (0x0700<<5)   1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0

    	 映像：                              StdId[10 : 0]            RTR  IDE  ExtId[17：15]

    	 屏蔽： fxr1[31:16] (0xE000)       1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0
    */
    CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)0xFFE0;              //fxr1 高16位
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)0x0701 << 5;          //fxr1 低16位  要接收的数据的 ID
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)0xFFFF;          //fxr2 高16位  （不启用）
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)0xFFFF;           //fxr2 高16位  （不启用）
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //过滤器被关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               //使能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
}

/*
 * 函数名：CAN_Config
 */
void CAN_Config(void)
{
    CAN_GPIO_Config();
    //  CAN_NVIC_Config();
    CAN_Mode_Config();
    CAN_Filter_Config();
}

/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置 数据加载
 */
void CAN_SetMsg(void)
{
    TxMessage.StdId = 0x701;      //标准ID 11位
                                  //  TxMessage.ExtId=0x131A;				 //扩展ID 29位
    TxMessage.IDE = CAN_ID_STD;   //标准标识符模式
    TxMessage.RTR = CAN_RTR_DATA; //发送的是数据
    TxMessage.DLC = 8;            //数据长度为2字节
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
    if (CAN_MessagePending(CAN1, CAN_FIFO0)) //有接收到数据
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

        /* 比较是否是发送的数据和ID */
        if (RxMessage.StdId == 0x705)
        {
            Can_Rseve_Flag = RxMessage.DLC;
            ; //接收成功
        }
        else
        {
            Can_Rseve_Flag = 0; //接收失败
        }
    }
}

/**************************END OF FILE************************************/
