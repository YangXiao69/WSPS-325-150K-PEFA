
#include "./../W5500/utility.h"
#include "RFE001.h"

tmErrorCode_t     err = TM_OK;
SysDependency_t   sSrvSysFunc;
/***********************************************************************************/
//FREQ_DATA_MHZ    2400-2500 步进1MHZ
//POWER_DATA_dBM   1-256---?????
//REF0001_Phase_DU   0-255---0-360度  步进1.4度
//REF0001_Plus_Width  脉冲宽度  REF0001_Plus_Duty  占空比  
/***********************************************************************************/
void REF0001_Plus_OUT(unsigned int REF0001_Plus_Width,unsigned int REF0001_Plus_Duty)
{
//    Generate_PWM(REF0001_Plus_Width,REF0001_Plus_Duty);
}

void REF0001_Phase_OUT(unsigned int REF0001_Phase_DU)
{   REF0001_Phase_DU=REF0001_Phase_DU*4;
    sSrvSysFunc.sIo.Write     = UserWrittenSPIWrite;
    sSrvSysFunc.sIo.WriteRead = UserWrittenSPIWriteRead;
  	sSrvSysFunc.sTime.Wait    = UserWrittenWait;	
    Volcano_SetRFPhase(0,1.43*REF0001_Phase_DU);
}

void REF0001_ATT_OUT(unsigned int POWER_DATA_dBM)
{
    sSrvSysFunc.sIo.Write     = UserWrittenSPIWrite;
    sSrvSysFunc.sIo.WriteRead = UserWrittenSPIWriteRead;
  	sSrvSysFunc.sTime.Wait    = UserWrittenWait;	

	  Volcano_SetRFAtten(0,POWER_DATA_dBM);
}
void REF0001_FERQ_OUT(unsigned int FREQ_DATA_MHZ)
{
    sSrvSysFunc.sIo.Write     = UserWrittenSPIWrite;
    sSrvSysFunc.sIo.WriteRead = UserWrittenSPIWriteRead;
  	sSrvSysFunc.sTime.Wait    = UserWrittenWait;	

	  Volcano_SetRF(0, FREQ_DATA_MHZ*1000 , True);//True   False
}
void REF0001_INIT(unsigned int FREQ_DATA_MHZ,unsigned int POWER_DATA_dBM)
{

    sSrvSysFunc.sIo.Write             = UserWrittenSPIWrite;
    sSrvSysFunc.sIo.WriteRead         = UserWrittenSPIWriteRead;
  	sSrvSysFunc.sTime.Wait            = UserWrittenWait;	
	
    err = Volcano_Open(0, 0, &sSrvSysFunc); // master unit 0	
    if (err == TM_OK) err = Volcano_Open(1,1, &sSrvSysFunc); // slave unit 1
 
  
    /* Volcano Hardware Device init Power on reset */
    if (err == TM_OK) err = Volcano_HwInit(0, Volcano_Master);
    if (err == TM_OK) err = Volcano_HwInit(1, Volcano_Slave);
 
    /* set proper device modes */
    if (err == TM_OK) err = Volcano_SetPowerStateMode(0, Volcano_generator_daisy);//Volcano_generator_daisy

    if (err == TM_OK) err = Volcano_SetPowerStateMode(1, Volcano_slave_no_daisy);//Volcano_slave_no_daisy

 
    /* set a frequency on Device Master */
    if (err == TM_OK) err = Volcano_SetRF(0, FREQ_DATA_MHZ*1000 , False);//True False
	  if (err == TM_OK) err = Volcano_SetRF(1, 2450000 , False);//True False
	
   if (err == TM_OK)  err = Volcano_SetRFAtten(0,0);
   if (err == TM_OK)  err = Volcano_SetRFAtten(1,0);  
   /* launch syncho reset all devices master and slave(s) */
   if (err == TM_OK) err = Volcano_ResetSynchro(0);
   if (err == TM_OK) err = Volcano_ResetSynchro(1);

   /* launch Synchro on slave(s) device(s) */
   if (err == TM_OK) err = Volcano_SetSynchroSlave(1);
  
   /* launch synchro on Master device */
   if (err == TM_OK) err = Volcano_SetSynchroMaster(0); 
    
   //if (err == TM_OK)  err = Volcano_SetRFAtten(0,4000);//数控衰减器值1000-4000 
	 if (err == TM_OK)  err = Volcano_SetRFAtten(0,4000);
   if (err == TM_OK)  err = Volcano_SetRFAtten(1,0);   
}	

void RFE001_IO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure                  ; 
	                                                                                              
    RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_NSS1, ENABLE) ;
    GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_NSS1     ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		
    GPIO_Init(GPIO_REF001_NSS1,&GPIO_InitStructure)      ; 
	
    RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_NSS2, ENABLE) ;
    GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_NSS2     ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		         
    GPIO_Init(GPIO_REF001_NSS2,&GPIO_InitStructure)      ; 

	
    RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_SCK, ENABLE)  ;
    GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_SCK      ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		        
    GPIO_Init(GPIO_REF001_SCK,&GPIO_InitStructure)       ;
                                                        
    RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_MOSI, ENABLE) ;
    GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_MOSI     ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		        
    GPIO_Init(GPIO_REF001_MOSI,&GPIO_InitStructure)      ;
    
    RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_MISO, ENABLE) ;
    GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_MISO     ;                         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   
    GPIO_Init(GPIO_REF001_MISO,&GPIO_InitStructure)      ;
    
