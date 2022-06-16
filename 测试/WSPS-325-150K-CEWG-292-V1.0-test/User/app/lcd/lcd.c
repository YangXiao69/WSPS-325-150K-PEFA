/*
  �յ���Ļ�Ĵ������ݸ�ʽ
	0    1     2        3        4   5       6               7   8
	A5  5A    06       83        00  00      01             00  00
	ͷ2byte   ����    ָ��       ��ַ         ���ݳ���       ��������
*/
#include "system.h" 

void lcd_send_two(u16 data1,u16 data2,u16 adrss);
void lcd_page(u8 page);

u16 RegisterAddr = 0;

u8  Reset_Flag = 0    ;//�쳣��λ
u8  waring_clear_flag = 0;

u16 StepPset=100;
u16 StepFreq=1;

u8 erroradr=0;   /* �쳣��̾�ŵ�ַ */

u8 retrest = 0;   /* �ָ��������� */

/* ϵͳ���ñ��� */

u8 Alarm       = 1;         /* �����ܿ��� ��1Ϊ������*/
u8 Alarm_Volt  = 1;   
u8 Alarm_Curr  = 1;      
u8 Alarm_Temp  = 1;        /* �¶ȱ��� */
u8 Alarm_Pout  = 1;        /* �ܹ��ʱ��� */    
u8 Alarm_Pfout = 1;        /* ���书�ʱ��� */
u8 Alarm_ZB    = 1;        /* פ������ */



u16 Alarm_RPD_MODE    =  1  ;
u16 Alarm_RPD_Value   =  300;
u16 Alarm_Vswr_MODE   =  1  ;


u8  Alarm_Vswr_Value  =  50;    // ����                                   
u16 Alarm_Pout_Value  ;         // ����
u16 Alarm_Pfout_Value ;         // ����
u16 Alarm_Flow_Value  =  100;
u16 Alarm_FlowTemp_Value = 300;


u8  Alarm_Curr2_Value ;  // ���
u8  Alarm_Vswr2_Value ;  // ���                               
u16 Alarm_Pout2_Value ;  // ���
u16 Alarm_Temp2_Value ;  // ��� 


u8 Store_Flag = 1;         /* �洢 */




u8 Alarm_Volt0  = 1;   
u8 Alarm_Curr0  = 1;      
u8 Alarm_Temp0  = 1;        /* �¶ȱ��� */
u8 Alarm_Pout0  = 1;        /* �ܹ��ʱ��� */    
u8 Alarm_Pfout0 = 1;        /* ���书�ʱ��� */
u8 Alarm_ZB0    = 1;        /* פ������ */
u8 Store_Flag0  = 1;        /* �洢 */
u8 Adrtop;
u32 boundtop;
u8  local_iptop[4], gatewaytop[4],subnettop[4];
u16 local_porttop;


/* �������ñ��� */
u8 Power = 1;              /* ��Դ״̬ �� 1  �� 0 */
u8 RF = 0;                 /* ��Ƶ     �� 1  ��0  */
u8 WorkMode = 1;           /* ����ģʽ ���� 1   ���� 0*/
u8 Duty = 0;               /* ռ�ձ� */
//Pset
u16 Pset = 0;              /* �趨���� */
u16 Phase = 0;             /* ��λ */
u16 PulseWidth = 10;       /* ���� */

u8 Source = 0;             /* Դ */
u8 PWMS   = 0;             /* pwmԴ */

u8 operate;                /* ���� */

u8 calebration=0;          /* 1�㹦��У׼  2������У׼ */

unsigned char  Rx_buff[48];
unsigned char  Tx_buff[48];
u16  len = 0;

u8 boundnum=0; 
u8 RestFlag = 0;      /* 1�� ����  485 ����ʼ����  2����ʼ������   3����ʼ��485 */
u8 mtset = 0;         /* �ֶ�����IP mtset=2 ��ʾ�ֶ����� */

u8 Adr = 10;
u32 bound = 115200;
u16 AlarmFlag=0;

u16 Freq=2450;

u8 time[5];                /* ��ȡ��ʱ���ݴ� */
u8 time0[5];               /* ���ص�BCDʱ��ֵ */

u16 user_settings[2] = {0,0};







