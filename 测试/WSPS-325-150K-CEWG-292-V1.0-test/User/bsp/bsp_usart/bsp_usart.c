#include "./system.h" 

u8 UART2_DMA_RX[64];
u8 UART2_DMA_TX[64];
u8 UART2_TX_Finish=0;

u8 UART1_DMA_RX[256];
u8 UART1_DMA_TX[256];
u8 UART1_TX_Finish=0;

u8 UART3_DMA_RX[256];
u8 UART3_DMA_TX[256];
u8 UART3_TX_Finish=0;


u8 BoundNum232;
u8 BoundNum485;
u32 bounddata[5]={115200,57600,38400,19200,9600};

void T_DE_GPIO_ININ(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
											 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);		

	GPIO_ResetBits(GPIOF, GPIO_Pin_14);	
    GPIO_ResetBits(GPIOG, GPIO_Pin_0);
}

/****************************************************************************************************/
                                        /*  USART2  */
/****************************************************************************************************/
void USART2_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);                   /* ʹ�� USART ʱ�� */	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         /* GPIO��ʼ�� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Tx����Ϊ���ù���  */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2  ;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Rx����Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);     /* ���� PXx �� USARTx_Tx*/
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);     /* ���� PXx �� USARTx__Rx*/

   /* ���ô�DEBUG_USART ģʽ */
    USART_InitStructure.USART_BaudRate = 115200;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_Init(USART2,&USART_InitStructure);  

    DMA_DeInit(DMA1_Stream5);                                               //DMAͨ������     ���ڷ�DMA���� 
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;      
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2)->DR);     
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(UART2_DMA_RX); 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    
    DMA_InitStructure.DMA_BufferSize = 64;      
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);    //����DMA1��ͨ��     
    DMA_Cmd(DMA1_Stream5,ENABLE);                  //ʹ��DMA����ͨ�� 
    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);                              //ʹ���ж�
		
    DMA_DeInit(DMA1_Stream6);                                                 //DMAͨ������     ���ڷ�DMA����   
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                            //DMA����ͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2)->DR);     //�����ַ         
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART2_DMA_TX;           //�ڴ��ַ                
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //dma���䷽��        
    DMA_InitStructure.DMA_BufferSize = 64;                                    //����DMA�ڴ���ʱ�������ĳ���        
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //����DMA���������ģʽ��һ������        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //���������ֳ� 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;       //�ڴ������ֳ� 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             //����DMA�Ĵ���ģʽ        
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       //����DMA�����ȼ���  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;         //ָ����FIFO��ֵˮƽ                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                  //ָ����Burstת�������ڴ洫��     
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //ָ����Burstת��������Χת�� //        
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);                               //����DMAx��ͨ��        
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);                              //ʹ���ж�  

    /* �ж� */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);          

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;    //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //���ж�    
    NVIC_Init(&NVIC_InitStructure);   

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;               //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�    
    NVIC_Init(&NVIC_InitStructure);     

    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);     //����DMA����
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  

    USART_Cmd(USART2, ENABLE);                       //ʹ�ܴ���
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);     //�����ж�
		
    T_DE_GPIO_ININ();
}

void USART2_Send(u8 len)
{
    UART2_TX_Finish=1;
    DMA_SetCurrDataCounter(DMA1_Stream6,len);     //����
    DMA_Cmd(DMA1_Stream6,ENABLE);
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  //ʹ���ж�  	
}


/*
*  USART2_��������ж�
*/
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //����������ڿ����ж�
    {
        USART_ReceiveData(USART2);                       //��仰�ܹؼ���û����仰�޷�������ڿ����ж�
        USART_ClearITPendingBit(USART2,USART_IT_IDLE);   //������ڿ����ж�
			
        while(DMA1_Stream5->CR & DMA_SxCR_EN)	  DMA_Cmd(DMA1_Stream5, DISABLE);           //�ر�DMA
			
        while(UART2_TX_Finish){Wait_USART2_Send();}			  
        lcd_receive();
        while(!(DMA1_Stream5->CR & DMA_SxCR_EN))
        {			
            DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);	
            DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TEIF5);					
            DMA1_Stream5->NDTR = 64;			
            DMA_Cmd(DMA1_Stream5, ENABLE);   					
        }
    }
}

