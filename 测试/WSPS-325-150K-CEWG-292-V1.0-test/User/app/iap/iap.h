#ifndef  __IAP_H
#define  __IAP_H




#include "stm32f4xx.h" 


#define APP_WAY  1   	 /* APP_WAY 选择升级方式，1 为 运行程序升级
                        *                      0 为 Boot程序升级
                       */
				
#define STM32_FLASH_BASE  0x08000000   	//STM32 FLASH的起始地址

#define FLASH_IAP_FLAG_ADDR	0x0800C000  //程序升级标志位存放地址(存放在FLASH) 
#define FLASH_APP0_ADDR		  0x08010000  	//应用程序起始地址(存放在FLASH)  保留0X08000000~0x08004000空间为Bootloader使用(共32KB)	 
#define FLASH_APP1_ADDR    	0x08040000  	//接收程序暂存地址(存放在FLASH)  


extern u32 IAP_flag ;             // 读取是否执行升级标志位
extern u32 FlashLen ;             // APP 程序得数据长度记录

typedef  void (*iapfun)(void);				  //定义一个函数类型的参数.   
void iap_load_app(u32 appxaddr);			  //跳转到APP程序执行
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin


//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes     
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes   48
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes   16
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes   64
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  128
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  128
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  128

#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes 
#define ADDR_FLASH_SECTOR_12	  ((u32)0x08100000) 	//扇区12起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_13	  ((u32)0x08104000) 	//扇区13起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_14    ((u32)0x08108000) 	//扇区14起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_15	  ((u32)0x0810C000) 	//扇区15起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_16    ((u32)0x08110000) 	//扇区16起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_17	  ((u32)0x08120000) 	//扇区17起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_18	  ((u32)0x08140000) 	//扇区18起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_19	  ((u32)0x08160000) 	//扇区19起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_20    ((u32)0x08180000) 	//扇区20起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_21	  ((u32)0x081A0000) 	//扇区21起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_22    ((u32)0x081C0000) 	//扇区22起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_23    ((u32)0x081E0000) 	//扇区23起始地址, 128 Kbytes   

void Flash_Clear(u32 WriteAddr)  ;
void STMFLASH_Unlock(void);					      //FLASH解锁
void STMFLASH_Lock(void);				  	      //FLASH上锁
u8 STMFLASH_GetStatus(void);			  	    //获得状态
u8 STMFLASH_WaitDone(u32 time);				    //等待操作结束
u8 STMFLASH_ErasePage(u32 sectoraddr);	 	//擦除页
u8 STMFLASH_WriteWord(u32 faddr, u32 dat);	//写入字
u32 STMFLASH_ReadWord(u32 faddr);		  	    //读出字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
void copy_flash(u32 StartAddr,u32 TargetAddr,u32 len);           

void Test_Write(u32 WriteAddr,u32 WriteData);								   

void IAP_RJ45_reseive(u8 socket,u8 *Readbuff,u16 len);



#endif