/*
*  ������־�������ˢ�º��� 
*/
void lcd_send3(void)
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 0x0C;    /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x80;    /* ��̶���ַд������ָ�� */ 
    UART2_DMA_TX[4] = 0x56;   
    
    UART2_DMA_TX[5] = 0x5A;
    UART2_DMA_TX[6] = 0xA0; 
    
    UART2_DMA_TX[7] = 0x00;   
    UART2_DMA_TX[8] = 0x00;              
    UART2_DMA_TX[9] = 0x02;   
    UART2_DMA_TX[10] = 0x20; 
    
    UART2_DMA_TX[11] = 0x02; 
    UART2_DMA_TX[12] = 0x20;	

    UART2_DMA_TX[13] = 0x01; 
    UART2_DMA_TX[14] = 0x90;	   
 		
    USART2_Send(15);			     
}




/***********************************************************************************/
//�����ж����洦������������������ģ���ʱ��Ƶ������ᵼ�����翨��
void Send_for_chajian(u8 NumName)               //У׼�������
{
    Tx_buff[0]=0xFF;
    Tx_buff[1]=NumName;         //���1
    Tx_buff[2] = 1;
    Tx_buff[3]=UART2_DMA_RX[7];
    Tx_buff[4]=UART2_DMA_RX[8];
    sendto(0,Tx_buff, 5, remote_ip, remote_port); 									 
}


void Send_for_waring_clear(void)          //У׼�������
{
    Tx_buff[0]=0xFF;Tx_buff[1]=0xEE;      //���1
    Tx_buff[2]=UART2_DMA_RX[7];
    Tx_buff[3]=UART2_DMA_RX[8];
    sendto(0,Tx_buff, 4, remote_ip, remote_port); 									 
}