//     RCC_APB2PeriphClockCmd(RCC_GPIO_REF001_SW, ENABLE)   ;
//     GPIO_InitStructure.GPIO_Pin   = PIN_REF0001_SW       ; 
//	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
//	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		        
//     GPIO_Init(GPIO_REF001_SW,&GPIO_InitStructure)        ;

    PIN_REF0001_NSS1_ON() ;
    PIN_REF0001_SCK_OFF() ;
    PIN_REF0001_MOSI_OFF();
//     PIN_REF0001_SW_OFF(); 	   
}


/*============================================================================*/
/* FUNCTION:    UserWrittenSPIWrite:                                          */
/*                                                                            */
/* DESCRIPTION: Writes I2C data in the Tuner.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t UserWrittenSPIWrite(tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pData)
{
    // customer platform dedicated SPI write implementation
    unsigned char i = 0;
    PIN_REF0001_SCK_OFF();  
    delay_us(40)         ;
    if (tUnit == 0)     {PIN_REF0001_NSS1_OFF();}
    else if (tUnit == 1){PIN_REF0001_NSS2_OFF();}
    delay_us(40);                         
    for(i=0;i<=WriteBitsLen;i++)
    { 
        PIN_REF0001_SCK_OFF();
        delay_us(20)         ;
        if(0x80 == (pData[i/8]&0x80)) 
        {                                    
        	PIN_REF0001_MOSI_ON() ;
        }
        else 
        { 
        	PIN_REF0001_MOSI_OFF();
        }
        PIN_REF0001_SCK_OFF()     ;
        delay_us(20)              ;
        PIN_REF0001_SCK_ON()      ;
        delay_us(20)              ;
        PIN_REF0001_SCK_OFF()     ; 
        delay_us(20)              ;
        pData[i/8]=pData[i/8] << 1;
        delay_us(20)              ;
    }                                        
    PIN_REF0001_SCK_OFF()         ;  
    delay_us(40)                  ;             
    if (tUnit == 0)     {PIN_REF0001_NSS1_ON();}
    else if (tUnit == 1){PIN_REF0001_NSS2_ON();}
    delay_us(40); 	
    return TM_OK;
}

/*============================================================================*/
/* FUNCTION:    UserWrittenSPIWriteRead                                       */
/*                                                                            */
/* DESCRIPTION: Read I2C data in the Tuner.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*================================================================ ============*/
tmErrorCode_t    UserWrittenSPIWriteRead(tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pDataWrite, UInt32 ReadBitsLen, UInt8* pDataRead)
{
    // customer platform dedicated SPI writeRead implementation
    unsigned char i = 0;
    PIN_REF0001_SCK_OFF();  
    delay_us(40)         ;             
    if (tUnit == 0)     {PIN_REF0001_NSS1_OFF();}
    else if (tUnit == 1){PIN_REF0001_NSS2_OFF();}
    delay_us(40)         ;                         
    for(i=0;i<=WriteBitsLen;i++)
    { 
        PIN_REF0001_SCK_OFF();
        delay_us(20)         ;
        if(0x80 == (pDataWrite[i/8]&0x80)) 
        {                                    
        	PIN_REF0001_MOSI_ON()           ;
        }                                   
        else                                
        {                                   
        	PIN_REF0001_MOSI_OFF()          ;
        }                                   
        PIN_REF0001_SCK_OFF()               ;
        delay_us(20)                        ;
        PIN_REF0001_SCK_ON()                ;
        delay_us(20)                        ;
        PIN_REF0001_SCK_OFF()               ; 
        delay_us(20)                        ;
        pDataWrite[i/8]=pDataWrite[i/8] << 1;
        delay_us(20)                        ;
    }

    for(i=0;i<=ReadBitsLen;i++)
    {         
        pDataRead[i/8]=pDataRead[i/8] << 1;
        PIN_REF0001_SCK_OFF()             ;
        delay_us(20)                      ;
        PIN_REF0001_SCK_ON()              ;
        delay_us(20)                      ;
        if (PIN_REF0001_MISO_IN()==1){pDataRead[i/8]++;}                
    }

    PIN_REF0001_SCK_OFF();    
    delay_us(40)         ;             
    if (tUnit == 0)     {PIN_REF0001_NSS1_ON();}
    else if (tUnit == 1){PIN_REF0001_NSS2_ON();}
    delay_us(40);
    return TM_OK;
}

/*============================================================================*/
/* FUNCTION:    UserWrittenWait:                                              */
/*                                                                            */
/* DESCRIPTION: This function waits for requested time.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t UserWrittenWait(UInt32 tms)
{
		// customer platform dedicated wait in Milliseconds 
    delay_us(1000);
    return TM_OK;
}
