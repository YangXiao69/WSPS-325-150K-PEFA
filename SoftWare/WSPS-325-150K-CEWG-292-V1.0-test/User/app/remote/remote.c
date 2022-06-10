#include "system.h" 
#include "misc.h"



u16 Modbus[2048];

uint8_t CalibFlag = 0;

u8 Calib_Tx_buff[4] = {0};


const uint8_t VerSionInfo[50] = {"WSPS-2450-12K-CEWG-292-V1.0\r\n"};



/*
*  CRC校验
*/
uint16_t crc16_modbus(u8 *data, u8 length)    // 0106000100179804
{  
    uint8_t i;  
    uint16_t crc = 0xffff;                    // Initial value  
    while(length--)  
    {  
        crc ^= *data++;                       // crc ^= *data; data++;  
        for (i = 0; i < 8; ++i)  
        {  
            if (crc & 1)  
                crc = (crc >> 1) ^ 0xA001;    // 0xA001 = reverse 0x8005  
            else  
                crc = (crc >> 1);  
        }  
    }  
    return crc;  
}  


void work_data_clean(u8 num)
{
    u8 i;
    u16 data;
	
    if(num>24) return;                                          //无效保护	
	
    data = num * 0x40;
    for(i=0; i<0x26; i++)
    {
        Modbus[0x0100 + data + i] = 0x00;			
    }   		
}
/*
num  模块组数
buff 传入数据
*/
void work_data_stor(u8 num,u8 *buff)
{
    u16 data;
    
    if(num == 0 || num == 1)
    {
        return;
    }
    
    if(num>24) return;                                   //无效保护	
    data = num * 0x40;
  
    Modbus[0x0100+data] = ((u16)buff[0]<<8) |buff[1];    //告警9  10
    Modbus[0x0101+data] = ((u16)buff[18]<<8)|buff[19];   //输出11 12
    Modbus[0x0102+data] = ((u16)buff[20]<<8)|buff[21];   //反射13 14
    Modbus[0x0103+data] = ((u16)buff[22]<<8)|buff[23];   //驻波15 16
    Modbus[0x0600+ num] = ((u16)buff[24]<<8)|buff[25];   //插件校正系数
//    if (num%2==1)
//    {
//        Modbus[0x0104+data] = Modbus[0x0104+((num-1) * 0x40)];
//    }
//    else
//    {
//        Modbus[0x0104+data] = ((u16)buff[4]<<8) |buff[5];    //温度17 18
//    }
    

    Modbus[0x0104+data] = ((u16)buff[4]<<8) |buff[5];    //温度17 18
       
    
    
    Modbus[0x0105+data] = ((u16)buff[2]<<8) |buff[3];    //电压19 20
    
    Modbus[0x0120+data] = ((u16)buff[6]<<8) |buff[7];    //电流41 42
    Modbus[0x0121+data] = ((u16)buff[8]<<8) |buff[9];    //电流43 44
    Modbus[0x0122+data] = ((u16)buff[10]<<8)|buff[11];   //电流45 46
    Modbus[0x0123+data] = ((u16)buff[12]<<8)|buff[13];   //电流47 48
    Modbus[0x0124+data] = ((u16)buff[14]<<8)|buff[15];   //电流49 50
    Modbus[0x0125+data] = ((u16)buff[16]<<8)|buff[17];   //电流51 52	  
}


/********************************************************************************/
                              /* RS485   USART3 */