/*
*    ���� LCD���ݴ�����  
*/
void lcd_receive(void)     
{         
    u16 receivedata;
	
    if(UART2_DMA_RX[0] == 0xA5 && UART2_DMA_RX[1] == 0x5A)
    {
        if(UART2_DMA_RX[3] == 0x83 && UART2_DMA_RX[4] == 0)
        {
            receivedata = ((u16)UART2_DMA_RX[7]<<8) +UART2_DMA_RX[8];
            switch(UART2_DMA_RX[5])	
            {							
                case 0x04:
                    RegisterAddr = 0x0080;
                    Power = Modbus[RegisterAddr] = (u16)UART2_DMA_RX[8];	/* ��Դ */
                    break;
                                    
                case 0x05: 
                    RegisterAddr = 0x0081;
                    RF = Modbus[RegisterAddr] = (u16)UART2_DMA_RX[8];	               				   /* ��Ƶ */
                    break;
                                    
                case 0x06: 
                    if(receivedata<=MAXPOWER)      Pset = receivedata;
                    else if(receivedata == 0xFFFE)
                    {
                        if(Pset>=StepPset)		    Pset -= StepPset;
                        else                        Pset = 0;
                    }													    
                    else if(receivedata == 0xFFFF)  Pset += StepPset;
                                    
                        if(Pset>MAXPOWER) Pset=MAXPOWER;	
                        lcd_send_one(Pset,0x0006);												 
                        break;
												 
                case 0x07: 
                    StepPset = receivedata;	               	  /* ���ʵ��ڲ��� */
                    break;		
                case 0x08: 
                    if(receivedata<=MAX_FREQ)       Freq = receivedata;	 
                    else if(receivedata == 0xFFFE)  Freq -= StepFreq;									    
                    else if(receivedata == 0xFFFF)	Freq += StepFreq; 
                    if(Freq<MIN_FREQ) Freq = MIN_FREQ;
                    if(Freq>MAX_FREQ) Freq = MAX_FREQ;	
                         lcd_send_one(Freq,0x0008);				
                         set_freq(Freq);														 
                    break;												 
												 
                case 0x09: 
                    StepFreq = receivedata;	               			  /* Ƶ�� */
                    if(StepFreq>5) StepFreq = 5;
                    lcd_send_one(StepFreq,0x0009);		
                    break;						 
												 
                case 0x0F: 
                    if(receivedata==1)          
                    lcd_send_pageone(); 
                    else if(receivedata==2)     lcd_send_pagetwo(); 
                    else if(receivedata==1800) 
                    { 
                        lcd_page(9);
                        lcd_send_two(V_data2,V_data,0x008F);
                        lcd_send_one(V_data1,0x008E);
                    }
                    break;								 
                case 0x10: 
                    Adr = UART2_DMA_RX[8];	               				   /* ������ַ */
                    I2C_EE_ByteWrite(&Adr,0);				
                    break;
												 
                case 0x11: 
                    BoundNum232 = UART2_DMA_RX[8];	                 /* ������ַ */			 
                    bound = bounddata[BoundNum232];
                    I2C_EE_ByteWrite(&BoundNum232,1);			
                    USART_Cmd(USART1, DISABLE);     			 
                    UPUSART1_Config(bound);		
                    break;  
												 
                case 0x12: 
                    BoundNum485 = UART2_DMA_RX[8];	                 /* ������ַ */			 
                    bound = bounddata[BoundNum485];
                    I2C_EE_ByteWrite(&BoundNum485,2);			
                    USART_Cmd(USART3, DISABLE);     			 
                    UPUSART3_Config(bound);		
                    break;         
								
                case 0x13: 
                    local_ip2[0] = UART2_DMA_RX[7];	       /* IPǰ2�ֽ� */
                    local_ip2[1] = UART2_DMA_RX[8];
                    break;
                    
                case 0x14: 
                    local_ip2[2] = UART2_DMA_RX[7];	       /* IP��2�ֽ� */
                    local_ip2[3] = UART2_DMA_RX[8];	
                    break;
                    
                case 0x15: 
                    subnet2[0] = UART2_DMA_RX[7];	         /* ����ǰ2�ֽ� */
                    subnet2[1] = UART2_DMA_RX[8];
                    break;
                    
                case 0x16: 
                    subnet2[2] = UART2_DMA_RX[7];	         /* ���غ�2�ֽ� */
                    subnet2[3] = UART2_DMA_RX[8];	
                    break;
                    
                case 0x17: 
                    gateway2[0] = UART2_DMA_RX[7];	         /* ����ǰ2�ֽ� */
                    gateway2[1] = UART2_DMA_RX[8];
                    break;
                    
                case 0x18: 
                    gateway2[2] = UART2_DMA_RX[7];	         /* �����2�ֽ� */
                    gateway2[3] = UART2_DMA_RX[8];	 
                    break;	 
                    
                case 0x19: 
                    local_port2 = receivedata;	             /* �����2�ֽ� */
                    break;	 
                
                case 0x1A: 
                    if(receivedata)
                    {
                        RestFlag=2;
                        store_ip_24c02();
                    }
                    else
                    {
                        read_ip_24c02();
                    }
                    break;	 
                    
                    
                    
                
//����Դ�澯��У׼��������												 
                case 0x20: 
                    RegisterAddr = 0x00CA;
                    Alarm_Volt = Modbus[RegisterAddr] = receivedata;	 
                    I2C_EE_ByteWrite(&Alarm_Volt,20);										 
                    break;	
												 
                case 0x21: 
                    RegisterAddr = 0x00C8;
                    Alarm_Temp = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Temp;   
                    I2C_EE_ByteWrite(&Alarm_Temp,21);										 
                    break;	 
												 
                case 0x22: 
                    RegisterAddr = 0x00CC;
                    Alarm_Curr = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Curr;
                    I2C_EE_ByteWrite(&Alarm_Curr,22);										 
                    break;

                case 0x23: 
                    Alarm_ZB = receivedata;
                    RegisterAddr = 0x00C5;
                    Modbus[RegisterAddr] = (u16)Alarm_ZB  ;
                    I2C_EE_ByteWrite(&Alarm_ZB,23);										 
                    break;	 
                                    
                case 0x24: 
                    RegisterAddr = 0x00C0;
                    Alarm_Pout = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Pout;
                    I2C_EE_ByteWrite(&Alarm_Pout,24);										 
                    break;
                                    
                case 0x25: 
                    RegisterAddr = 0x00C2;
                    Alarm_Pfout = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Pfout;
                    I2C_EE_ByteWrite(&Alarm_Pfout,25);										 
                    break;													 
												 
                case 0x26: 
                    Alarm_Vswr_Value = receivedata;	 
                    I2C_EE_ByteWrite(&Alarm_Vswr_Value,26);										 
                    break;		
												 
                case 0x27: 
                    Alarm_Pout_Value = receivedata;	 
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],27);	delay_ms(10);				
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],28);		
                    break;		
                    
                case 0x28: 
                    Alarm_Pfout_Value = receivedata;	 
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],29);	delay_ms(10);				
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],30);									 
                    break;		
                    
                case 0x29:
                    RegisterAddr  = 0x0117;        
                    Alarm_Temp2_Value = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Temp2_Value;	
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],31);	delay_ms(10);				 //����¶�
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],32);									 
                    break;		
												 												 
                case 0x2A:
                    RegisterAddr  = 0x011B;
                    Alarm_Curr2_Value = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Curr2_Value;                                                //�������
                    I2C_EE_ByteWrite(&Alarm_Curr2_Value,33);				
                    break;	
												 
                case 0x2B:
                    RegisterAddr  = 0x0115;    
                    Alarm_Vswr2_Value = receivedata;
                    Modbus[RegisterAddr] = (u16)Alarm_Vswr2_Value ;                                                //���פ��
                    I2C_EE_ByteWrite(&Alarm_Vswr2_Value,34);				  
                    break;			
												 
                case 0x2C: 
                    RegisterAddr  = 0x0111;
                    Alarm_Pout2_Value = receivedata;
                    Modbus[0x111] = (u16)Alarm_Pout2_Value;	                                                //������������
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],35);	delay_ms(10);				
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],36);									 
                    break;													 												 
                
                case 0x8E: 
                    V_data1 = receivedata;                                       /* ���ؼ첨΢��׼ ���书�� */
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],41); delay_ms(10);			
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],42);										 
                    break;	
                    
                case 0x8F: 
                    V_data2 = receivedata;                                       /* ���ؼ첨΢��׼    */
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],37); delay_ms(10);			
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],38);										 
                    break;		
                    
                case 0x90: 
                    V_data = receivedata;                                       /* ���ؼ첨΢��׼ */
                    I2C_EE_ByteWrite(&UART2_DMA_RX[7],39); delay_ms(10);			
                    I2C_EE_ByteWrite(&UART2_DMA_RX[8],40);										 
                    break;		
                    
                case 0x91: Send_for_chajian(1);	 break;	
                case 0x92: Send_for_chajian(2);	 break;	
                case 0x93: Send_for_chajian(3);	 break;	
                case 0x94: Send_for_chajian(4);	 break;	
                case 0x95: Send_for_chajian(5);	 break;	
                case 0x96: Send_for_chajian(6);	 break;	
                case 0x97: Send_for_chajian(7);	 break;	
                case 0x98: Send_for_chajian(8);	 break;	
                case 0x99: Send_for_chajian(9);	 break;	
                case 0x9A: Send_for_chajian(10); break;	
                case 0x9B: Send_for_chajian(11); break;	
                case 0x9C: Send_for_chajian(12); break;	
                case 0x9D: Send_for_chajian(13); break;	
                case 0x9E: Send_for_chajian(14); break;	
                case 0x9F: Send_for_chajian(15); break;	
                case 0xA0: Send_for_chajian(16); break;
                case 0xA1: Send_for_chajian(17); break;
                case 0xA2: Send_for_chajian(18); break;
                case 0xA3: Send_for_chajian(19); break;
                case 0xA4: Send_for_chajian(20); break;
                case 0xA5: Send_for_chajian(21); break;
                case 0xA6: Send_for_chajian(22); break;
                case 0xA7: Send_for_chajian(23); break;
                case 0xA8: Send_for_chajian(24); break;
                									
            }
            ReadySend(RegisterAddr);     
        }				
    }
}

