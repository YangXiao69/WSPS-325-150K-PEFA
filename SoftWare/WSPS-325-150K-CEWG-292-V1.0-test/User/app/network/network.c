#include "system.h"





uint8_t  g_ucUdpReady = 0; 

uint8_t RJ45_Rx_buff[64] = {0};







void Write_ModbusReg(void)
{
    Modbus[0x0011] = Adr;  		
	Modbus[0x0012] = BoundNum232 ;								
    Modbus[0x0013] = BoundNum485 ;	
	
    Modbus[0x0080] = Power;                 /* 电源 */								
    Modbus[0x0081] = RF   ;                 /* 射频 */	
    Modbus[0x0082] = Pset ;                 /* 功率 */
    Modbus[0x0083] = Freq	;               /* 频率 */
	
    Modbus[0x00A1] = PowerOut  ; 
    Modbus[0x00A2] = PowerR    ; 
    Modbus[0x00A3] = PowerZB   ; 

    Modbus[0x00AF] = Power   ; 
    Modbus[0x00B0] = RF      ; 
    Modbus[0x00B1] = Pset    ;
    Modbus[0x00B2] = PfoutOut; 
 	
	Modbus[0x00B4] = FlowTemp;
	Modbus[0x00B5] = Flow;	
    Modbus[0x00B6] = pfout_temp;
}














void UDP_SendSettings(void)
{
    uint8_t  ucSetTxBuff[14]  = {0};
    uint16_t send_settings[2] = {0}; 
    
    ucSetTxBuff[0] = Power;	ucSetTxBuff[1] = RF; 
    ucSetTxBuff[2] = 0;     ucSetTxBuff[3] = 0; 
    ucSetTxBuff[4] = 0;     ucSetTxBuff[5] = 0;  
    ucSetTxBuff[6] = 0;     ucSetTxBuff[7] = 0; 	
    
    ucSetTxBuff[8]  = Alarm_Temp2_Value>>8;   ucSetTxBuff[9]  = Alarm_Temp2_Value;  
    ucSetTxBuff[10] = Alarm_Curr2_Value;      ucSetTxBuff[11] = Alarm_Vswr2_Value; 		
    ucSetTxBuff[12] = Alarm_Pout2_Value>>8;   ucSetTxBuff[13] = Alarm_Pout2_Value; 
    
    if(!CirBuff_IsEmpty(&cp))
    {
        CirBuff_Pop(&cp, send_settings);
        
        switch(send_settings[0])
        {
            case 0x0080:
                ucSetTxBuff[0] = send_settings[1];
                break;
            case 0x0081:
                ucSetTxBuff[1] = send_settings[1];
                break;        
            case 0x0117:
                ucSetTxBuff[8] = send_settings[1] >> 8;
                ucSetTxBuff[9] = send_settings[1] & 0xFF;
                break;
            case 0x011B:
                ucSetTxBuff[10] = send_settings[1];
                break;  
            case 0x0115:
                ucSetTxBuff[11] = send_settings[1];
                break;
            case 0x0111:
                ucSetTxBuff[12] = send_settings[1] >> 8;
                ucSetTxBuff[13] = send_settings[1] & 0xFF;
                break;
            default:
                break;
        }
        sendto(0,ucSetTxBuff, 14, remote_ip,remote_port);    
    }
    
    
    if(CalibFlag && (CirBuff_IsEmpty(&cp)))
    {
        CalibFlag = 0;
        sendto(0,Calib_Tx_buff, 4, remote_ip,remote_port); 
    }

}