/********************************************************************************/
void work_485send_buff(u16 addr,u8 len,u8 way)
{
    u16 i,crc; 
	
    if(len>120) return;
    
    while(UART3_TX_Finish){Wait_USART3_Send();}		
    UART3_DMA_TX[0] = Adr;
    UART3_DMA_TX[1] = way;	
		
    if(way==3)                                                     //读
    {
        UART3_DMA_TX[2]=len*2; 	
        for(i=0;i< len;i++)
        {
            UART3_DMA_TX[2*i+3] = Modbus[i+addr]>>8;
            UART3_DMA_TX[2*i+4] = Modbus[i+addr];
        }
        crc=crc16_modbus(UART3_DMA_TX,3+len*2);
        UART3_DMA_TX[3+len*2] = crc;
        UART3_DMA_TX[4+len*2] = crc>>8;
        USART3_Send(5+len*2);				
    }
    else if(way==6)                                              //写
    {
        UART3_DMA_TX[2]=addr>>8; UART3_DMA_TX[3]=addr;	
			
        UART3_DMA_TX[4] = Modbus[addr]>>8;
        UART3_DMA_TX[5] = Modbus[addr];				
        
        crc=crc16_modbus(UART3_DMA_TX,6);
        UART3_DMA_TX[6] = crc;
        UART3_DMA_TX[7] = crc>>8;
        USART3_Send(8);						
	}
	else if(way==16)                                             //写
    {
        UART3_DMA_TX[2]=addr>>8; UART3_DMA_TX[3]=addr;	
        UART3_DMA_TX[4] = 0;  	 UART3_DMA_TX[5] = len*2;					  
        crc=crc16_modbus(UART3_DMA_TX,6);
        UART3_DMA_TX[6] = crc;
        UART3_DMA_TX[7] = crc>>8;	
        USART3_Send(8);		
    }	
}