/* LCD��ҳ */
void lcd_page(u8 page)
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 0x03;    /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x80;    /* ��̶���ַд������ָ�� */ 
    UART2_DMA_TX[4] = 0x04;    
    UART2_DMA_TX[5] = page;
    USART2_Send(6);
}

 
/*
*  LCD����һ���ֵ�����
*/
void lcd_send_one(u16 data,u16 adrss) 
{
    while(UART2_TX_Finish)
    {
        Wait_USART2_Send();		 
    }
    
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 0x05;    /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x82;    /* ��̶���ַд������ָ�� */ 
    
    UART2_DMA_TX[4] = adrss>>8;/* ��ʼ��ַ��8λ */ 
    UART2_DMA_TX[5] = adrss;   /* ��ʼ��ַ��8λ */ 
    
    UART2_DMA_TX[6] = data>>8;
    UART2_DMA_TX[7] = data;
    
    USART2_Send(8);			
}  


/*
*  LCD���������ֵ�����
*/
void lcd_send_two(u16 data1,u16 data2,u16 adrss) 
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 0x07;    /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x82;    /* ��̶���ַд������ָ�� */ 
    UART2_DMA_TX[4] = adrss>>8;/* ��ʼ��ַ��8λ */ 
    UART2_DMA_TX[5] = adrss;   /* ��ʼ��ַ��8λ */ 
    UART2_DMA_TX[6] = data1>>8;
    UART2_DMA_TX[7] = data1;
    UART2_DMA_TX[8] = data2>>8;
    UART2_DMA_TX[9] = data2;
    USART2_Send(10);	
} 

