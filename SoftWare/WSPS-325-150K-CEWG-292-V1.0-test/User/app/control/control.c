#include "system.h" 

void Ctrl_GPIO_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	  // PTT
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
	RF_OFF();
}


void Ptt_Control(void)
{
	if (RF == 1)
	{		
		RF_ON();   LED_CONTROL(RF_GRE_LED); 
	}
	else
	{
        Pset = 0;
		RF_OFF();  LED_CONTROL(RF_OFF_LED); 
	}
 
}	         
                       
void Power_Control(void)
{
//	 u8 databit;
//	 databit = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);
//	 if(databit)
//	 { 
//		 Power = 0;RF = 0;
//	 }
//	 if(AlarmError)
//	 {
//		 RF = 0; 
//	 }
//	 if (Power == 1)
//   { POWER_ON(); if(AlarmError==0) LED_CONTROL(RF_OFF_LED);}
//	 else{POWER_OFF();LED_CONTROL(RF_OFF_LED); }           
}



void Power_Adj(void)        
{
    static float dac_channe11_value=600;
    static float power_error,power_last_error,power_previous_error;      

    if(RF & Power && g_ucUdpReady && Pset)
    {
        power_error = Pset - PowerOut;

        dac_channe11_value += 0.002f*(power_error-power_last_error)
                              +0.028f*power_error
                              +0.004f*(power_error-2*power_last_error+power_previous_error);

        power_previous_error = power_last_error;         
        power_last_error = power_error;          

        if(dac_channe11_value > 4000)
        {
            dac_channe11_value = 4000;
        }
        
        if(dac_channe11_value < 0)
        {
            dac_channe11_value = 0;
        }

        DAC_SetChannel2Data(DAC_Align_12b_R,dac_channe11_value);	   		 
        DAC_SetChannel1Data(DAC_Align_12b_R,1500);
    }
    else
    {
        dac_channe11_value=600;
        power_error=0, power_last_error=0,power_previous_error=0;
        DAC_SetChannel2Data(DAC_Align_12b_R,dac_channe11_value);
        DAC_SetChannel1Data(DAC_Align_12b_R,dac_channe11_value);
    }
}


void power_pid1(void)
{
    static float value=600,value2=600;
    static float powe1=0,powe2=0;    
    static u8    error=0;

    if (RF && Power)
    {
        if(Pset>MAXPOWER)  Pset=MAXPOWER;
        powe1 = Pset - PowerOut;
					
        if(PowerOut<100) 
        {
            if(powe1>800)   powe1=800; 					
        }			       
        else if(PowerOut>=100 && PowerOut<1000) 
        {
            if(powe1>600)   powe1=600; 					
        }
        else if(PowerOut>=1000 && PowerOut<2000) 
        {
            if(powe1>300)   powe1=300; 								
        }
        else if(PowerOut>=2000 && PowerOut<3000) 
        {
            if(powe1>200)   powe1=200; 								
        }		
        else if(PowerOut>=3000 && PowerOut<4000) 
        {
            if(powe1>150)   powe1=150; 								
        }			
        else if(PowerOut>=4000 && PowerOut<5000) 
        {
            if(powe1>130)   powe1=130; 								
        }		
        else if(PowerOut>=5000) 
        {
            if(powe1>120)   powe1=120; 								
        }	
				
        if(powe1<34 && powe1>-34)           {value += 0.03f * powe1 + 0.003f*(powe1-powe2);}		
        else                                {value += 0.05f * powe1 + 0.005f*(powe1-powe2);}		

        if(value>4000)
        {
            value=4000;			
            if(powe1<34 && powe1>-34)       value2 += 0.03f * powe1 + 0.003f*(powe1-powe2);	
            else                            value2 += 0.05f * powe1 + 0.005f*(powe1-powe2);	
            
            if(value2>4000) value2=4000;					
        }

        if(value<600)	
        {
            value=600;					
            if(powe1<34 && powe1>-34)       value2 += 0.03f * powe1 + 0.003f*(powe1-powe2);	
            else                            value2 += 0.05f * powe1 + 0.005f*(powe1-powe2);
            if(value2<600) value2=600;						  
        }
				     
        DAC_SetChannel1Data(DAC_Align_12b_R,value2);				
        DAC_SetChannel2Data(DAC_Align_12b_R,value);
        powe2 = powe1; 
    }  
    else
    { 
        value=600,value2=600; powe1=0, powe2=0; 
        DAC_SetChannel1Data(DAC_Align_12b_R,value2);			
        DAC_SetChannel2Data(DAC_Align_12b_R,value);
    }
    
    if(value>=3800 && value2>=1200 && PowerOut<1 && RF && Power && Alarm_Volt)  error++; /* ¼ì²¨Æ÷Òì³£±£»¤ */
    else error=0; 
    if(error>=10)  { value = 3800; value2=1200;}
    if(error>=250) { error = 0;    if(g_ucUdpReady)  RF=0; } 		
}