void remote_RS485_reseive(void)   // 01 06  00 01  00 01  CRC
{
    u16 Data,ADDR;

    if(UART3_DMA_RX[0] == Adr || UART3_DMA_RX[0]==0x00)
    {
        ADDR = ((u16)UART3_DMA_RX[2] << 8) + UART3_DMA_RX[3];           //访问地址
        Data = ((u16)UART3_DMA_RX[4] << 8) + UART3_DMA_RX[5];           //写数据 或者读取长度
        
        if(UART3_DMA_RX[1] == 0x06)                                     /* 写指令 */
        {
            switch(ADDR)
            {
                case 0x0010: 
                    if(Data)  NVIC_SystemReset();  
                    break;  
                        
                case 0x0011: 
                    Modbus[ADDR] = Adr = Data;  
                    I2C_EE_ByteWrite(&Adr,0);						
                    break;	
                        
                case 0x0012: 
                    Modbus[ADDR] = BoundNum232 = Data;	
                    I2C_EE_ByteWrite(&BoundNum232,1);							
                    break;	
                    
                case 0x0013: 
                    Modbus[ADDR] = BoundNum485 = Data;	                
                    I2C_EE_ByteWrite(&BoundNum485,2);			
                    break;	
                    
                case 0x0080: 
                    Modbus[ADDR] = Power = Data;	                /* 电源 */
                    break;							
                
                case 0x0081: 
                    Modbus[ADDR] = RF    = Data;	                /* 射频 */			 
                    break;		
                                        
                case 0x0082: 
                    Modbus[ADDR] = Pset = Data;	     
                    if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                    break; 								           
                                    
                case 0x0083: 
                    Modbus[ADDR] = Freq = Data; /* 频率 */  			
                    set_freq(Freq);		
                    break; 		
                                        
                case 0x0084: 
                    Modbus[ADDR] = Phase = 0;   /* 频率 */  			 
                    break;		
                                                
                case 0x0085: 
                    Modbus[ADDR] = 0;           /* 连续 */  			 
                    break;	
                                                
                case 0x0086: 
                    Modbus[ADDR] = Duty = 100;  /* 占空比 */  			
                    break; 
                                                
                case 0x0087: 
                    Modbus[ADDR] = Data;
                    break; 
                                            
                case 0x00C5: 
                    Modbus[ADDR] = Alarm_ZB = Data;               //驻波开关
                    I2C_EE_ByteWrite(&Alarm_ZB,23);					
                    break;	
                                            
                case 0x00C0: 
                    Modbus[ADDR] = Alarm_Pout = Data;             //过功率开关
                    I2C_EE_ByteWrite(&Alarm_Pout,24);		
                    break;		
                                    
                case 0x00C2: 
                    Modbus[ADDR] = Alarm_Pfout = Data;            //反射过功率开关
                    I2C_EE_ByteWrite(&Alarm_Pfout,25);				
                    break;		
                    
                case 0x00C7: 
                    Modbus[ADDR] = Alarm_Vswr_Value = Data;	     //驻波门限
                    I2C_EE_ByteWrite(&Alarm_Vswr_Value,26);										 
                    break;
                                        
                case 0x00C1: 
                    Modbus[ADDR] = Alarm_Pout_Value = Data;       //过功率门限
                    I2C_EE_ByteWrite(&UART3_DMA_RX[4],27); 
                    delay_ms(10);			
                    I2C_EE_ByteWrite(&UART3_DMA_RX[5],28);		
                    break;		
                                        
                case 0x00C4: 
                    Modbus[ADDR] = Alarm_Pfout = Data;            //反射过功率门限
                    I2C_EE_ByteWrite(&UART3_DMA_RX[4],29);	
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&UART3_DMA_RX[5],30);		
                    break;		
                default:		 
                    break;	
            }								
            work_485send_buff(ADDR,1,6); 
            if(ADDR==0x0012)
            { 		
                bound = bounddata[BoundNum232];
                USART_Cmd(USART1, DISABLE);     			 
                UPUSART1_Config(bound);	 
            }	
            if(ADDR==0x0013)
            { 		
                bound = bounddata[BoundNum485];
                USART_Cmd(USART3, DISABLE);     			 
                UPUSART3_Config(bound);
            }	
        }
        else if(UART3_DMA_RX[1] == 0x03)  
        work_485send_buff(ADDR,Data,3);	
            
        else if(UART3_DMA_RX[1] == 0x10)                   //连续写
        {
            if(ADDR==0x0080 && Data==3)
            {	
                Modbus[0x0080] = Power = UART3_DMA_RX[8];	                /* 电源 */
                Modbus[0x0081] = RF = UART3_DMA_RX[10];                   /* 射频 */
                
                Modbus[ADDR] = Pset = ((u16)UART3_DMA_RX[11] << 8) + UART3_DMA_RX[12];
                if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                
                work_485send_buff(ADDR,Data,16);								
            }
            if(ADDR==0x0014 && Data==7)
            {
                local_ip2[0] = UART3_DMA_RX[7];	    local_ip2[1] = UART3_DMA_RX[8];
                local_ip2[2] = UART3_DMA_RX[9];	    local_ip2[3] = UART3_DMA_RX[10];	
                    
                subnet2[0]   = UART3_DMA_RX[11];	  subnet2[1]   = UART3_DMA_RX[12];
                subnet2[2]   = UART3_DMA_RX[13];	  subnet2[3]   = UART3_DMA_RX[14];
                    
                gateway2[0]  = UART3_DMA_RX[15];	  gateway2[1]  = UART3_DMA_RX[16];
                gateway2[2]  = UART3_DMA_RX[17];	  gateway2[3]  = UART3_DMA_RX[18];	
                    
                local_port2 = ((u16)UART3_DMA_RX[19] << 8) | UART3_DMA_RX[20];
                    
                Modbus[0x0014]=((u16)UART3_DMA_RX[7] << 8) | UART3_DMA_RX[7];
                Modbus[0x0015]=((u16)UART3_DMA_RX[9] << 8) | UART3_DMA_RX[9];
                        
                Modbus[0x0016]=((u16)UART3_DMA_RX[11] << 8) | UART3_DMA_RX[12];
                Modbus[0x0017]=((u16)UART3_DMA_RX[13] << 8) |UART3_DMA_RX[14];
                
                Modbus[0x0018]=((u16)UART3_DMA_RX[15] << 8) | UART3_DMA_RX[16];
                Modbus[0x0019]=((u16)UART3_DMA_RX[17] << 8) | UART3_DMA_RX[18];
                
                Modbus[0x001A]=((u16)UART3_DMA_RX[19] << 8) | UART3_DMA_RX[20];
                        
                RestFlag=2;
                store_ip_24c02();
                work_485send_buff(ADDR,Data,16);		
                lcd_send_pageone();								
            }					
        }
    }	
}


/********************************************************************************/
                               /*RJ45 UDP */