/*
*  LCD���������ֵ�����
*/
void lcd_send_pagemain(void) 
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;                                               
    UART2_DMA_TX[1] = 0x5A;   /* д��ͷ */                                
    UART2_DMA_TX[2] = 23;     /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */
    UART2_DMA_TX[3] = 0x82;   /* ��̶���ַд������ָ�� */
    UART2_DMA_TX[4] = 0x00;   /* ��ַ��8λ */
    UART2_DMA_TX[5] = 0x00; 	
    
    UART2_DMA_TX[6]  = (u16)(PowerOut/10)>>8;     UART2_DMA_TX[7]  = (u16)(PowerOut/10); 	   //0	
    UART2_DMA_TX[8]  = (u16)PfoutOut>>8;          UART2_DMA_TX[9]  = (u16)PfoutOut;          //1
    UART2_DMA_TX[10] = (u16)PowerR>>8;         	  UART2_DMA_TX[11] = (u16)PowerR; 	         //2
    UART2_DMA_TX[12] = (u16)PowerZB>>8;        	  UART2_DMA_TX[13] = (u16)PowerZB; 	         //3
    UART2_DMA_TX[14] = 0x00;                	  UART2_DMA_TX[15] = Power;                  //4
    UART2_DMA_TX[16] = 0x00;	                  UART2_DMA_TX[17] = RF;                     //5
	UART2_DMA_TX[18] = (u16)Pset>>8;	          UART2_DMA_TX[19] = Pset;                   //6
    UART2_DMA_TX[20] = (u16)StepPset>>8;	      UART2_DMA_TX[21] = StepPset;               //7
    UART2_DMA_TX[22] = (u16)Freq>>8;	          UART2_DMA_TX[23] = Freq;                   //8		
    UART2_DMA_TX[24] = (u16)StepFreq>>8;	      UART2_DMA_TX[25] = StepFreq;               //9	
    
    USART2_Send(26);		
} 
/*
*  ˢ��ͨ������
*/
void lcd_send_pageone(void) 
{
    u8 i;
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 23;      /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x82;    /* ��̶���ַд������ָ�� */ 
    UART2_DMA_TX[4] = 0x00;    /* ��ʼ��ַ��8λ */ 
    UART2_DMA_TX[5] = 0x10;    /* ��ʼ��ַ��8λ */ 
    
    UART2_DMA_TX[6]  = 0x00;	UART2_DMA_TX[7]  = Adr;
    UART2_DMA_TX[8]  = 0x00;    UART2_DMA_TX[9]  = BoundNum232;
    UART2_DMA_TX[10] = 0x00;    UART2_DMA_TX[11] = BoundNum485;		
    
    for(i = 0; i < 4; i++)
    {
        UART2_DMA_TX[i+12] =  local_ip2[i];     
        UART2_DMA_TX[i+16] =  subnet2[i]; 
        UART2_DMA_TX[i+20] =  gateway2[i];  
    }
		
    UART2_DMA_TX[24] = local_port2>>8;  UART2_DMA_TX[25] = local_port2;	 		
    USART2_Send(26);	
} 

