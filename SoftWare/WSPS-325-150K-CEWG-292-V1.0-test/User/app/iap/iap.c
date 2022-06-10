#include "./system.h" 


u32 iapbuf[512]; 	            //2K�ֽڻ��� 

u32 FlashRedLen = 0;          // Ҫ���յ�APP �����ܳ���
u32 IAP_flag = 0;             // ��ȡ�Ƿ�ִ��������־λ
u32 FlashLen = 0;             // ��¼�Ѿ����� APP ��������ݳ���

u8 Write_Flash_Buff[2048];    // 2K��С���ﵽ2K �洢 һ��
u16 BuffLen = 0;              // ��¼Write_Flash_Buff �������



//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

/*
*  ��д��  һ��д�� ������ 64���ֽ�
*/
void write_flash(u8 *Readbuff,u16 len)
{
    u16 i;
    
    u32 datalen;
    
    for(i=0; i<len; i++)           //128 ��
    {
        Write_Flash_Buff[BuffLen] = Readbuff[i];
        BuffLen++;
    }
    
    if(BuffLen >= 2048)             //2K  �ֽ�
    {
        if(APP_WAY) iap_write_appbin(FLASH_APP1_ADDR + FlashLen,Write_Flash_Buff,BuffLen);
        else        iap_write_appbin(FLASH_APP0_ADDR + FlashLen,Write_Flash_Buff,BuffLen);
        FlashLen += BuffLen;
        BuffLen = 0;
    }
    else
    {
        datalen = FlashLen + BuffLen;	
        if(FlashRedLen == datalen)   //���
        {
            if(APP_WAY) iap_write_appbin(FLASH_APP1_ADDR + FlashLen,Write_Flash_Buff,BuffLen);
            else        iap_write_appbin(FLASH_APP0_ADDR + FlashLen,Write_Flash_Buff,BuffLen);  
        }
    }
}

void IAP_RJ45_reseive(u8 socket,u8 *Readbuff,u16 len)
{
    u8 i;
    u8 sendbuff[16];
    u8 pbuff[160];
    u32 Buff[2];
    
    if(Readbuff[0]=='r' && Readbuff[1]=='e' && Readbuff[2]=='a' && Readbuff[3]=='d' && Readbuff[4]=='y')        //ready
    {
        FlashRedLen = ((u32)Readbuff[5]<<24) | ((u32)Readbuff[6]<<16) | ((u32)Readbuff[7]<<8) | Readbuff[8];
        BuffLen = 0;
        FlashLen= 0;
        sendbuff[0] = 'r'; sendbuff[1] = 'e'; 
        send_TOP(socket,sendbuff,2);							  
    }
    else if(Readbuff[0]=='f' && Readbuff[1]=='i' && Readbuff[2]=='n' && Readbuff[3]=='i' && Readbuff[4]=='s')   //Finish
    {  
        IAP_flag = APP_WAY;   
        Buff[0] = IAP_flag;
        Buff[1] = FlashRedLen;
        STMFLASH_Write(FLASH_IAP_FLAG_ADDR,Buff,2);	    //IAP  boot ������־		IAP_flag = 0     APP  boot ������־		IAP_flag = 1  
        sendbuff[0] = 'f'; sendbuff[1] = 'i';           //�������
        send_TOP(socket,sendbuff,2);		
        
        delay_ms(200);  		
        NVIC_SystemReset();  			
    }	
    else if(Readbuff[0] == 's' && Readbuff[1] == 'w')   //����ͷ
    {
        for(i=0; i<len-2; i++)  
        {
            pbuff[i] =  Readbuff[2+i];
        }
        write_flash(pbuff,len-2);
        sendbuff[0] = 'o'; sendbuff[1] = 'k'; 
        send_TOP(socket,sendbuff,2);								
    }
}






//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�)
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
    u32 t;
    u16 i=0;
    u32 temp;
    u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
    u8 *dfu=appbuf;
    for(t=0;t<appsize;t+=4)
    {						   
        temp=(u32)dfu[3]<<24;   
        temp|=(u32)dfu[2]<<16;    
        temp|=(u32)dfu[1]<<8;
        temp|=(u32)dfu[0];	  
        dfu+=4;//ƫ��4���ֽ�
        iapbuf[i++]=temp;	    
        if(i==512)
        {
            i=0; 
            STMFLASH_Write(fwaddr,iapbuf,512);
            fwaddr+=2048;//ƫ��2048  512*4=2048
        }
    } 
    if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{ 
    iapfun jump2app; 
    if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
    { 
        jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
        MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        jump2app();									//��ת��APP.
    }
}		 

