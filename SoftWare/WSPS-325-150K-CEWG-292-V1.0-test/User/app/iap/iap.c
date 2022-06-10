#include "./system.h" 


u32 iapbuf[512]; 	            //2K字节缓存 

u32 FlashRedLen = 0;          // 要接收的APP 数据总长度
u32 IAP_flag = 0;             // 读取是否执行升级标志位
u32 FlashLen = 0;             // 记录已经缓存 APP 程序得数据长度

u8 Write_Flash_Buff[2048];    // 2K大小，达到2K 存储 一次
u16 BuffLen = 0;              // 记录Write_Flash_Buff 缓冲个数



//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

/*
*  此写入  一次写入 必须是 64个字节
*/
void write_flash(u8 *Readbuff,u16 len)
{
    u16 i;
    
    u32 datalen;
    
    for(i=0; i<len; i++)           //128 个
    {
        Write_Flash_Buff[BuffLen] = Readbuff[i];
        BuffLen++;
    }
    
    if(BuffLen >= 2048)             //2K  字节
    {
        if(APP_WAY) iap_write_appbin(FLASH_APP1_ADDR + FlashLen,Write_Flash_Buff,BuffLen);
        else        iap_write_appbin(FLASH_APP0_ADDR + FlashLen,Write_Flash_Buff,BuffLen);
        FlashLen += BuffLen;
        BuffLen = 0;
    }
    else
    {
        datalen = FlashLen + BuffLen;	
        if(FlashRedLen == datalen)   //完成
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
        STMFLASH_Write(FLASH_IAP_FLAG_ADDR,Buff,2);	    //IAP  boot 升级标志		IAP_flag = 0     APP  boot 升级标志		IAP_flag = 1  
        sendbuff[0] = 'f'; sendbuff[1] = 'i';           //接收完成
        send_TOP(socket,sendbuff,2);		
        
        delay_ms(200);  		
        NVIC_SystemReset();  			
    }	
    else if(Readbuff[0] == 's' && Readbuff[1] == 'w')   //数据头
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






//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节)
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
    u32 t;
    u16 i=0;
    u32 temp;
    u32 fwaddr=appxaddr;//当前写入的地址
    u8 *dfu=appbuf;
    for(t=0;t<appsize;t+=4)
    {						   
        temp=(u32)dfu[3]<<24;   
        temp|=(u32)dfu[2]<<16;    
        temp|=(u32)dfu[1]<<8;
        temp|=(u32)dfu[0];	  
        dfu+=4;//偏移4个字节
        iapbuf[i++]=temp;	    
        if(i==512)
        {
            i=0; 
            STMFLASH_Write(fwaddr,iapbuf,512);
            fwaddr+=2048;//偏移2048  512*4=2048
        }
    } 
    if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{ 
    iapfun jump2app; 
    if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	//检查栈顶地址是否合法.
    { 
        jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
        MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        jump2app();									//跳转到APP.
    }
}		 

void STMFLASH_Unlock(void)  //解锁STM32的FLASH
{
    FLASH->KEYR=FLASH_KEY1;	//写入解锁序列.
    FLASH->KEYR=FLASH_KEY2; 
}

void STMFLASH_Lock(void)   //flash上锁
{
    FLASH->CR|=(u32)1<<31;//上锁
}


u8 STMFLASH_GetStatus(void)   //得到FLASH状态
{	
    u32 res=0;		
    res=FLASH->SR;  
    if(res&(1<<16))return 1;   		//忙
    else if(res&(1<<4))return 2;	//操作异常 
    else if(res&(1<<5))return 2;	//操作异常 
    else if(res&(1<<6))return 2;	//操作异常 
    else if(res&(1<<7))return 2;	//操作异常 
    return 0;						//没有任何状态/操作完成.
} 
//等待操作完成
//time:要延时的长短(单位:10us)
//返回值:
//0,完成
//2,操作异常
//0XFF,超时       
u8 STMFLASH_WaitDone(u32 time)
{
    u8 res;
    do
    {
        res=STMFLASH_GetStatus();
        if(res!=1)break;//非忙,无需等待了,直接退出.
        delay_us(10);
        time--;
    }while(time);
    if(time==0)res=0xff;//TIMEOUT
    return res;
}
//擦除扇区
//sectoraddr:扇区地址,范围是:0~11.
//0~3,16K扇区;4,64K扇区;5~11,128K扇区.
//返回值:执行情况
u8 STMFLASH_EraseSector(u32 sectoraddr)
{
    u8 res=0;
    res=STMFLASH_WaitDone(200000);//等待上次操作结束,最大2s    
    if(res==0)
    { 
        FLASH->CR&=~(3<<8);	//清除PSIZE原来的设置
        FLASH->CR|=2<<8;	//设置为32bit宽,确保VCC=2.7~3.6V之间!!
        FLASH->CR&=~(0X1F<<3);//清除原来的设置
        FLASH->CR|=sectoraddr<<3;//设置要擦除的扇区 
        FLASH->CR|=1<<1;	//扇区擦除 
        FLASH->CR|=1<<16;	//开始擦除		  
        res=STMFLASH_WaitDone(200000);//等待操作结束,最大2s  
        if(res!=1)			//非忙
        {
            FLASH->CR&=~(1<<1);//清除扇区擦除标志.
        }
    }
    return res;
}
//在FLASH指定地址写一个字
//faddr:指定地址(此地址必须为4的倍数!!)
//dat:要写入的数据
//返回值:0,写入成功
//    其他,写入失败
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
    u8 res;	   	    
    res=STMFLASH_WaitDone(0XFF);	 
    if(res==0)//OK
    {
        FLASH->CR&=~(3<<8);	 //清除PSIZE原来的设置
        FLASH->CR|=2<<8;	   //设置为32bit宽,确保VCC=2.7~3.6V之间!!
        FLASH->CR|=1<<0;  	 //编程使能
        *(vu32*)faddr=dat;	 //写入数据
        res=STMFLASH_WaitDone(0XFF);//等待操作完成,一个字编程,最多100us.
        if(res!=1)//操作成功
        {
            FLASH->CR&=~(1<<0);//清除PG位.
        }
    } 
    return res;
} 