/*
*  ˢ�±�������
*/
void lcd_send_pagetwo(void) 
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;
    UART2_DMA_TX[1] = 0x5A;    /* д��ͷ */
    UART2_DMA_TX[2] = 29;      /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */		
    UART2_DMA_TX[3] = 0x82;    /* ��̶���ַд������ָ�� */ 
    UART2_DMA_TX[4] = 0x00;    /* ��ʼ��ַ��8λ */ 
    UART2_DMA_TX[5] = 0x20;    /* ��ʼ��ַ��8λ */ 
    
    UART2_DMA_TX[6]  = 0x00;	UART2_DMA_TX[7]  = Alarm_Volt;
    UART2_DMA_TX[8]  = 0x00;    UART2_DMA_TX[9]  = Alarm_Temp;
    UART2_DMA_TX[10] = 0x00;    UART2_DMA_TX[11] = Alarm_Curr;		
    UART2_DMA_TX[12] = 0x00;	UART2_DMA_TX[13] = Alarm_ZB;
    UART2_DMA_TX[14] = 0x00;    UART2_DMA_TX[15] = Alarm_Pout;
    UART2_DMA_TX[16] = 0x00;    UART2_DMA_TX[17] = Alarm_Pfout;	 
    
    UART2_DMA_TX[18] = 0x00;                    UART2_DMA_TX[19] = Alarm_Vswr_Value;
    UART2_DMA_TX[20] = Alarm_Pout_Value>>8; ;   UART2_DMA_TX[21] = Alarm_Pout_Value;	 		
    UART2_DMA_TX[22] = Alarm_Pfout_Value>>8; ;  UART2_DMA_TX[23] = Alarm_Pfout_Value;	 		
    
    UART2_DMA_TX[24] = Alarm_Temp2_Value>>8;    UART2_DMA_TX[25] = Alarm_Temp2_Value;
    UART2_DMA_TX[26] = 0x00 ;                   UART2_DMA_TX[27] = Alarm_Curr2_Value;	 		
    UART2_DMA_TX[28] = 0x00 ;                   UART2_DMA_TX[29] = Alarm_Vswr2_Value;	 	
    UART2_DMA_TX[30] = Alarm_Pout2_Value>>8 ;   UART2_DMA_TX[31] = Alarm_Pout2_Value;	 	
    
    USART2_Send(32);	
} 



/**************************************��Ӳ���*******************************/
void LCD_DisPA(uint8_t IPnum,uint8_t * Rx_b)
{
    
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;                                               
    UART2_DMA_TX[1] = 0x5A;   /* д��ͷ */                                
    UART2_DMA_TX[2] = 23;     /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */	                                                                          
    UART2_DMA_TX[3] = 0x82;   /* ��̶���ַд������ָ�� */ 
    switch(IPnum)
    {
        case 101:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x00; break;              
        case 102:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x10; break;             
        case 103:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x20; break;                
        case 104:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x30; break;			
        default: break;	 			
    }
    
    UART2_DMA_TX[6] = ((Rx_b[3]+Rx_b[4])*10) >> 8;
    UART2_DMA_TX[7] = ((Rx_b[3]+Rx_b[4])*10); //���ŵ�ѹ
    
    UART2_DMA_TX[8] = Rx_b[23];
    UART2_DMA_TX[9] = Rx_b[24];  /*�¶�*/

    UART2_DMA_TX[10] = Rx_b[11];
    UART2_DMA_TX[11] = Rx_b[12]; //����
    UART2_DMA_TX[12] = Rx_b[13];
    UART2_DMA_TX[13] = Rx_b[14];
    UART2_DMA_TX[14] = Rx_b[15];
    UART2_DMA_TX[15] = Rx_b[16];
    UART2_DMA_TX[16] = Rx_b[17];
    UART2_DMA_TX[17] = Rx_b[18];

    UART2_DMA_TX[18] = 0;
    UART2_DMA_TX[19] = 0; 
    UART2_DMA_TX[20] = 0;
    UART2_DMA_TX[21] = 0; 
    UART2_DMA_TX[22] = Rx_b[7];
    UART2_DMA_TX[23] = Rx_b[8]; 
    
    USART2_Send(24);			
}