/********************************************************************************/
void work_RJ45send_buff(u16 addr,u8 len,u8 way)
{
    u16 i = 0; 
//	u16 crc = 0; 
    u8 RJ45_TX_Buff[256] = {0};
    
    if(len>120) return;

    RJ45_TX_Buff[0] = RJ45_Rx_buff[0];
    RJ45_TX_Buff[1] = RJ45_Rx_buff[1];	
    RJ45_TX_Buff[2] = RJ45_Rx_buff[2];
    RJ45_TX_Buff[3] = RJ45_Rx_buff[3];		
    
    RJ45_TX_Buff[6] = Adr;
    RJ45_TX_Buff[7] = way;	
		
    if(way==3)                                                     //读
    {
        RJ45_TX_Buff[4]=0x00; RJ45_TX_Buff[5]=len*2+3; 	
        RJ45_TX_Buff[8]=len*2; 
        for(i=0;i< len;i++)
        {
            RJ45_TX_Buff[2*i+9]  = Modbus[i+addr]>>8;
            RJ45_TX_Buff[2*i+10] = Modbus[i+addr] & 0xFF;
        }
//	    crc=crc16_modbus(RJ45_TX_Buff,9+len*2);
//        RJ45_TX_Buff[9+len*2] = crc;
//        RJ45_TX_Buff[10+len*2] = crc>>8;
//        sendto_top(0,RJ45_TX_Buff,len*2+11, remote_ip2, remote_port2); 
		sendto_top(0,RJ45_TX_Buff,len*2+9, remote_ip2, remote_port2);		
    }
    else if(way==6)                                              //写
    {
//        RJ45_Rx_buff[8] = addr >> 8;
//        RJ45_Rx_buff[9] = addr &0xFF;
//        RJ45_Rx_buff[10]= Modbus[addr] >> 8;
//        RJ45_Rx_buff[11]= Modbus[addr] & 0xFF;
        sendto_top(0,RJ45_Rx_buff,12,  remote_ip2, remote_port2); 			
    }
    else if(way==16)                                              //写
    {
        RJ45_TX_Buff[4]  = 0x00;   RJ45_TX_Buff[5] = 6; 	
        RJ45_TX_Buff[8]  = addr>>8;RJ45_TX_Buff[9] = addr;				  
        RJ45_TX_Buff[11] = len;
    
        sendto_top(0,RJ45_TX_Buff,12,remote_ip2,remote_port2); 			
    }		
}



void SendVersionInfo(void)
{
    uint8_t i = 0;
    uint8_t RJ45_TxBuff[80] = {0};
    
    RJ45_TxBuff[0] = RJ45_Rx_buff[0];
    RJ45_TxBuff[1] = RJ45_Rx_buff[1];	
    RJ45_TxBuff[2] = RJ45_Rx_buff[2];
    RJ45_TxBuff[3] = RJ45_Rx_buff[3];		
    
    RJ45_TxBuff[6] = Adr;
    RJ45_TxBuff[7] = 0x03;    
    
    
    
    for(i = 0; i< 50; i++)
    {
        RJ45_TxBuff[i + 8] = VerSionInfo[i];
    
    }
    sendto_top(0, RJ45_TxBuff, 60, remote_ip2,remote_port2);

}


