
#include "system.h"

void calibration(void);

//数组定义，含义如题名：
u8 USART1_SEND_DATA[64];
u8 USART1_RECEIVE_DATA[64];

void RS485_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        //PD7端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /* 复用输出 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* 浮空输入 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); /* 空闲中断 */
    USART_Cmd(USART1, ENABLE);                     /* 使能 USART， 配置完毕 */
    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */
    GPIO_ResetBits(GPIOE, GPIO_Pin_9);      //默认为接收模式

    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RECEIVE_DATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; /* 从外设到内存 */
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* 单次传输模式 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    //    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);   /* 发送中断 */
    //    DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);   /* 接收中断 */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); /* Enable USART1 DMA RX request */
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

//void RS485_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//	GPIO_SetBits(GPIOE,GPIO_Pin_9);			    //设置为发送模式
//  for(t=0;t<len;t++)		                  //循环发送数据
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,buf[t]);
//	}
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_9);				//设置为接收模式
//}

void USART1_IRQHandler(void)
{
    u16 DATA_LEN;
    /* 检测到接收数据后，在数据总线上的一个字节时间内，没有接收到数据触发空闲中断。RXNE置位一次，空闲总线就检测一次！ */
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) /* 空闲总线中断 */
    {
        DMA_Cmd(DMA1_Channel5, DISABLE);                                              //关闭DMA,防止处理其间有数据
        DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5); //清标志

        /*读SR后读DR清除Idle 先读SR，然后读DR才能清除 */
        DATA_LEN = USART1->SR;
        DATA_LEN = USART1->DR;
        DATA_LEN = 64 - DMA_GetCurrDataCounter(DMA1_Channel5); //剩余数据数量
                                                               //				calibration();
        DATA_LEN = 64;
        DMA1_Channel5->CNDTR = DATA_LEN; //重装填
        DMA_Cmd(DMA1_Channel5, ENABLE);  //处理完,重开DMA
    }
    if (USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET) //出错
    {
        USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
    }
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_ClearITPendingBit(USART1, USART_IT_IDLE);
}

void calibration(void)
{
    if (USART1_RECEIVE_DATA[0] == 0 && USART1_RECEIVE_DATA[1] == 0) /* 0 校准 */
    {
        CalibrationFlag = 1;
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART1, 0x00);
    }
    else
    {
        CalibrationFlag = 2;
        K_value = (u16)(USART1_RECEIVE_DATA[0] << 8) + USART1_RECEIVE_DATA[1];
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART1, 0x01);
    }
}