void LCD_DisErr(uint8_t adr_H, uint8_t adr_L)
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;                                               
    UART2_DMA_TX[1] = 0x5A;   /* д��ͷ */                                
    UART2_DMA_TX[2] = 0x05;   /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */
                                                                            
    UART2_DMA_TX[3] = 0x82;   /* ��̶���ַд������ָ�� */  
    
    switch(adr_H)                                                             //��ʾ���ݵĵ�ַ
    {
        case 101: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x00 + adr_L;break;
        case 102: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x10 + adr_L;break;
        case 103: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x20 + adr_L;break;
        case 104: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x30 + adr_L;break;
        case 105: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x40 + adr_L;break;
        case 106: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x50 + adr_L;break;
        case 107: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x60 + adr_L;break;
        case 108: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x70 + adr_L;break;
        case 109: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x80 + adr_L;break;
        case 110: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0x90 + adr_L;break;
        case 111: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xA0 + adr_L;break;
        case 112: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xB0 + adr_L;break;
        case 113: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xC0 + adr_L;break;
        case 114: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xD0 + adr_L;break;
        case 115: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xE0 + adr_L;break;
        case 116: UART2_DMA_TX[4]=0x04;  UART2_DMA_TX[5] = 0xF0 + adr_L;break;
        
        case 117: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x00 + adr_L;break;
        case 118: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x10 + adr_L;break;	
        case 119: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x20 + adr_L;break;
        case 120: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x30 + adr_L;break;
        case 121: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x40 + adr_L;break;
        case 122: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x50 + adr_L;break;	
        case 123: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x60 + adr_L;break;
        case 124: UART2_DMA_TX[4]=0x05;  UART2_DMA_TX[5] = 0x70 + adr_L;break;			
    }							
    UART2_DMA_TX[6] = 0x00;    
    UART2_DMA_TX[7] = 1;  
    USART2_Send(8);							
}
void clear_lcd_display(u8 num)
{
    unsigned char i = 0;

    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;                                               
    UART2_DMA_TX[1] = 0x5A;   /* д��ͷ */                                
    UART2_DMA_TX[2] = 27;   /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */                                                                         
    UART2_DMA_TX[3] = 0x82;   /* ��̶���ַд������ָ�� */                
    switch(num)                                                             //��ʾ���ݵĵ�ַ
    {
        case 0:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x00; break;              
        case 1:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x10; break;             
        case 2:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x20; break;                
        case 3:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x30; break;
        case 4:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x40; break;
        case 5:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x50; break;
        case 6:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x60; break;                    
        case 7:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x70; break;                    
        case 8:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x80; break;                    
        case 9:		UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0x90; break;                    
        case 10:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xA0; break;
        case 11:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xB0; break;
        case 12:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xC0; break;              
        case 13:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xD0; break;             
        case 14:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xE0; break;                
        case 15:	UART2_DMA_TX[4] = 0x01;  UART2_DMA_TX[5] = 0xF0; break;
            
        case 16:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x00; break;
        case 17:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x10; break;
        case 18:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x20; break;                    
        case 19:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x30; break;                    
        case 20:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x40; break;                    
        case 21:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x50; break;                    
        case 22:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x60; break;
        case 23:	UART2_DMA_TX[4] = 0x02;  UART2_DMA_TX[5] = 0x70; break;  			
        default:  break;	 				
    }		                        
    for(i=0;i<24;i++)
    {
        UART2_DMA_TX[6+i] = 0;    
    } 	
    USART2_Send(30);								
}


void display_power(void)
{
    while(UART2_TX_Finish){Wait_USART2_Send();}
    UART2_DMA_TX[0] = 0xA5;                                               
    UART2_DMA_TX[1] = 0x5A;   /* д��ͷ */                                
    UART2_DMA_TX[2] = 0x0B;   /* ���ݳ���ע�⣺���ݳ���ֻ�������ֽ��Ժ� */
    UART2_DMA_TX[3] = 0x82;   /* ��̶���ַд������ָ�� */
    UART2_DMA_TX[4] = 0x00;   /* ��ַ��8λ */
    UART2_DMA_TX[5] = 0x00; 	
    UART2_DMA_TX[6] = (u16)(PowerOut/10)>>8;    
    UART2_DMA_TX[7] = (u16)(PowerOut/10); 		
    UART2_DMA_TX[8] = (u16)PowerR>>8;    
    UART2_DMA_TX[9] = (u16)PowerR; 
    UART2_DMA_TX[10] = (u16)PowerZB>>8;    
    UART2_DMA_TX[11] = (u16)PowerZB; 	
    UART2_DMA_TX[12] = (u16)Freq>>8;    
    UART2_DMA_TX[13] = (u16)Freq; 
    
    USART2_Send(14);		
}