void remote_RJ45_reseive(void)
{
    u16 Data = 0, ADDR = 0, k=0;
    uint8_t iap_buf = 0x06;
    
    if((RJ45_Rx_buff[6] == Adr || RJ45_Rx_buff[6]==0x00) && (RJ45_Rx_buff[8] != 0xFF))
    {
        ADDR = ((u16)RJ45_Rx_buff[8] << 8)  + RJ45_Rx_buff[9] ;//访问地址
        Data = ((u16)RJ45_Rx_buff[10] << 8) + RJ45_Rx_buff[11];//写数据 或者读取长度
	
        if((ADDR >= 0x800))
        {
            return;
        }
        
        
        if(RJ45_Rx_buff[7] == 0x06)                                     /* 写指令 */
		{
            
            if(CirBuff_IsFull(&cp))
            {
                return;
            } 
          
            switch(ADDR)
            {
                                            
//                case 0x0010: 
//                    if(Data)  NVIC_SystemReset();  
//                    break;  
					
                case 0x0011: 
                    Modbus[ADDR] = Adr = Data;  
                    I2C_EE_ByteWrite(&Adr,0);						
                    break;	
                    
                case 0x0012: 
                    Modbus[ADDR] = BoundNum232 = Data;	
                    I2C_EE_ByteWrite(&BoundNum232,1);							
                    break;	
                    
                case 0x0013: 
                    Modbus[ADDR] = BoundNum485 = Data;	                
                    I2C_EE_ByteWrite(&BoundNum485,2);			
                    break;	

                case 0x007F: 
                    Reset_Flag = Data;	                          /* 复位 */
                    if (Reset_Flag)
                    {
                        //waring_clear_flag = 1;
                        //Send_for_waring_clear();
                        for(k=0;k<6;k++)
                        {clear_lcd_display(k);work_data_clean(k);}
                                                                                            
                        LED_CONTROL(ALARM_OFF_LED);
                        delay_ms(2000);  
                                        
                        RF = 0;
                        Modbus[0x0081] = RF;
                        
                        AlarmFlag  = 0;
                                                                                                                                        
                        LED_CONTROL(ALARM_OFF_LED);
                        
                            
                        Pset = 0;
                        Modbus[0x0082] = Pset  ;							 
                    }
                    break;	
                    
                case 0x0080:
                    RegisterAddr = 0x0080;    
                    Modbus[RegisterAddr] = Power = Data;	                /* 电源 */
                    break;							
                
                case 0x0081: 
                    RegisterAddr = 0x0081;
                    Modbus[RegisterAddr] = RF = Data;	                  /* 射频 */			 
                    break;		
                                        
                case 0x0082: 
                    Modbus[ADDR] = Pset = Data;	     
                    if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                    break; 								           
                                            
                case 0x0083: 
                    Modbus[ADDR] = Freq = Data; /* 频率 */  			
                    set_freq(Freq);		
                    break; 				
                                    
                case 0x0084: 
                    Modbus[ADDR] = Phase = 0;   /* 频率 */  			 
                    break;		
                                        
                case 0x0085: 
                    Modbus[ADDR] = 0;           /* 连续 */  			 
                    break;	
                                        
                case 0x0086: 
                    Modbus[ADDR] = Duty = 100;  /* 占空比 */  			
                    break; 
                                        
                case 0x0087: 
                    Modbus[ADDR] = Data;
                    break; 
											 						
                case 0x008C: 
                    Modbus[ADDR] = Data;
                    if(Modbus[0x8C])
                    {
                        RF_OFF();
                        Modbus[0x0081] = RF = 0;
                        LED_CONTROL(ALARM_OFF_LED);
                        LED_CONTROL(ALARM_RED_LED);
                    }
                    else
                    {
                        LED_CONTROL(ALARM_OFF_LED);
                    }
                    break; 
					
                case 0x00A0: 
                    Modbus[ADDR] = Data;
                    if(Modbus[0xA0])
                    {   
                        LED_CONTROL(ALARM_OFF_LED);
                        LED_CONTROL(ALARM_YEL_LED);
                    }
                    else
                    {
                        LED_CONTROL(ALARM_OFF_LED);
                    
                    }
                    break; 


                
/******************************************功率源告警和校准参数设置**********************************************/		
                case 0x00CA:
                    RegisterAddr = 0x00CA;    
                    Modbus[RegisterAddr] = Alarm_Volt = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Volt,20);											 
                    break;	
                                
                case 0x00C8: 
                    RegisterAddr = 0x00C8;
                    Modbus[RegisterAddr] = Alarm_Temp = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Temp,21);
                    break;	  
                                
                case 0x00CC: 
                    RegisterAddr = 0x00CC;
                    Modbus[RegisterAddr] = Alarm_Curr = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Curr,22);													 
                    break;
                    
                case 0x00C5: 
                    RegisterAddr = 0x00C5;
                    Modbus[RegisterAddr] = Alarm_ZB = Data ;   	 
                    I2C_EE_ByteWrite(&Alarm_ZB,23) ;													 
                    break;	 
                                
                case 0x00C0:
                    RegisterAddr = 0x00C0;        
                    Modbus[RegisterAddr] = Alarm_Pout = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Pout,24);												 
                    break;
                                
                case 0x00C2:
                    RegisterAddr = 0x00C2;
                    Modbus[RegisterAddr] = Alarm_Pfout = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Pfout,25);													 
                    break;													 
                                
                case 0x00C7: 
                    Modbus[ADDR] = Alarm_Vswr_Value = Data;	 
                    I2C_EE_ByteWrite(&Alarm_Vswr_Value,26);												 
                    break;		
                                            
                case 0x00C1: 
                    Modbus[ADDR] = Alarm_Pout_Value = Data;	 
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],27) ;	
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],28) ;												 
                    break;		 
                    
                case 0x00C4: 
                    Modbus[ADDR] = Alarm_Pfout_Value = Data;	 
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],29)  ;
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],30)  ;												 
                    break;		
                                                
                case 0x00D0: 
                    Modbus[ADDR] = Alarm_FlowTemp_Value = Data;   //  流量计水温过高门限	 
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],51) ;	
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],52) ;												 
                    break;		 
                    
                case 0x00D1: 
                    Modbus[ADDR] = Alarm_Flow_Value = Data;	     //  流量计流速过低门限 
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],53)  ;
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],54)  ;												 
                    break;															 
                        
                case 0x0117:
                    RegisterAddr  = 0x0117;    
                    Modbus[RegisterAddr] = Alarm_Temp2_Value = Data;//插件温度	 
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],31)  ;
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],32)  ;													 
                    break;		
                                                                    
                case 0x011B:
                    RegisterAddr  = 0x011B;
                    Modbus[RegisterAddr] = Alarm_Curr2_Value = Data;//插件电流
                    I2C_EE_ByteWrite(&Alarm_Curr2_Value,33);											 
                    break;	
                                        
                case 0x0115:
                    RegisterAddr  = 0x0115;    
                    Modbus[RegisterAddr] = Alarm_Vswr2_Value = Data;//插件驻波
                    I2C_EE_ByteWrite(&Alarm_Vswr2_Value,34);												 
                    break;			
                                            
                case 0x0111:
                    RegisterAddr = 0x0111;    
                    Modbus[RegisterAddr] = Alarm_Pout2_Value = Data;//插件输出过功率
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],35)  ;	
                    delay_ms(10)                           ;				
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],36)  ;													 
                    break;													 												 
                    
                case 0x00E2: 
                    Modbus[ADDR] = V_data2 = Data         ;/* 本地检波微较准 */
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],37) ; 
                    delay_ms(10);			
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],38) ;												 
                    break;		
                    
                case 0x00E0:
                    Modbus[ADDR] = V_data = Data          ;/* 本地检波微较准 */
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],39) ; 
                    delay_ms(10);			
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],40) ;													 
                    break;	
                case 0x00E1: 
                    Modbus[ADDR] = V_data1 = Data         ; /* 本地检波微较准 反射功率 */
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[10],41) ; 
                    delay_ms(20);			
                    I2C_EE_ByteWrite(&RJ45_Rx_buff[11],42) ;	
                    break;	
                case 0x00e3:
                    if(Data == 0x3274)
                    {
                        FLASH_If_Erase(FLASH_Sector_3,FLASH_Sector_3);
                        send_TOP(0,&iap_buf,1);
                        NVIC_SystemReset();
                    }
                    break;                  
            }
            
            ReadySend(RegisterAddr);  
            
            work_RJ45send_buff(ADDR,1,6); 
	
		}
		else if(RJ45_Rx_buff[7] == 0x03) 
        {       
            if(ADDR == 0x01)
            {
                SendVersionInfo();
                return;
            }
            work_RJ45send_buff(ADDR,Data,3);	
        }
		else if(RJ45_Rx_buff[7] == 0x10)                   //连续写
		{
            if(ADDR==0x0080 && Data==3)
            {	
                Modbus[0x0080] = Power = RJ45_Rx_buff[14];	              /* 电源 */
                Modbus[0x0081] = RF = RJ45_Rx_buff[16];                   /* 射频 */
                
                Modbus[ADDR] = Pset = ((u16)RJ45_Rx_buff[17] << 8) + RJ45_Rx_buff[18];
                if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                
                work_RJ45send_buff(ADDR,Data,16);								
            }			
		}
    }

    if(RJ45_Rx_buff[8] == 0xFF)
    {
        CalibFlag = 1;
        Calib_Tx_buff[0]=0xFF; Calib_Tx_buff[1]=RJ45_Rx_buff[9]; 
        Calib_Tx_buff[2]=RJ45_Rx_buff[10];
        Calib_Tx_buff[3]=RJ45_Rx_buff[11];	
    }		
}