void DMA1_Stream5_IRQHandler(void)                    // uart2 rx
{
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
        DMA1_Stream5->NDTR = 64;			
        DMA_Cmd(DMA1_Stream5, ENABLE);   			 
    }

    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TEIF5) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TEIF5);
        DMA1_Stream5->NDTR = 64;			
        DMA_Cmd(DMA1_Stream5, ENABLE); 			
    }
}
/* 
*  USART2_��������ж�
*/
void DMA1_Stream6_IRQHandler(void)                              //com6 uart2 tx
{
    if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)    //transfer complete interrupt flag
    {    
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_HTIF6);			
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);        
        DMA_Cmd(DMA1_Stream6,DISABLE);                          // stop DMA transmit
        DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  //ʹ���ж�  	
        UART2_TX_Finish = 0;		
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_HTIF6) != RESET)   //half transfer interrupt flag
    {
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_HTIF6);
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);        
        DMA_Cmd(DMA1_Stream6,DISABLE);                          // stop DMA transmit
        DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  //ʹ���ж�  				
        UART2_TX_Finish = 0;					
    }                                
}
void Wait_USART2_Send(void)
{
    if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)    //transfer complete interrupt flag
    {    
        DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_HTIF6);			
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6); 
        UART2_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream6,DISABLE);   			
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_HTIF6) != RESET)   //half transfer interrupt flag
    {
        DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_HTIF6);			
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6); 
        UART2_TX_Finish = 0;	
        DMA_Cmd(DMA1_Stream6,DISABLE);   			
    } 	
}

/****************************************************************************************************/
                                        /*  USART1  */
/****************************************************************************************************/
void UPUSART1_Config(u32 bound) 
{
    USART_InitTypeDef USART_InitStructure; 
        /* ���ô�DEBUG_USART ģʽ */
    USART_InitStructure.USART_BaudRate = bound;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_Init(USART1,&USART_InitStructure);  
}

void USART1_Config(u32 bound)      //DMA2_Channel_4   TX-Stream7   RX-Stream5
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);                   /* ʹ�� USART ʱ�� */	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         /* GPIO��ʼ�� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Tx����Ϊ���ù���  */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9  ;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Rx����Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);     /* ���� PXx �� USARTx_Tx*/
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);    /* ���� PXx �� USARTx__Rx*/
    
    /* ���ô�DEBUG_USART ģʽ */
    USART_InitStructure.USART_BaudRate = 115200;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_Init(USART1,&USART_InitStructure);  

    DMA_DeInit(DMA2_Stream5);                       //DMAͨ������     ���ڽ���DMA���� 
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;      
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1)->DR);     
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(UART1_DMA_RX); 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    
    DMA_InitStructure.DMA_BufferSize = 64;      
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);    //����DMA1��ͨ��     
    DMA_Cmd(DMA2_Stream5,ENABLE);                  //ʹ��DMA����ͨ�� 
    DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);                              //ʹ���ж�
		
    DMA_DeInit(DMA2_Stream7);                                                 //DMAͨ������     ���ڷ�DMA����   
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                            //DMA����ͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1)->DR);     //�����ַ         
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART1_DMA_TX;           //�ڴ��ַ                
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //dma���䷽��        
    DMA_InitStructure.DMA_BufferSize = 64;                                    //����DMA�ڴ���ʱ�������ĳ���        
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //����DMA���������ģʽ��һ������        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //���������ֳ� 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;       //�ڴ������ֳ� 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             //����DMA�Ĵ���ģʽ        
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       //����DMA�����ȼ���  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;         //ָ����FIFO��ֵˮƽ                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                  //ָ����Burstת�������ڴ洫��     
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //ָ����Burstת��������Χת�� //        
		DMA_Init(DMA2_Stream7, &DMA_InitStructure);                               //����DMAx��ͨ��        
    DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);                              //ʹ���ж�  

    /* �ж� */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);          

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;    //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //���ж�    
    NVIC_Init(&NVIC_InitStructure);   

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�    
    NVIC_Init(&NVIC_InitStructure);   
  

    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);     //����DMA����
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  

    USART_Cmd(USART1, ENABLE);                       //ʹ�ܴ���
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);     //�����ж�
}

void USART1_Send(u8 len)
{
    while(UART1_TX_Finish);
    GPIO_SetBits(GPIOG, GPIO_Pin_0);
    UART1_TX_Finish=1;
    DMA_SetCurrDataCounter(DMA2_Stream7,len);     //����
    DMA_Cmd(DMA2_Stream7,ENABLE);			
}