void STMFLASH_Unlock(void)  //����STM32��FLASH
{
    FLASH->KEYR=FLASH_KEY1;	//д���������.
    FLASH->KEYR=FLASH_KEY2; 
}

void STMFLASH_Lock(void)   //flash����
{
    FLASH->CR|=(u32)1<<31;//����
}


u8 STMFLASH_GetStatus(void)   //�õ�FLASH״̬
{	
    u32 res=0;		
    res=FLASH->SR;  
    if(res&(1<<16))return 1;   		//æ
    else if(res&(1<<4))return 2;	//�����쳣 
    else if(res&(1<<5))return 2;	//�����쳣 
    else if(res&(1<<6))return 2;	//�����쳣 
    else if(res&(1<<7))return 2;	//�����쳣 
    return 0;						//û���κ�״̬/�������.
} 
//�ȴ��������
//time:Ҫ��ʱ�ĳ���(��λ:10us)
//����ֵ:
//0,���
//2,�����쳣
//0XFF,��ʱ       
u8 STMFLASH_WaitDone(u32 time)
{
    u8 res;
    do
    {
        res=STMFLASH_GetStatus();
        if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
        delay_us(10);
        time--;
    }while(time);
    if(time==0)res=0xff;//TIMEOUT
    return res;
}
//��������
//sectoraddr:������ַ,��Χ��:0~11.
//0~3,16K����;4,64K����;5~11,128K����.
//����ֵ:ִ�����
u8 STMFLASH_EraseSector(u32 sectoraddr)
{
    u8 res=0;
    res=STMFLASH_WaitDone(200000);//�ȴ��ϴβ�������,���2s    
    if(res==0)
    { 
        FLASH->CR&=~(3<<8);	//���PSIZEԭ��������
        FLASH->CR|=2<<8;	//����Ϊ32bit��,ȷ��VCC=2.7~3.6V֮��!!
        FLASH->CR&=~(0X1F<<3);//���ԭ��������
        FLASH->CR|=sectoraddr<<3;//����Ҫ���������� 
        FLASH->CR|=1<<1;	//�������� 
        FLASH->CR|=1<<16;	//��ʼ����		  
        res=STMFLASH_WaitDone(200000);//�ȴ���������,���2s  
        if(res!=1)			//��æ
        {
            FLASH->CR&=~(1<<1);//�������������־.
        }
    }
    return res;
}
//��FLASHָ����ַдһ����
//faddr:ָ����ַ(�˵�ַ����Ϊ4�ı���!!)
//dat:Ҫд�������
//����ֵ:0,д��ɹ�
//    ����,д��ʧ��
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
    u8 res;	   	    
    res=STMFLASH_WaitDone(0XFF);	 
    if(res==0)//OK
    {
        FLASH->CR&=~(3<<8);	 //���PSIZEԭ��������
        FLASH->CR|=2<<8;	   //����Ϊ32bit��,ȷ��VCC=2.7~3.6V֮��!!
        FLASH->CR|=1<<0;  	 //���ʹ��
        *(vu32*)faddr=dat;	 //д������
        res=STMFLASH_WaitDone(0XFF);//�ȴ��������,һ���ֱ��,���100us.
        if(res!=1)//�����ɹ�
        {
            FLASH->CR&=~(1<<0);//���PGλ.
        }
    } 
    return res;
} 

u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr; 
}  

