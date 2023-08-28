#include "c8051F340.h"

#include <main.h>
#include <mmc_sd\mmc_sd.h>

unsigned char xdata buffer[512];

//---------------------------------------------------------
unsigned char SPI_WriteByte(uint8 val)
{
    unsigned char ret;

    SPI0DAT = val;
    while(!SPIF);                      
    SPIF = 0;
    ret = SPI0DAT;
    return ret;          
}

unsigned char SPI_ReadByte(void)
{   
    unsigned char ret;

	SPI0DAT  = 0xff;
    while (!SPIF);
	SPIF = 0;  
	ret = SPI0DAT;
    return ret;
}

unsigned char MMC_SD_SendCommand(uint8 cmd, uint32 arg)
{
	unsigned char r1;
	unsigned char  retry=0;
	
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);

	SPI_CS_Assert();
	
	SPI_WriteByte(cmd | 0x40);
	SPI_WriteByte(arg>>24);
	SPI_WriteByte(arg>>16);
	SPI_WriteByte(arg>>8);
	SPI_WriteByte(arg); 
	SPI_WriteByte(0x95); 
	
	while((r1 = SPI_WriteByte(0xff)) == 0xff)
		if(retry++ > 100) break;  //time out error

	SPI_CS_Deassert();

	return r1;
}

unsigned char MMC_SD_Reset(void)
{
	unsigned char i;
	unsigned char retry;
	unsigned char r1=0;

	retry = 0;
	CLKSEL = 0x03;

	SPI0CKR   = (48000000/(2*400000)) - 1;  
	do
	{
		for(i=0;i<100;i++) SPI_WriteByte(0xff);
		r1 = MMC_SD_SendCommand(0, 0);	//send idle command
		retry++;
		if(retry>10) break;	//time out
	} while(r1 != 0x01);	

	retry = 0;
	do
	{
	    do
	    {
		    r1 = MMC_SD_SendCommand(55, 0);	
		    retry++;
		    if(retry>100) return 1;	//time out
	    } while(r1!=0X01);

	    retry = 0;
		r1 = MMC_SD_SendCommand(41, 0);	
		retry++;
		if(retry>100) return 1;	//time out
	} while(r1!=0X00);

	if (r1!=0X00)
	{
	    retry = 0;
	    do
	    {
		    r1 = MMC_SD_SendCommand(1, 0);	//send active command
		    retry++;
		    if(retry>100) return 1;	//time out
	    } while(r1);
	}
	r1 = MMC_SD_SendCommand(59, 0);	//disable CRC

	r1 = MMC_SD_SendCommand(16, 512);	//set sector size to 512
    SPI0CKR   = (48000000/(2*12000000)) - 1; 
	CLKSEL = 0x00;
	return 0;	
}

unsigned char MMC_SD_WriteSingleBlock(uint32 sector)
{
	unsigned char r1;
	unsigned int i;
	unsigned char retry=0;

	CLKSEL = 0x03;
	do
	{
		r1 = MMC_SD_SendCommand(24, sector<<9);//send command
		retry++;
		if(retry>10) return 1;		//time out
	} while(r1 != 0x00);	



	SPI_CS_Assert();
	
	SPI_WriteByte(0xff);

	SPI_WriteByte(0xfe);			//send start byte
	
	for(i=0; i<512; i++)		//send 512 bytes data
	{
		SPI_WriteByte(buffer[i]);   
	}
	
	SPI_WriteByte(0xff);
	SPI_WriteByte(0xff);
	
	r1 = SPI_WriteByte(0xff);
	
	if( (r1&0x1f) != 0x05)
	{
		SPI_CS_Deassert();
		return r1;
	}
			
	while(!SPI_WriteByte(0xff));

	SPI_CS_Deassert();
	CLKSEL = 0x00;
	return 0;
}

unsigned char MMC_SD_ReadSingleBlock(uint32 sector)
{
	unsigned char r1;
	unsigned int i;
	unsigned char retry=0;

	CLKSEL = 0x03;
	do
	{
		r1 = MMC_SD_SendCommand(17, sector<<9);	//read command
		retry++;
		if(retry>10) return 1;	//time out
	} while(r1 != 0x00);	


	SPI_CS_Assert(); //wait to start recieve data
	
	while(SPI_ReadByte() != 0xfe);

	for(i=0; i<512; i++)	//read 512 bytes
	{
		buffer[i] = SPI_ReadByte();
	}

	SPI_WriteByte(0xff);//crc
	SPI_WriteByte(0xff);
	
	SPI_CS_Deassert();
	CLKSEL = 0x00;
	return 0;
}

#ifdef USE_SD_INIT
void SD_Init(void)
{
    unsigned char n;

    while(MMC_SD_Reset())
    {
	    n++;
	    if(n>1)break;
    }
}
#endif