void Wait_USART1_Send(void)
{
    if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)    //transfer complete interrupt flag
    {    
        //  delay_us(2000);		
        DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_HTIF7);			
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7); 
        UART1_TX_Finish = 0;
        DMA_Cmd(DMA2_Stream7,DISABLE); 
        GPIO_ResetBits(GPIOG, GPIO_Pin_0);					
    }                        																					
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_HTIF7) != RESET)   //half transfer interrupt flag
    {
        //  delay_us(2000);				
        DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_HTIF7);			
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7); 
        UART1_TX_Finish = 0;	
        DMA_Cmd(DMA2_Stream7,DISABLE);   	
        GPIO_ResetBits(GPIOG, GPIO_Pin_0);				
    } 	
}
/*
*  USART1_��������ж�
*/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //����������ڿ����ж�
    {
        USART_ReceiveData(USART1); //��仰�ܹؼ���û����仰�޷�������ڿ����ж�
        USART_ClearITPendingBit(USART1,USART_IT_IDLE); //������ڿ����ж�

        while(DMA2_Stream5->CR & DMA_SxCR_EN)	  DMA_Cmd(DMA2_Stream5, DISABLE);           //�ر�DMA

        while(UART1_TX_Finish){Wait_USART1_Send();}
				
        remote_RS232_reseive();
								
        while(!(DMA2_Stream5->CR & DMA_SxCR_EN))
        {			
            DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);	
            DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TEIF5);					
            DMA2_Stream5->NDTR = 64;			
            DMA_Cmd(DMA2_Stream5, ENABLE);   					
        } 				
    }
}

void DMA2_Stream5_IRQHandler(void)                    // uart1 rx
{
    if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) != RESET)
    {
       DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
    }

    if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TEIF5) != RESET)
    {
       DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TEIF5);
    }
}
/* 
*  USART2_��������ж�
*/
void DMA2_Stream7_IRQHandler(void)                              //com6 uart2 tx
{
    if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)    //transfer complete interrupt flag
    {    
        delay_us(2000);			 		
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);        
        DMA_Cmd(DMA2_Stream7,DISABLE);                          // stop DMA transmit
        UART1_TX_Finish = 0;		
        GPIO_ResetBits(GPIOG, GPIO_Pin_0);				
    }                        																					
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_HTIF7) != RESET)   //half transfer interrupt flag
    {
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_HTIF7);
    }                                
}


/****************************************************************************************************/
                                        /*  USART3  */
/****************************************************************************************************/

void UPUSART3_Config(u32 bound)     //DMA2_Channel_4   TX-Stream7   RX-Stream5
{
    USART_InitTypeDef USART_InitStructure; 
   /* ���ô�DEBUG_USART ģʽ */
    USART_InitStructure.USART_BaudRate = bound;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_Init(USART3,&USART_InitStructure);  
}

void USART3_Config(u32 bound)     //DMA2_Channel_4   TX-Stream7   RX-Stream5
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);                   /* ʹ�� USART ʱ�� */	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         /* GPIO��ʼ�� */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Tx����Ϊ���ù���  */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8  ;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           /* ����Rx����Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);     /* ���� PXx �� USARTx_Tx*/
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);     /* ���� PXx �� USARTx__Rx*/

   /* ���ô�DEBUG_USART ģʽ */
    USART_InitStructure.USART_BaudRate = bound;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_Init(USART3,&USART_InitStructure);  

    DMA_DeInit(DMA1_Stream1);                                               //DMAͨ������     ���ڽ���DMA���� 
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;      
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3)->DR);     
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(UART3_DMA_RX); 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    
    DMA_InitStructure.DMA_BufferSize = 64;      
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);    //����DMA1��ͨ��     
    DMA_Cmd(DMA1_Stream1,ENABLE);                  //ʹ��DMA����ͨ�� 
    DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);                              //ʹ���ж�
		
    DMA_DeInit(DMA1_Stream3);                                                 //DMAͨ������     ���ڷ�DMA����   
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;                            //DMA����ͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3)->DR);     //�����ַ         
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART3_DMA_TX;           //�ڴ��ַ                
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                   //dma���䷽��        
    DMA_InitStructure.DMA_BufferSize = 64;                                    //����DMA�ڴ���ʱ�������ĳ���        
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //����DMA���������ģʽ��һ������        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //���������ֳ� 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;       //�ڴ������ֳ� 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                             //����DMA�Ĵ���ģʽ        
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       //����DMA�����ȼ���  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;         //ָ����FIFO��ֵˮƽ                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                  //ָ����Burstת�������ڴ洫��     
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       //ָ����Burstת��������Χת�� //        
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);                               //����DMAx��ͨ��        
    DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);                              //ʹ���ж�  

    /* �ж� */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);          

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;    //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //���ж�    
    NVIC_Init(&NVIC_InitStructure);   

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;               //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;              //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�    
    NVIC_Init(&NVIC_InitStructure);     

    USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);     //����DMA����
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);  

    USART_Cmd(USART3, ENABLE);                       //ʹ�ܴ���
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);     //�����ж�
}