u8 STMFLASH_GetFlashSector(u32 addr) //��ȡĳ����ַ���ڵ�flash����  ����ֵ:0~11,��addr���ڵ�����
{
    if(addr<ADDR_FLASH_SECTOR_1)return 0;
    else if(addr<ADDR_FLASH_SECTOR_2)return 1;
    else if(addr<ADDR_FLASH_SECTOR_3)return 2;
    else if(addr<ADDR_FLASH_SECTOR_4)return 3;
    else if(addr<ADDR_FLASH_SECTOR_5)return 4;
    else if(addr<ADDR_FLASH_SECTOR_6)return 5;
    else if(addr<ADDR_FLASH_SECTOR_7)return 6;
    else if(addr<ADDR_FLASH_SECTOR_8)return 7;
    else if(addr<ADDR_FLASH_SECTOR_9)return 8;
    else if(addr<ADDR_FLASH_SECTOR_10)return 9;
    else if(addr<ADDR_FLASH_SECTOR_11)return 10; 
    else if(addr<ADDR_FLASH_SECTOR_12)return 11; 
    else if(addr<ADDR_FLASH_SECTOR_13)return 12; 
    else if(addr<ADDR_FLASH_SECTOR_14)return 13; 
    else if(addr<ADDR_FLASH_SECTOR_15)return 14; 
    else if(addr<ADDR_FLASH_SECTOR_16)return 15; 
    else if(addr<ADDR_FLASH_SECTOR_17)return 16; 
    else if(addr<ADDR_FLASH_SECTOR_18)return 17; 
    else if(addr<ADDR_FLASH_SECTOR_19)return 18; 
    else if(addr<ADDR_FLASH_SECTOR_20)return 19; 
    else if(addr<ADDR_FLASH_SECTOR_21)return 20; 
    else if(addr<ADDR_FLASH_SECTOR_22)return 21; 
    else if(addr<ADDR_FLASH_SECTOR_23)return 22;  
    return 23;	
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u8 status=0;
    u32 addrx=0;
    u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
    STMFLASH_Unlock();									//���� 
    FLASH->ACR&=~(1<<10);			//FLASH�����ڼ�,�����ֹ���ݻ���!!!
    addrx=WriteAddr;				  //д�����ʼ��ַ
    endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
    if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
    {
        while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
        {
            if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
            {   
                status=STMFLASH_EraseSector(STMFLASH_GetFlashSector(addrx));
                if(status)break;	//����������
            }
            else addrx+=4;
        } 
    }
    if(status==0)
    {
        while(WriteAddr<endaddr)//д����
        {
            if(STMFLASH_WriteWord(WriteAddr,*pBuffer))//д������
            { 
                    break;	//д���쳣
            }
            WriteAddr+=4;
            pBuffer++;
        } 
    }
    FLASH->ACR|=1<<10;		//FLASH��������,��������fetch
    STMFLASH_Lock();//����
} 

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
        ReadAddr+=4;//ƫ��4���ֽ�.	
    }
}


//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u32 WriteData)   	
{
    STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}

//��������
void Flash_Clear(u32 WriteAddr)   	
{
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
    STMFLASH_Unlock();									//���� 
    FLASH->ACR&=~(1<<10);			//FLASH�����ڼ�,�����ֹ���ݻ���!!!
    
    STMFLASH_EraseSector(STMFLASH_GetFlashSector(WriteAddr));
    
    FLASH->ACR|=1<<10;		//FLASH��������,��������fetch
    STMFLASH_Lock();//����
}
 

void copy_flash(u32 StartAddr,u32 TargetAddr,u32 len)
{
    u32 endaddr=0;	
    u32 addr=0;	
    u8 status=0;
    u32 data;
	
    if(StartAddr<STM32_FLASH_BASE||StartAddr%4)
        return;	    //�Ƿ���ַ
    if(TargetAddr<STM32_FLASH_BASE||TargetAddr%4)
        return;	    //�Ƿ���ַ
    
    FLASH_Unlock(); 	
    FLASH->ACR&=~(1<<10);		     	//FLASH�����ڼ�,�����ֹ���ݻ���!!!
    
  	endaddr = TargetAddr + len*4;	//д��Ľ�����ַ
    addr = TargetAddr;
    while(addr<endaddr)		 //�Է�FFFFFFFF�ĵط�,�Ȳ���
    {
        if(STMFLASH_ReadWord(addr)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
        {   
            status = STMFLASH_EraseSector(STMFLASH_GetFlashSector(addr));
            if(status) 
                break;	 //����������
        }
        else addr+=4;
    } 
    
    if(status==0)
    {
        while(TargetAddr<endaddr)//д����
        {
            data = STMFLASH_ReadWord(StartAddr);
            if(STMFLASH_WriteWord(TargetAddr,data))//д������
            { 
                break;	//д���쳣
            }
            TargetAddr+=4;
            StartAddr+=4;
        } 
    }
    FLASH->ACR|=1<<10;		//FLASH��������,��������fetch
    STMFLASH_Lock();//����
}









