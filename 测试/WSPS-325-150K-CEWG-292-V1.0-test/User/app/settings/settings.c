#include "system.h" 


T_CirBuffer cp = {0, 0, 0, 0, {0}};



void store_ip_24c02(void)
{
    u8 WriteData[14];		
    u8 i;
	
    for(i = 0; i < 4; i++)
    {
        WriteData[i]   =  local_ip2[i];   
        WriteData[8+i] =  subnet2[i];    				
        WriteData[4+i] =  gateway2[i];		
    }	  
    WriteData[12] =  local_port2>>8;  		
    WriteData[13] =  local_port2;  
		
    Modbus[0x0014] = ((u16)local_ip2[0] << 8) | local_ip2[1];
    Modbus[0x0015] = ((u16)local_ip2[2] << 8) | local_ip2[3];

    Modbus[0x0016] = ((u16)subnet2[0] << 8) | subnet2[1];
    Modbus[0x0017] = ((u16)subnet2[2] << 8) | subnet2[3];

    Modbus[0x0018] = ((u16)gateway2[0] << 8) | gateway2[1];
    Modbus[0x0019] = ((u16)gateway2[2] << 8) | gateway2[3];
		
    Modbus[0x001A] = local_port2;
		
	I2C_EE_BufferWrite(WriteData,3,14);		
}

void read_ip_24c02(void)
{
    u8 Readdata[14];		
	u8 i;
		
	I2C_EE_BufferRead(Readdata,3,14);			
    for(i = 0; i < 4; i++)
	{
        local_ip2[i] = Readdata[i];    
        subnet2[i]   = Readdata[8+i];   			
        gateway2[i]  = Readdata[4+i];     		
	}
	local_port2 = Readdata[12];
	local_port2 = (local_port2<<8) + Readdata[13];	


    if (local_ip2[0] == 0xFF){local_ip2[0] = 192;}
    if (local_ip2[1] == 0xFF){local_ip2[1] = 168;}
    if (local_ip2[2] == 0xFF){local_ip2[2] = 1  ;}
    if (local_ip2[3] == 0xFF){local_ip2[3] = 105;}    
    
    if (gateway2[0] == 0xFF){gateway2[0] = 192;}
    if (gateway2[1] == 0xFF){gateway2[1] = 168;}
    if (gateway2[2] == 0xFF){gateway2[2] = 1  ;}
    if (gateway2[3] == 0xFF){gateway2[3] = 1  ;}  

    if (subnet2[0] == 0xFF){subnet2[0] = 0xFF;}
    if (subnet2[1] == 0xFF){subnet2[1] = 0xFF;}
    if (subnet2[2] == 0xFF){subnet2[2] = 0xFF;}
    if (subnet2[3] == 0xFF){subnet2[3] = 0   ;}      

    if (local_port2 == 0xFFFF){local_port2 = 5000;} 	
}