/********************************************************************************/
                              /* RS232   USART1 */
/********************************************************************************/
void work_232send_buff(u16 addr,u8 len,u8 way)
{
    u16 i,crc; 
	
    if(len>120) return;
    
    while(UART1_TX_Finish){Wait_USART1_Send();}		
    UART1_DMA_TX[0] = Adr;
    UART1_DMA_TX[1] = way;	
		
    if(way==3)                                                     //读
    {
        UART1_DMA_TX[2]=len*2; 	
        for(i=0;i< len;i++)
        {
            UART1_DMA_TX[2*i+3] = Modbus[i+addr]>>8;
            UART1_DMA_TX[2*i+4] = Modbus[i+addr];
        }
        crc=crc16_modbus(UART1_DMA_TX,3+len*2);
        UART1_DMA_TX[3+len*2] = crc;
        UART1_DMA_TX[4+len*2] = crc>>8;
        USART1_Send(5+len*2);				
    }
    else if(way==6)                                              //写
    {
        UART1_DMA_TX[2]=addr>>8; UART1_DMA_TX[3]=addr;	
        
        UART1_DMA_TX[4] = Modbus[addr]>>8;
        UART1_DMA_TX[5] = Modbus[addr];				
        
        crc=crc16_modbus(UART1_DMA_TX,6);
        UART1_DMA_TX[6] = crc;
        UART1_DMA_TX[7] = crc>>8;
        USART1_Send(8);						
	}
    else if(way==16)                                             //写
    {
        UART1_DMA_TX[2]=addr>>8; UART1_DMA_TX[3]=addr;	
        UART1_DMA_TX[4] = 0;  	 UART1_DMA_TX[5] = len*2;					  
        crc=crc16_modbus(UART1_DMA_TX,6);
        UART1_DMA_TX[6] = crc;
        UART1_DMA_TX[7] = crc>>8;	
        USART1_Send(8);		
    }	
}


