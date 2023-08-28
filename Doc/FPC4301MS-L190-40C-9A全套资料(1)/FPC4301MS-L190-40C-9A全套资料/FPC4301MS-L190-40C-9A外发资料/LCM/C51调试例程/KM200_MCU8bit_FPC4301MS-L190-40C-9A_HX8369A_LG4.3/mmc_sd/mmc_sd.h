//#include<main.h>

#ifndef __MMC_SD_h__
#define __MMC_SD_h__

#define SPI_CS_Assert()    NSSMD0   = 0;   
#define SPI_CS_Deassert()  NSSMD0   = 1;  

//extern unsigned char xdata buffer[512];

extern void  SD_Init(void);
extern unsigned char MMC_SD_ReadSingleBlock(unsigned long sector);
extern unsigned char MMC_SD_WriteSingleBlock(unsigned long sector);
extern unsigned char MMC_SD_SendCommand(unsigned char cmd, unsigned long arg);
extern unsigned char SPI_WriteByte(unsigned char val);

#endif

