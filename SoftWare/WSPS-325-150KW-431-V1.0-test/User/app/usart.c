#include "system.h"

u8 USART2_SEND_DATA[64];
u8 USART2_RECEIVE_DATA[64];

/* 配置USART参数 */
void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* 串口1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /* 推挽输出 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* 浮空输入 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 串口2 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); /* 空闲中断 */
    USART_Cmd(USART1, ENABLE);                     /* 使能 USART， 配置完毕 */
    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
    如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); /* 开启空闲,帧错,噪声,校验错中断 */
    USART_Cmd(USART2, ENABLE);                     /* 使能 USART， 配置完毕 */
    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
    如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART2, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */
}

/* DMA配置 */
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //DMA1

    /* DMA1 Channel6 (triggered by USART2 Rx event) Config */
    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_RECEIVE_DATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; /* 从外设到内存 */
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    //    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);   /* 发送中断 */
    //    DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);   /* 接收中断 */
    /* Enable USART2 DMA RX request */
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); /* 串口DMA接收 */
    DMA_Cmd(DMA1_Channel6, ENABLE);

    //    /* DMA1 Channel7 (triggered by USART2 Tx event) Config */
    //    DMA_DeInit(DMA1_Channel7);
    //    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
    //    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_SEND_DATA;
    //    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;   /* 从内存到外设 */
    //    DMA_InitStructure.DMA_BufferSize = 64;
    //    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    //    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);     /* 发送中断 */
    ////    DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE);   /* 接收中断 */
    //    /* Enable USART1 DMA TX request */
    //    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    //    DMA_Cmd(DMA1_Channel7, DISABLE);
}

/* 中断优先级配置 */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 定时器2中断配置 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 定时器4中断配置 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                           //初始化NVIC寄存器

    /* ADC中断配置 */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //    //Enable DMA Channel4 Interrupt
    //    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;      /* DMA发送完成中断优先级应最高 */
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);
    //    //Enable DMA Channel7 Interrupt
    //    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;       /* DMA发送完成中断优先级应最高 */
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);
}

/* USART2 Rx: DMA1 Channel6 */
void USART2_IRQHandler(void)
{
    u16 DATA_LEN;

    /* 检测到接收数据后，在数据总线上的一个字节时间内，没有接收到数据触发空闲中断。RXNE置位一次，空闲总线就检测一次！*/
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) /* 空闲中断 */
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);                                              /* 首先关闭DMA,防止处理其间有数据，防止被其他中断打断不能及时关闭DMA*/
        DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6); /* 清除标志位 */

        /*读SR后读DR清除Idle 先读SR，然后读DR才能清除 */
        DATA_LEN = USART2->SR;
        DATA_LEN = USART2->DR;
        DATA_LEN = 64 - DMA_GetCurrDataCounter(DMA1_Channel6); /* 得到接收数据长度 */

        DATA_LEN = 64;
        DMA1_Channel6->CNDTR = DATA_LEN; /* 重新写入接受数据长度 */
        DMA_Cmd(DMA1_Channel6, ENABLE);  /* 打开接受通道 */
    }
    if (USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET) /* 出错 */
    {
        USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE);
    }
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    USART_ClearITPendingBit(USART2, USART_IT_IDLE);
}

///* DMA1_Channel7中断服务函数 串口2 DMA发送完成触发 */
//void DMA1_Channel7_IRQHandler(void)
//{
//  if(DMA_GetITStatus(DMA1_FLAG_TC7))
//	{
//	    DMA_ClearITPendingBit(DMA1_IT_TC7);
//	    DMA_ClearITPendingBit(DMA1_IT_TE7);
//	    DMA_Cmd(DMA1_Channel7, DISABLE);  //关闭DMA
//	}
//}