u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr; 
}  

u8 STMFLASH_GetFlashSector(u32 addr) //获取某个地址所在的flash扇区  返回值:0~11,即addr所在的扇区
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
//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u8 status=0;
    u32 addrx=0;
    u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    STMFLASH_Unlock();									//解锁 
    FLASH->ACR&=~(1<<10);			//FLASH擦除期间,必须禁止数据缓存!!!
    addrx=WriteAddr;				  //写入的起始地址
    endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
    if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
    {
        while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
            {   
                status=STMFLASH_EraseSector(STMFLASH_GetFlashSector(addrx));
                if(status)break;	//发生错误了
            }
            else addrx+=4;
        } 
    }
    if(status==0)
    {
        while(WriteAddr<endaddr)//写数据
        {
            if(STMFLASH_WriteWord(WriteAddr,*pBuffer))//写入数据
            { 
                    break;	//写入异常
            }
            WriteAddr+=4;
            pBuffer++;
        } 
    }
    FLASH->ACR|=1<<10;		//FLASH擦除结束,开启数据fetch
    STMFLASH_Lock();//上锁
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移4个字节.	
    }
}


//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u32 WriteData)   	
{
    STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}

//擦除数据
void Flash_Clear(u32 WriteAddr)   	
{
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    STMFLASH_Unlock();									//解锁 
    FLASH->ACR&=~(1<<10);			//FLASH擦除期间,必须禁止数据缓存!!!
    
    STMFLASH_EraseSector(STMFLASH_GetFlashSector(WriteAddr));
    
    FLASH->ACR|=1<<10;		//FLASH擦除结束,开启数据fetch
    STMFLASH_Lock();//上锁
}
 

void copy_flash(u32 StartAddr,u32 TargetAddr,u32 len)
{
    u32 endaddr=0;	
    u32 addr=0;	
    u8 status=0;
    u32 data;
	
    if(StartAddr<STM32_FLASH_BASE||StartAddr%4)
        return;	    //非法地址
    if(TargetAddr<STM32_FLASH_BASE||TargetAddr%4)
        return;	    //非法地址
    
    FLASH_Unlock(); 	
    FLASH->ACR&=~(1<<10);		     	//FLASH擦除期间,必须禁止数据缓存!!!
    
  	endaddr = TargetAddr + len*4;	//写入的结束地址
    addr = TargetAddr;
    while(addr<endaddr)		 //对非FFFFFFFF的地方,先擦除
    {
        if(STMFLASH_ReadWord(addr)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
        {   
            status = STMFLASH_EraseSector(STMFLASH_GetFlashSector(addr));
            if(status) 
                break;	 //发生错误了
        }
        else addr+=4;
    } 
    
    if(status==0)
    {
        while(TargetAddr<endaddr)//写数据
        {
            data = STMFLASH_ReadWord(StartAddr);
            if(STMFLASH_WriteWord(TargetAddr,data))//写入数据
            { 
                break;	//写入异常
            }
            TargetAddr+=4;
            StartAddr+=4;
        } 
    }
    FLASH->ACR|=1<<10;		//FLASH擦除结束,开启数据fetch
    STMFLASH_Lock();//上锁
}