void UDP_Service(void)                                                      /* 接收接口板网口服务函数 */
{   
    uint8_t status = 0;
    uint8_t n = 0;
    uint16_t len = 0;
	uint8_t Reseve_IP[4] = {0};
	uint8_t Tx_buff[48] = {0};
    
    uint8_t ucaUdpRescv[64] = {0};
    static uint16_t dataflag[25] = {0};                                      /* 收到数据标志，收到数据则清零，没收到则计数加，多次没清零则表示连接断开 */
    
    status = IINCHIP_READ(Sn_SR(0));
	
    switch(status)                                                       /*获取socket的状态*/
	{
		case SOCK_CLOSED:                                                  /*socket处于关闭状态*/
            socket(0,Sn_MR_UDP,local_port,0);                             /*初始化socket*/
            break;
		
		case SOCK_UDP:                                                     /*socket初始化完成*/
            if(IINCHIP_READ(Sn_IR(0)) & Sn_IR_RECV)
                IINCHIP_WRITE(Sn_IR(0), Sn_IR_RECV);                          /*清接收中断*/	

            dataflag[24]++;	  
            if(dataflag[24]>=550)
            {
                dataflag[24] = 0; 
                Tx_buff[0] = Power;					 
                Tx_buff[1] = RF; 
                
//                Tx_buff[2] = Alarm_Volt;  
//                Tx_buff[3] = Alarm_Temp; 
//                Tx_buff[4] = Alarm_Curr; 		 
//                Tx_buff[5] = Alarm_Pout;  
//                Tx_buff[6] = Alarm_Pfout; 
//                Tx_buff[7] = Alarm_ZB; 

                Tx_buff[2] = 0;  
                Tx_buff[3] = 0; 
                Tx_buff[4] = 0; 		 
                Tx_buff[5] = 0;  
                Tx_buff[6] = 0; 
                Tx_buff[7] = 0; 

                Tx_buff[8] = Alarm_Temp2_Value>>8; 
                Tx_buff[9] = Alarm_Temp2_Value;  
                Tx_buff[10] = Alarm_Curr2_Value; 
                Tx_buff[11] = Alarm_Vswr2_Value; 		
                Tx_buff[12] = Alarm_Pout2_Value>>8; 
                Tx_buff[13] = Alarm_Pout2_Value; 						 
                
                sendto(0,Tx_buff, 14, remote_ip,remote_port); 
            }
            
            UDP_SendSettings();
//            if(AlarmFlag==0)
//            {		 
                len = getSn_RX_RSR(0);
                if(len>0)                                                      /*接收到数据*/
                {
                    g_ucUdpReady = 1;
                    recvfrom(0,ucaUdpRescv, len, Reseve_IP,&remote_port);           /*W5500接收计算机发送来的数据*/	 
                    LCD_DisPA(Reseve_IP[3],ucaUdpRescv);                               /* 将受到的数据发送到显示屏 */
                    work_data_stor(Reseve_IP[3]-101, ucaUdpRescv);                  /* 将数据保存到数组等远程读取 */		 
                    dataflag[Reseve_IP[3]-101] = 0;                             /* 收到数据清除未收到数据标志位 */      
                }
                else                                                       /* 没有接收到数据 */
                {
                    for(n=0; n<18; n++)
                    {
                        dataflag[n]++;
                        if(dataflag[n]>=1000)
                        {										 
                            work_data_clean(n);
                            dataflag[n]=0;
//                            clear_lcd_display(n);											
                        }
                    } 						
                }
			 break;
	  }		
}




void TCP_TopServer(void)
{
    uint8_t status = 0;
	uint16_t len = 0;  
	
    if(RestFlag == 2)
    {
        RestFlag = 0; 
	  	close_TOP(0);	
		close_TOP(1);	
		mac2[5] = local_ip2[3];

		wiz_write_buf_top(SHAR0, mac2, 6);          /*将IP配置信息写入W5500相应寄存器*/	             
		wiz_write_buf_top(SUBR0, subnet2, 4);	
		wiz_write_buf_top(GAR0,  gateway2, 4);
		wiz_write_buf_top(SIPR0, local_ip2, 4); 	
    }	
	
	status=IINCHIP_READ_TOP(Sn_SR(0));
    
	switch(status)	                                              /*获取socket的状态*/
	{
		case SOCK_CLOSED:													                  /*socket处于关闭状态*/
			 socket_top(0 ,Sn_MR_TCP,local_port2,Sn_MR_ND);         /*打开socket*/
		     break;     
    
		case SOCK_INIT:														                  /*socket已初始化状态*/
			 listen_TOP(0);												                  /*socket建立监听*/
		     break;
		
		case SOCK_ESTABLISHED:												              /*socket处于连接建立状态*/
            if(IINCHIP_READ_TOP(Sn_IR(0)) & Sn_IR_CON)
            {
                IINCHIP_WRITE_TOP(Sn_IR(0), Sn_IR_CON);				/*清除接收中断标志位*/
            }
            len=getSn_RX_RSR_TOP(0);		  	                        /*定义len为已接收数据的长度*/
            if(len>0)
            {
                if(len > 64)  len = 64;
                recv_TOP(0,RJ45_Rx_buff,len); 							   		  /*接收来自Server的数据*/
                recvfrom_top(0,RJ45_Rx_buff, len, remote_ip2,&remote_port2);          /*W5500接收计算机发送来的数据*/
                remote_RJ45_reseive();
                LED_COMM_COML();
			}
            break;
		
		case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
			close_TOP(0);
            break;
	}

}