void remote_RS232_reseive(void)   // 01 06  00 01  00 01  CRC
{
    u16 Data,ADDR;

    if(UART1_DMA_RX[0] == Adr || UART1_DMA_RX[0]==0x00)
    {
        ADDR = ((u16)UART1_DMA_RX[2] << 8) + UART1_DMA_RX[3];           //访问地址
        Data = ((u16)UART1_DMA_RX[4] << 8) + UART1_DMA_RX[5];           //写数据 或者读取长度
        
        if(UART1_DMA_RX[1] == 0x06)                                     /* 写指令 */
        {
            switch(ADDR)
            {
                case 0x0010: 
                    if(Data)  NVIC_SystemReset();  
                    break;  
                        
                case 0x0011: 
                    Modbus[ADDR] = Adr = Data;  
                    I2C_EE_ByteWrite(&Adr,0);						
                    break;	
                    
                case 0x0012: 
                    Modbus[ADDR] = BoundNum232 = Data;	
                    I2C_EE_ByteWrite(&BoundNum232,1);							
                    break;	
                    
                case 0x0013: 
                    Modbus[ADDR] = BoundNum485 = Data;	                
                    I2C_EE_ByteWrite(&BoundNum485,2);			
                    break;	
                    
                case 0x0080: 
                    Modbus[ADDR] = Power = Data;	                /* 电源 */
                    break;							
        
                case 0x0081: 
                    Modbus[ADDR] = RF = Data;	                    /* 射频 */			 
                    break;		
                                                
                case 0x0082: 
                    Modbus[ADDR] = Pset = Data;	     
                    if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                    break; 								           
                                    
                case 0x0083: 
                    Modbus[ADDR] = Freq = Data; /* 频率 */  			
                    set_freq(Freq);		
                    break; 		
                                        
                case 0x0084: 
                    Modbus[ADDR] = Phase = 0;   /* 频率 */  			 
                    break;		
                                                
                case 0x0085: 
                    Modbus[ADDR] = 0;           /* 连续 */  			 
                    break;	
                                                
                case 0x0086: 
                    Modbus[ADDR] = Duty = 100;  /* 占空比 */  			
                    break; 
                                                
                case 0x0087: 
                    Modbus[ADDR] = Data;
                    break; 
                                                
                case 0x00C5: 
                    Modbus[ADDR] = Alarm_ZB = Data;               //驻波开关
                    I2C_EE_ByteWrite(&Alarm_ZB,23);					
                    break;	
                                        
                case 0x00C0: 
                    Modbus[ADDR] = Alarm_Pout = Data;             //过功率开关
                    I2C_EE_ByteWrite(&Alarm_Pout,24);		
                    break;		
                                        
                case 0x00C2: 
                    Modbus[ADDR] = Alarm_Pfout = Data;            //反射过功率开关
                    I2C_EE_ByteWrite(&Alarm_Pfout,25);				
                    break;		
                    
                case 0x00C7: 
                    Modbus[ADDR] = Alarm_Vswr_Value = Data;	     //驻波门限
                    I2C_EE_ByteWrite(&Alarm_Vswr_Value,26);										 
                    break;
                                        
                case 0x00C1: 
                    Modbus[ADDR] = Alarm_Pout_Value = Data;       //过功率门限
                    I2C_EE_ByteWrite(&UART1_DMA_RX[4],27); delay_ms(10);			
                    I2C_EE_ByteWrite(&UART1_DMA_RX[5],28);		
                    break;		
                                        
                case 0x00C4: 
                    Modbus[ADDR] = Alarm_Pfout = Data;            //反射过功率门限
                    I2C_EE_ByteWrite(&UART1_DMA_RX[4],29);	
                    delay_ms(10);				
                    I2C_EE_ByteWrite(&UART1_DMA_RX[5],30);		
                    break;		
                default:
                    break;	
            }								
            work_232send_buff(ADDR,1,6); 
            if(ADDR==0x0012)
            { 		
                bound = bounddata[BoundNum232];
                USART_Cmd(USART1, DISABLE);     			 
                UPUSART1_Config(bound);	 
            }	
            if(ADDR==0x0013)
            { 		
                bound = bounddata[BoundNum485];
                USART_Cmd(USART3, DISABLE);     			 
                UPUSART3_Config(bound);
            }	
        }
        else if(UART1_DMA_RX[1] == 0x03)  
            work_232send_buff(ADDR,Data,3);	

        else if(UART1_DMA_RX[1] == 0x10)                                //连续写
        {
            if(ADDR==0x0080 && Data==3)
            {	
                Modbus[0x0080] = Power = UART1_DMA_RX[8];	            /* 电源 */
                Modbus[0x0081] = RF = UART1_DMA_RX[10];                 /* 射频 */
                
                Modbus[ADDR] = Pset = ((u16)UART1_DMA_RX[11] << 8) + UART1_DMA_RX[12];
                if(Pset>MAXPOWER) Modbus[ADDR]=Pset=MAXPOWER;	 
                
                work_232send_buff(ADDR,Data,16);								
            }
            if(ADDR==0x0014 && Data==7)
            {
                local_ip2[0] = UART1_DMA_RX[7];	    local_ip2[1] = UART1_DMA_RX[8];
                local_ip2[2] = UART1_DMA_RX[9];	    local_ip2[3] = UART1_DMA_RX[10];	
                    
                subnet2[0]   = UART1_DMA_RX[11];	  subnet2[1]   = UART1_DMA_RX[12];
                subnet2[2]   = UART1_DMA_RX[13];	  subnet2[3]   = UART1_DMA_RX[14];
                    
                gateway2[0]  = UART1_DMA_RX[15];	  gateway2[1]  = UART1_DMA_RX[16];
                gateway2[2]  = UART1_DMA_RX[17];	  gateway2[3]  = UART1_DMA_RX[18];	
                
                local_port2 = ((u16)UART1_DMA_RX[19] << 8) | UART1_DMA_RX[20];
                
                Modbus[0x0014]=((u16)UART1_DMA_RX[7] << 8) | UART1_DMA_RX[7];
                Modbus[0x0015]=((u16)UART1_DMA_RX[9] << 8) | UART1_DMA_RX[9];
							
                Modbus[0x0016]=((u16)UART1_DMA_RX[11] << 8) | UART1_DMA_RX[12];
                Modbus[0x0017]=((u16)UART1_DMA_RX[13] << 8) |UART1_DMA_RX[14];
                
                Modbus[0x0018]=((u16)UART1_DMA_RX[15] << 8) | UART1_DMA_RX[16];
                Modbus[0x0019]=((u16)UART1_DMA_RX[17] << 8) | UART1_DMA_RX[18];
							
                Modbus[0x001A]=((u16)UART1_DMA_RX[19] << 8) | UART1_DMA_RX[20];
                            
                RestFlag=2;
                store_ip_24c02();
                work_232send_buff(ADDR,Data,16);		
                lcd_send_pageone();								
            }					
        }
    }	
}