void read_from_24c02(void)
{
    u8 Readdata[55];		
    u8 i;

    I2C_EE_BufferRead(Readdata,0,55);		

    Adr = Readdata[0];
    BoundNum232 = Readdata[1];
    BoundNum485 = Readdata[2];
	
    for(i = 0; i < 4; i++)               
    {
        local_ip2[i] = Readdata[i+3];        // 3 4 5 6
        gateway2[i]  = Readdata[i+7];        // 7 8 9 10
        subnet2[i]   = Readdata[i+11];    	 //	11 12 13 14 
    }
    local_port2 = Readdata[15];
    local_port2 = (local_port2<<8) + Readdata[16]; 
    
    if (local_ip2[0] == 0xFF){local_ip2[0] = 192;}
    if (local_ip2[1] == 0xFF){local_ip2[1] = 168;}
    if (local_ip2[2] == 0xFF){local_ip2[2] = 1  ;}
    if (local_ip2[3] == 0xFF){local_ip2[3] = 105;}    
    
    if (gateway2[0] == 0xFF){gateway2[0] = 192;}
    if (gateway2[1] == 0xFF){gateway2[1] = 168;}
    if (gateway2[2] == 0xFF){gateway2[2] = 1  ;}
    if (gateway2[3] == 0xFF){gateway2[3] = 1  ;}  

    if (subnet2[0] == 0xFF){subnet2[0] = 0xFF;}
    if (subnet2[1] == 0xFF){subnet2[1] = 0xFF;}
    if (subnet2[2] == 0xFF){subnet2[2] = 0xFF;}
    if (subnet2[3] == 0xFF){subnet2[3] = 0   ;}      

    if (local_port2 == 0xFFFF){local_port2 = 5000;}    
		
    Alarm_Volt  = Readdata[20];
    Alarm_Temp  = Readdata[21];								 
    Alarm_Curr  = Readdata[22];
    Alarm_ZB    = Readdata[23];								 
    Alarm_Pout  = Readdata[24];
    Alarm_Pfout = Readdata[25];			
		
    Alarm_Vswr_Value  = Readdata[26];	
    Alarm_Pout_Value  = ((u16)Readdata[27]<<8) + Readdata[28];
    Alarm_Pfout_Value = ((u16)Readdata[29]<<8) + Readdata[30];

    Alarm_Temp2_Value = ((u16)Readdata[31]<<8) + Readdata[32]; 
    Alarm_Curr2_Value = Readdata[33];	
    Alarm_Vswr2_Value = Readdata[34];	
    Alarm_Pout2_Value = ((u16)Readdata[35]<<8) + Readdata[36]; 
    
    Modbus[0xE2] = V_data2 = ((u16)Readdata[37]<<8) + Readdata[38]; 
    Modbus[0xE0] = V_data  = ((u16)Readdata[39]<<8) + Readdata[40]; 	
    Modbus[0xE1] = V_data1 = ((u16)Readdata[41]<<8) + Readdata[42]; 

    Modbus[0x0014] = ((u16)local_ip2[0] << 8) | local_ip2[1];
    Modbus[0x0015] = ((u16)local_ip2[2] << 8) | local_ip2[3];

    Modbus[0x0016] = ((u16)subnet2[0] << 8) | subnet2[1];
    Modbus[0x0017] = ((u16)subnet2[2] << 8) | subnet2[3];

    Modbus[0x0018] = ((u16)gateway2[0] << 8) | gateway2[1];
    Modbus[0x0019] = ((u16)gateway2[2] << 8) | gateway2[3];
		
    Modbus[0x001A] = local_port2;

    Modbus[0x00C0] = Alarm_Pout;		
    Modbus[0x00C1] = Alarm_Pout_Value;	
    
    Modbus[0x00C2] = Alarm_Pfout;
    Modbus[0x00C4] = Alarm_Pfout_Value;
		
    Modbus[0x00C5] = Alarm_ZB;
    Modbus[0x00C7] = Alarm_Vswr_Value;	
    
    Modbus[0x00C8] = Alarm_Temp;    
    Modbus[0x00CA] = Alarm_Volt;
	    
    Modbus[0x00CC] = Alarm_Curr;    

    Modbus[0x00D0] = Alarm_FlowTemp_Value = ((u16)Readdata[51]<<8) + Readdata[52];
    Modbus[0x00D1] = Alarm_Flow_Value     = ((u16)Readdata[53]<<8) + Readdata[54];
    
//    bound = bounddata[BoundNum232];		
//    USART1_Config(bound);
//    
//    bound = bounddata[BoundNum485];		
//    USART3_Config(bound);
}


void ReadySend(uint16_t modbusreg)
{

    uint16_t usersettings[2] = {0};
    
    if(modbusreg)
    {
        usersettings[0] = modbusreg;
        usersettings[1] = Modbus[modbusreg];
    
        CirBuff_Push(&cp, usersettings);
    } 

    RegisterAddr = 0;
}








void CirBuff_Init(T_CirBuffer *cirbuff)
{
   cirbuff->size = CIRBUFF_SIZE;
   cirbuff->head = 0;
   cirbuff->tail = 0;

   cirbuff->tag = 0;
}



int8_t CirBuff_Push(T_CirBuffer *cirbuff,uint16_t *data)
{
//  print_cirq(cirbuff);

    if(CirBuff_IsFull(cirbuff))
    {

//     printf("ringq is full\n");
        return -1;
    }

    cirbuff->buff[cirbuff->tail] = *data;

    cirbuff->tail = (cirbuff->tail + 1) % cirbuff->size ;

    if(cirbuff->tail == cirbuff->head)
    {
        cirbuff->tag = 1;
    }

    cirbuff->buff[cirbuff->tail] = *(data + 1);

    cirbuff->tail = (cirbuff->tail + 1) % cirbuff->size ;    
    
    if(cirbuff->tail == cirbuff->head)
    {
        cirbuff->tag = 1;
    }    
    
    return cirbuff->tag;  
}

int8_t CirBuff_Pop(T_CirBuffer *cirbuff, uint16_t *data)
{
//    print_cirq(cirbuff);
    if(CirBuff_IsEmpty(cirbuff))
    {

//        printf("ringq is empty\n");
        return -1;
    }

    *data = cirbuff->buff[cirbuff->head];

    cirbuff->head = (cirbuff->head + 1) % cirbuff->size ;


    if(cirbuff->tail == cirbuff->head)
    {
       cirbuff->tag = 0;
    }   

    *(data +1) = cirbuff->buff[cirbuff->head];

    cirbuff->head = (cirbuff->head + 1) % cirbuff->size ;


   if(cirbuff->tail == cirbuff->head)
    {
       cirbuff->tag = 0;
    } 
    
    return cirbuff->tag ;
}



uint8_t CirBuff_IsEmpty(T_CirBuffer *cirbuff)
{
    if((cirbuff->head == cirbuff->tail) && (cirbuff->tag == 0))
    {
        return 1;
    }
    return 0;
}



uint8_t CirBuff_IsFull(T_CirBuffer *cirbuff)
{
    if((cirbuff->head == cirbuff->tail) && (cirbuff->tag == 1))
    {
        return 1;
    }
    return 0;
}