void USART3_Send(u8 len)
{
    while(UART3_TX_Finish);
    GPIO_SetBits(GPIOF, GPIO_Pin_14);
    UART3_TX_Finish=1;
    DMA_SetCurrDataCounter(DMA1_Stream3,len);     //����
    DMA_Cmd(DMA1_Stream3,ENABLE);			
}

void Wait_USART3_Send(void)
{ 
    static u8 time=0;

    if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)    
    {    
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);          //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3); 
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);					
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_HTIF3) != RESET)  
    {
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_HTIF3);			
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);				
    }
    if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TEIF3) != RESET)   
    {    
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TEIF3); 	
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);					
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_DMEIF3) != RESET)  
    {
        delay_us(2000);				
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_DMEIF3); 		
        UART3_TX_Finish = 0;	
        DMA_Cmd(DMA1_Stream3,DISABLE);   	
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);						
    }
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_FEIF3) != RESET)  
    {
        delay_us(2000);				
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_FEIF3);	
        UART3_TX_Finish = 0;	
        DMA_Cmd(DMA1_Stream3,DISABLE);   	
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);						
    }		
    if(UART3_TX_Finish)
    { 
        time++; delay_us(2000);
        if(time>250)
        {
            time=0;
            DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
            DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_HTIF3);			
            DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3); 
            UART3_TX_Finish = 0;
            DMA_Cmd(DMA1_Stream3,DISABLE); 
            GPIO_ResetBits(GPIOF, GPIO_Pin_14);							
        }
    }
    else time=0; 		
}

/*
*  USART3_��������ж�
*/
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //����������ڿ����ж�
    {
        USART_ReceiveData(USART3); //��仰�ܹؼ���û����仰�޷�������ڿ����ж�
        USART_ClearITPendingBit(USART3,USART_IT_IDLE); //������ڿ����ж�
			
        while(DMA1_Stream1->CR & DMA_SxCR_EN)	  DMA_Cmd(DMA1_Stream1, DISABLE);           //�ر�DMA

        while(UART3_TX_Finish){ Wait_USART3_Send(); }	
				
        remote_RS485_reseive();
			
        while(!(DMA1_Stream1->CR & DMA_SxCR_EN))
        {			
            DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);	
            DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TEIF1);					
            DMA1_Stream1->NDTR = 64;			
            DMA_Cmd(DMA1_Stream1, ENABLE);   					
        } 
    }
}
void DMA1_Stream1_IRQHandler(void)                    // uart1 rx
{
    if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1) != RESET)
    {
       DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
    }

    if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TEIF1) != RESET)
    {
       DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TEIF1);
    }
}
/* 
*  USART3_��������ж�
*/
void DMA1_Stream3_IRQHandler(void)                              //com6 uart2 tx
{
    if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)    
    {    
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);          //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3); 
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);					
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_HTIF3) != RESET)  
    {
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_HTIF3);			
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);				
    }
    if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TEIF3) != RESET)   
    {    
        delay_us(2000);		
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TEIF3); 	
        UART3_TX_Finish = 0;
        DMA_Cmd(DMA1_Stream3,DISABLE); 
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);					
    }                        																					
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_DMEIF3) != RESET)  
    {
        delay_us(2000);				
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_DMEIF3); 		
        UART3_TX_Finish = 0;	
        DMA_Cmd(DMA1_Stream3,DISABLE);   	
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);						
    }
    if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_FEIF3) != RESET)  
    {
        delay_us(2000);				
        DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);  //ʹ���ж� 
        DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_FEIF3);	
        UART3_TX_Finish = 0;	
        DMA_Cmd(DMA1_Stream3,DISABLE);   	
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);						
    }		                                
}




///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
//int fputc(int ch, FILE *f)
//{
//		/* ����һ���ֽ����ݵ����� */
//		USART_SendData(USART2, (uint8_t) ch);
//		
//		/* �ȴ�������� */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
//int fgetc(FILE *f)
//{
//		/* �ȴ������������� */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART2);
//}


/*********************************************END OF FILE**********************/
