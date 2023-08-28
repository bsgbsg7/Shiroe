#include "c8051F340.h"
#include <intrins.h>
#include <absacc.h>
#include <main.h>
#include <mmc_sd\mmc_sd.h>
#include <lcd\lcd.h>
unsigned char a,b,c,d;
xdata unsigned int reg_data[20];	
//----------------------------------------------------------------------

unsigned char SPI_SendData(unsigned char i)
{ 
#ifdef    SPI_3W
   unsigned char n;
  
   for(n=0; n<8; n++)			
   {  
	  if(i&0x80) SPI_DI=1;
      	else SPI_DI=0;
      i<<= 1;

	  SPI_CLK=0;
      SPI_CLK=1;			
   }
//	     return i;  
#endif 
	 
#ifdef    SPI_4W
	    unsigned char ret;

    SPI0DAT = i;
    while(!SPIF);                      
    SPIF = 0;
    ret = SPI0DAT;
//    return ret;      
#endif 
	 
	     return i;   
}

void WriteComm(uint8 i)
{	
	//-------------------------------------------------	
#ifdef MCU8bit	
		RS=0;	
		CS0=0;
		RD0=1;
		WR0=0;  
		DBL=i;	
		WR0=1; 	
		CS0=1; 
	#endif
//-------------------------------------------------		
	#ifdef MCU16bit	
		RS=0;	
		CS0=0;
		RD0=1;
		WR0=0;  
		DBH=i>>8;
		DBL=i;	
		WR0=1; 	
		CS0=1; 
	#endif
	
//-------------------------------------------------	
	#ifdef SPI	

			#ifdef SPI_4W
			SPI_CS=0;
			SPI_RS=0;
			#endif

    #ifdef SPI_3W
			SPI_CS=0;
			SPI_DI=0;	
			SPI_CLK=0;
			SPI_CLK=1;
			#endif

		SPI_SendData(i);
		SPI_CS=1;
	
#endif	
//-------------------------------------------------		
}

void WriteData(uint8 i)
{
#ifdef MCU8bit		
		RS=1;		
		CS0=0;
		RD0=1;
		WR0=0;  	
		DBL=i; 	
		WR0=1; 
		CS0=1; 
	#endif
#ifdef MCU16bit		
		RS=1;		
		CS0=0;
		RD0=1;
		WR0=0;  	
		DBH=i>>8;
		DBL=i; 	
		WR0=1; 
		CS0=1; 
	#endif
	
#ifdef SPI

	#ifdef SPI_4W
		SPI_CS=0;
		SPI_RS=1;		
	#endif

	#ifdef SPI_3W
		SPI_CS=0;
		SPI_DI=1;	 
		SPI_CLK=0;
		SPI_CLK=1;
	#endif

		SPI_SendData(i);
		SPI_CS=1;
#endif
	
	
}
void DispGrayHor16(void)	 
{
		unsigned int i,j,k;
   	BlockWrite(0,COL-1,0,ROW-1);    	
		CS0=0;
		RD0=1;
		RS=1;		

	
#ifdef MCU8bit

	for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL%16;j++)
		{
			DBL=0;
			WR0=0;
			WR0=1;
		}
		
		for(j=0;j<16;j++)
		{
	        for(k=0;k<COL/16;k++)
			{		
				DBL=((j*2)<<3)|((j*4)>>3);	
				WR0=0;
				WR0=1;
					 
				DBL=((j*4)<<5)|(j*2);
				WR0=0;
				WR0=1;
			} 
		}
	}

#endif
#ifdef MCU16bit
	for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL%16;j++)
		{
			DBH=0;
			DBL=0;
			WR0=0;
			WR0=1;
		}
		
		for(j=0;j<16;j++)
		{
	        for(k=0;k<COL/16;k++)
			{		
				DBH=((j*2)<<3)|((j*4)>>3);		 
				DBL=((j*4)<<5)|(j*2);
				WR0=0;
				WR0=1;
			} 
		}	
	}
#endif
	
	
#ifdef SPI
//i=0;
		for(j=0;j<16;j++)
		{
	        for(k=0;k<COL/16;k++)
			{		
				
							 WriteData(((j*2)<<3)|((j*4)>>3));  

				      WriteData(((j*4)<<5)|(j*2));  	 

			} 
		}	
		
	///--------------------------------------------	
		for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL%16;j++)
		{
WriteData(0x00);
			WR0=0;
			WR0=1;
		}
		
		for(j=0;j<16;j++)
		{
	        for(k=0;k<COL/16;k++)
			{		
			WriteData(((j*2)<<3)|((j*4)>>3));					 
			WriteData(((j*4)<<5)|(j*2));

			} 
		}
	}	
		
		
		
		
	#endif	

	
	CS0=1;
}

void DispPic(unsigned int code *picture)
{
    unsigned int *p;
	unsigned int  i,j; //i-row,j-col
	unsigned char n,k; //n-row repeat count,k-col repeat count

	BlockWrite(0,COL-1,0,ROW-1);

	CS0=0;
	RS =1;
	RD0=1;
#ifdef MCU8bit
	for(n=0;n<ROW/PIC_HEIGHT;n++)         //n-row repeat count
	{
		for(i=0;i<PIC_HEIGHT;i++)
		{
			p=picture;
			for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
		    {
				for(j=0;j<PIC_WIDTH;j++)
		    	{
				    DBL=(*(p+i*PIC_HEIGHT+j))>>8; 
					WR0=0;
					WR0=1; 		
					DBL=*(p+i*PIC_HEIGHT+j);  
					WR0=0;
					WR0=1;
				}

		  	}

			p=picture;
			for(j=0;j<COL%PIC_WIDTH;j++)
		    {
				    DBL=(*(p+i*PIC_HEIGHT+j))>>8; 
					WR0=0;
					WR0=1; 
		
					DBL=*(p+i*PIC_HEIGHT+j);  
					WR0=0;
					WR0=1;
		  	}
		}
	}

	for(i=0;i<ROW%PIC_HEIGHT;i++)
	{
		p=picture;
		for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
	    {
			for(j=0;j<PIC_WIDTH;j++)
	    	{
			    DBL=(*(p+i*PIC_HEIGHT+j))>>8; 
				WR0=0;
				WR0=1; 
	
				DBL=*(p+i*PIC_HEIGHT+j);  
				WR0=0;
				WR0=1;
			}

	  	}

		p=picture;
		for(j=0;j<COL%PIC_WIDTH;j++)
	    {
			    DBL=(*(p+i*PIC_HEIGHT+j))>>8; 
				WR0=0;
				WR0=1; 
	
				DBL=*(p+i*PIC_HEIGHT+j);  
				WR0=0;
				WR0=1;
	  	}
	}
#endif

#ifdef MCU16bit

	for(n=0;n<ROW/PIC_HEIGHT;n++)         //n-row repeat count
	{
		for(i=0;i<PIC_HEIGHT;i++)
		{
			p=picture;
			for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
		    {
				for(j=0;j<PIC_WIDTH;j++)
		    	{
				    DBH=(*(p+i*PIC_HEIGHT+j))>>8; 
					//WR0=0;
					//WR0=1; 
		
					DBL=*(p+i*PIC_HEIGHT+j);  
					WR0=0;
					WR0=1;
				}

		  	}

			p=picture;
			for(j=0;j<COL%PIC_WIDTH;j++)
		    {
				    DBH=(*(p+i*PIC_HEIGHT+j))>>8; 
					//WR0=0;
					//WR0=1; 
		
					DBL=*(p+i*PIC_HEIGHT+j);  
					WR0=0;
					WR0=1;
		  	}
		}
	}

	for(i=0;i<ROW%PIC_HEIGHT;i++)
	{
		p=picture;
		for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
	    {
			for(j=0;j<PIC_WIDTH;j++)
	    	{
			    DBH=(*(p+i*PIC_HEIGHT+j))>>8; 
				//WR0=0;
				//WR0=1; 
	
				DBL=*(p+i*PIC_HEIGHT+j);  
				WR0=0;
				WR0=1;
			}

	  	}

		p=picture;
		for(j=0;j<COL%PIC_WIDTH;j++)
	    {
			    DBH=(*(p+i*PIC_HEIGHT+j))>>8; 
				//WR0=0;
				//WR0=1; 
	
				DBL=*(p+i*PIC_HEIGHT+j);  
				WR0=0;
				WR0=1;
	  	}
	}

#endif
#ifdef SPI
		for(n=0;n<ROW/PIC_HEIGHT;n++)         //n-row repeat count
	{
		for(i=0;i<PIC_HEIGHT;i++)
		{
			p=picture;
			for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
		    {
				for(j=0;j<PIC_WIDTH;j++)
		    	{
							WriteData(((*(p+i*PIC_HEIGHT+j))>>8));  	
              WriteData(*(p+i*PIC_HEIGHT+j)); 
				}

		  	}

			p=picture;
			for(j=0;j<COL%PIC_WIDTH;j++)
		    {
					WriteData(((*(p+i*PIC_HEIGHT+j))>>8));  	
                    WriteData(*(p+i*PIC_HEIGHT+j));
		  	}
		}
	}

	for(i=0;i<ROW%PIC_HEIGHT;i++)
	{
		p=picture;
		for(k=0;k<COL/PIC_WIDTH;k++) //k-col repeat count
	    {
			for(j=0;j<PIC_WIDTH;j++)
	    	{
					WriteData(((*(p+i*PIC_HEIGHT+j))>>8));  	
                    WriteData(*(p+i*PIC_HEIGHT+j));
			}

	  	}

		p=picture;
		for(j=0;j<COL%PIC_WIDTH;j++)
	    {
					WriteData(((*(p+i*PIC_HEIGHT+j))>>8));  	
                    WriteData(*(p+i*PIC_HEIGHT+j));
	  	}
	}
	
	#endif


	CS0=1;
}

void DispScaleVer_Gray(void)
{
	unsigned int i,j,k;
	
	BlockWrite(0,COL-1,0,ROW-1);

	CS0=0;
	RD0=1;
	RS=1;

#ifdef MCU8bit
	for(k=0;k<ROW%32;k++)
	{
		for(j=0;j<COL;j++)
		{
			DBL=0;
			WR0=0;
			WR0=1;
		}			
	}
	
	for(k=0;k<32;k++)
	{	
		for(i=0;i<ROW/32;i++)
		{	
			// GRAY										  
			for(j=0;j<COL;j++)
			{
				DBL=((k<<3)|((k*2)>>3));
				WR0=0;
				WR0=1;

				DBL=((k*2)<<5)|k;
				WR0=0;
				WR0=1;
			}
		}
	
	}
#endif

#ifdef MCU16bit
	for(k=0;k<ROW%32;k++)
	{
		for(j=0;j<COL;j++)
		{
			DBH=0;
			DBL=0;
			WR0=0;
			WR0=1;

		}			
	}
	
	for(k=0;k<32;k++)
	{	
		for(i=0;i<ROW/32;i++)
		{	
			// GRAY										  
			for(j=0;j<COL;j++)
			{
				DBH=(k<<3)|((k*2)>>3);
				DBL=((k*2)<<5)|k;
				WR0=0;
				WR0=1;
			}
		}
	
	}
#endif


#ifdef SPI//-----------------------------------------------------------------------------------------------------------------------------------------------
	
		for(k=0;k<32;k++)
	{	
		for(i=0;i<ROW/32;i++)
		{	
			// GRAY										  
			for(j=0;j<COL;j++)
			{
				
			WriteData((k<<3)|((k*2)>>3));  	
			WriteData(((k*2)<<5)|k);  

			}
		}
	
	}
	
#endif



	CS0=1;
}

void sleep(void)
{
WriteComm(0x28);
Delay(10);
WriteComm(0x10);
Delay(10);
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) reentrant
{
	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);

	WriteComm(0x2c);
}


void DispColor(unsigned int color)
{
	unsigned int i,j;

	BlockWrite(0,COL-1,0,ROW-1);

	CS0=0; 
	RS=1;
	RD0=1;
#ifdef MCU8bit
			for(i=0;i<ROW;i++)
			{
					for(j=0;j<COL;j++)
				{    
					 DBL=color>>8;
					 WR0=0;  
					 WR0=1;  
			
					 DBL=color;
					 WR0=0;  
					 WR0=1;  
				}
			}

#endif
#ifdef MCU16bit
	DBH=color>>8;
	DBL=color; 

	for(i=0;i<ROW;i++)
	{
	    for(j=0;j<COL;j++)
		{    
			WR0=0;
			WR0=1;
		}
	}

#endif
	
#ifdef SPI
	
	for(i=0;i<ROW;i++)
	{
	    for(j=0;j<COL;j++)
		{    
			 WriteData(color>>8);  	
             WriteData(color);  
		}
	}	
#endif	

	CS0=1; 
}



void DispBand(void)	 
{
	unsigned int i,j,k;
	//unsigned int color[8]={0x001f,0x07e0,0xf800,0x07ff,0xf81f,0xffe0,0x0000,0xffff};
	unsigned int color[8]={0xf800,0xf800,0x07e0,0x07e0,0x001f,0x001f,0xffff,0xffff};//0x94B2
	//unsigned int gray16[]={0x0000,0x1082,0x2104,0x3186,0x42,0x08,0x528a,0x630c,0x738e,0x7bcf,0x9492,0xa514,0xb596,0xc618,0xd69a,0xe71c,0xffff};

   	BlockWrite(0,COL-1,0,ROW-1);
    	
	CS0=0;
	RD0=1;
	RS=1;																									  
#ifdef MCU8bit
	for(i=0;i<8;i++)
	{
		for(j=0;j<ROW/8;j++)
		{
	        for(k=0;k<COL;k++)
			{
				DBL=color[i]>>8;      
				WR0=0;  
				WR0=1;  
   
				DBL=color[i]; 
				WR0=0;  
				WR0=1;  
			} 
		}
	}
	for(j=0;j<ROW%8;j++)
	{
        for(k=0;k<COL;k++)
		{
			DBL=color[7]>>8;      
			WR0=0;  
			WR0=1;  

			DBL=color[7]; 
			WR0=0;  
			WR0=1;  
		} 
	}
	#endif
	
	#ifdef MCU16bit
	for(i=0;i<8;i++)
	{
		for(j=0;j<ROW/8;j++)
		{
	        for(k=0;k<COL;k++)
			{
				DBL=color[i];     
				DBH=color[i]>>8;  
				WR0=0;  
				WR0=1;  
			} 
		}
	}
	for(j=0;j<ROW%8;j++)
	{
        for(k=0;k<COL;k++)
		{
			DBL=color[7];     
			DBH=color[7]>>8;  
			WR0=0;  
			WR0=1;  
		} 
	}	
	
	#endif
	
	#ifdef SPI
	

	for(i=0;i<8;i++)
	{
		for(j=0;j<ROW/8;j++)
		{
	        for(k=0;k<COL;k++)
			{

			 WriteData(color[i]>>8);  	
             WriteData(color[i]);  
			} 
		}
	}
	for(j=0;j<ROW%8;j++)
	{
        for(k=0;k<COL;k++)
		{

			WriteData(color[7]>>8);  	
             WriteData(color[7]); 
		} 
	}	
	
	
	#endif
	
	
	CS0=1;
}

void DispFrame(void)
{
	unsigned int i,j;
	
	BlockWrite(0,COL-1,0,ROW-1);

	CS0=0;
	RD0=1;
	RS=1;
#ifdef MCU8bit
	DBL=0xf8;WR0=0;WR0=1;DBL=0x00,WR0=0;WR0=1;
	for(i=0;i<COL-2;i++){DBL=0xFF;WR0=0;WR0=1;DBL=0xFF;WR0=0;WR0=1;}
	DBL=0x00;WR0=0;WR0=1;DBL=0x1f;WR0=0;WR0=1;

	for(j=0;j<ROW-2;j++)
	{
		DBL=0xf8;WR0=0;WR0=1;DBL=0x00;WR0=0;WR0=1;
		for(i=0;i<COL-2;i++){DBL=0x00;WR0=0;WR0=1;DBL=0x00;WR0=0;WR0=1;}
		DBL=0x00;WR0=0;WR0=1;DBL=0x1F;WR0=0;WR0=1;
	}

	DBL=0xf8;WR0=0;WR0=1;DBL=0x00;WR0=0;WR0=1;
	for(i=0;i<COL-2;i++){DBL=0xFF;WR0=0;WR0=1;DBL=0xFF;WR0=0;WR0=1;}
	DBL=0x00;WR0=0;WR0=1;DBL=0x1F;WR0=0;WR0=1;
#endif
	
#ifdef MCU16bit
	
		DBH=0xf8,DBL=0x00,WR0=0; WR0=1;
	for(i=0;i<COL-2;i++){DBH=0xFF,DBL=0xFF,WR0=0;WR0=1;}
	DBH=0x00,DBL=0x1f,WR0=0;WR0=1;

	for(j=0;j<ROW-2;j++)
	{
		DBH=0xf8,DBL=0x00,WR0=0;WR0=1;
		for(i=0;i<COL-2;i++){DBH=0x00,DBL=0x00,WR0=0;WR0=1;}
		DBH=0x00,DBL=0x1F,WR0=0;WR0=1;
	}

	DBH=0xf8,DBL=0x00,WR0=0;WR0=1;
	for(i=0;i<COL-2;i++){DBH=0xFF,DBL=0xFF,WR0=0;WR0=1;}
	DBH=0x00,DBL=0x1F,WR0=0;WR0=1;
	
#endif
	
#ifdef SPI
	
	WriteData(WHITE>>8);  	
    WriteData(WHITE);
	for(i=0;i<COL-2;i++)
	{
	WriteData(WHITE>>8);  	
    WriteData(WHITE);
	}
    WriteData(WHITE>>8);  	
    WriteData(WHITE);

	for(j=0;j<ROW-2;j++)
	{
		WriteData(WHITE>>8);  	
        WriteData(WHITE);
		for(i=0;i<COL-2;i++)
		{
		WriteData(BLACK>>8);  	
        WriteData(BLACK);
		}
		WriteData(WHITE>>8);  	
        WriteData(WHITE);

	}

	WriteData(WHITE>>8);  	
    WriteData(WHITE);
	for(i=0;i<COL-2;i++)
	{
	WriteData(WHITE>>8);  	
	WriteData(WHITE);
	}
	WriteData(WHITE>>8);  	
    WriteData(WHITE);	
	
#endif	
		
	CS0=1;
}






void WriteOneDot(unsigned int color)
{ 
#ifdef MCU8bit	
	CS0=0;  
	RD0=1;
	RS=1;	

	DBL=color>>8; 
	WR0=0;
	WR0=1;

	DBL=color;		
	WR0=0;
	WR0=1;

	CS0=1;
#endif		
	
	#ifdef MCU16bit	
	CS0=0;  
	RD0=1;
	RS=1;	
	DBH=color>>8; 
	DBL=color;		
	WR0=0;
	WR0=1;
	CS0=1;
#endif	

#ifdef SPI_3W	

		SPI_CS=0;
		SPI_DI=1;	 
		SPI_CLK=0;
		SPI_CLK=1;
		SPI_SendData(color>>8);
		
		SPI_DI=1;	 
		SPI_CLK=0;
		SPI_CLK=1;
		SPI_SendData(color);		
		SPI_CS=1;	
#endif	

#ifdef SPI_4W	


		SPI_CS=0;
		SPI_RS=1;		
		SPI_SendData(color>>8);
		
		SPI_RS=1;		
		SPI_SendData(color);		

		SPI_CS=1;
#endif	
	
}

unsigned char ToOrd(unsigned char ch)
{
	if(ch<32)
	{
		ch=95;
	}
	else if((ch>=32)&&(ch<=47)) //(32~47)空格~/
	{
		ch=(ch-32)+10+62;
	}
	else if((ch>=48)&&(ch<=57))//(48~57)0~9
	{
		ch=ch-48;
	}
	else if((ch>=58)&&(ch<=64))//(58~64):~@
	{
		ch=(ch-58)+10+62+16;
	}
	else if((ch>=65)&&(ch<=126))//(65~126)A~~
	{
		ch=(ch-65)+10;
	}
	else if(ch>126)
	{		
		ch=95;
	}

	return ch;
}

void  DispOneChar(unsigned char ord,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor)	 // ord:0~95
{													  
   unsigned char i,j;
   unsigned char  *p;
   unsigned char dat;
   unsigned int index;

   BlockWrite(Xstart,Xstart+(FONT_W-1),Ystart,Ystart+(FONT_H-1));

   index = ord;

   if(index>95)	   //95:ASCII CHAR NUM
   		index=95;

   index = index*((FONT_W/8)*FONT_H);	 

   p = ascii;
   p = p+index;

   for(i=0;i<(FONT_W/8*FONT_H);i++)
    {
       dat=*p++;
       for(j=0;j<8;j++)
        {
           if((dat<<j)&0x80)
             {
                WriteOneDot(TextColor);
             }      
           else 
             {
                WriteOneDot(BackColor);	  
             }
         }
     }
}


void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor)
{

	while(!(*str=='\0'))
	{
		DispOneChar(ToOrd(*str++),Xstart,Ystart,TextColor,BackColor);

		if(Xstart>((COL-1)-FONT_W))
		{
			Xstart=0;
		    Ystart=Ystart+FONT_H;
		}
		else
		{
			Xstart=Xstart+FONT_W;
		}

		if(Ystart>((ROW-1)-FONT_H))
		{
			Ystart=0;
		}
	}	
	BlockWrite(0,COL-1,0,ROW-1);
}


void DispInt(unsigned int i,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor)
{
	if(Xstart>((COL-1)-FONT_W*4))
	{
		Xstart=(COL-1)-FONT_W*4;
	}
	if(Ystart>((ROW-1)-FONT_H))
	{
		Ystart=(Ystart-1)-FONT_H;
	}
			
//	DispOneChar((i>>12)%16,Xstart,Ystart,TextColor,BackColor); //ID value
//	DispOneChar((i>>8)%16,Xstart+FONT_W,Ystart,TextColor,BackColor);
	DispOneChar((i>>4)%16,Xstart+FONT_W*2,Ystart,TextColor,BackColor);
	DispOneChar(i%16,Xstart+FONT_W*3,Ystart,TextColor,BackColor); 

	BlockWrite(0,COL-1,0,ROW-1);
}

void Disp_ID_Lcd(void)
{	
		u8 table_V[]="0123456789ABCDEF";	 
		unsigned char i;
//		DispStr("READ ID:",15,5,BLUE,BLACK);	
    for(i=0;i<REG_NUM;i++)
	{
			if(i<=6)//小屏只显示7个参数，大于自动忽略
			{
					OLED_ShowChar((i*18),6,table_V[((reg_data[i]&0xf0)>>4)]);
					OLED_ShowChar(((i*18)+8),6,table_V[(reg_data[i]&0x0f)]);
			}
		DispInt(reg_data[i],i*20,22,GREEN,BLACK);	
	}	
	
}


void Disp_ID_clr(void)//清除缓存的ID数据
{
		unsigned char i;
	for(i=0;i<REG_NUM;i++)
	{
		reg_data[i]=0x00;
	}
}


unsigned int ReadData(void)
{
#ifdef MCU8bit
  unsigned int temp;
		DBL = 0xFF; 
		DBH = 0xff;

	CS0 = 0;
	RS  = 1;        
	WR0 = 1;
	RD0 = 0;

	P3MDOUT = 0x00;	 //Set Input Mode Before Read 
	P0MDOUT = 0x00;
	P0MDIN  |= 0xFF; 	
	P3MDIN  |= 0xFF; 
	
  _nop_();  _nop_();_nop_();  _nop_();
  RD0 = 1;  	
  _nop_();  _nop_();_nop_();  _nop_();
	

  temp=DBH;
   temp<<=8;
  temp|=DBL;

  P3MDOUT = 0xFF;
  P0MDOUT = 0xFF;
		
  RD0 = 1;  
	Delay(1);
                    
  CS0 = 1;
  return temp;
#endif	
	
#ifdef MCU16bit
  unsigned int temp;
		DBL = 0xFF; 
		DBH = 0xff;

	CS0 = 0;
	RS  = 1;        
	WR0 = 1;
	RD0 = 0;

	P3MDOUT = 0x00;	 //Set Input Mode Before Read 
	P0MDOUT = 0x00;
	P0MDIN  |= 0xFF; 	
	P3MDIN  |= 0xFF; 
	
  _nop_();  _nop_();_nop_();  _nop_();
  RD0 = 1;  	
  _nop_();  _nop_();_nop_();  _nop_();
	

  temp=DBH;
   temp<<=8;
  temp|=DBL;

  P3MDOUT = 0xFF;
  P0MDOUT = 0xFF;
		
  RD0 = 1;  
	Delay(1);
                    
  CS0 = 1;
  return temp;
#endif	


#ifdef SPI
unsigned int i=0,tmp=0;
P0MDOUT &= ~(1 << 2);		  
P0 |=  (1 << 2); 

		for(i = 0; i < 8; i++)
	{			
		SPI_CLK=0;
		SPI_CLK=1;
		tmp <<= 1;
		if(SPI_DI) 
			tmp |= 0x01;
	}
P0MDOUT |= 0Xff;	
return tmp;
	
#endif	 

}



void DispRegValue(unsigned int RegIndex,unsigned char ParNum)
{
		unsigned char i;
//		xdata unsigned int reg_data[20];	
	

//--------Reset_LCD----------	
//		CS0=0;		
//		RST=0;
//		Delay(10);
//		RST=1;
//		Delay(10);
	
//---------PASSWORD----------
	

//---------------------------	

	
#ifdef MCU
	WriteComm(RegIndex);	
#endif

#ifdef  SPI_3W
		SPI_CS=0;
		SPI_DI=0;	
		SPI_CLK=0;
		SPI_CLK=1;
		SPI_SendData(RegIndex);
#endif

#ifdef  SPI_4W
			SPI_CS=0;
			SPI_RS=0;
			SPI_SendData(RegIndex);			
			XBR0 &= ~(1 << 1);						
#endif


	if(ParNum>20)
		ParNum=20;

	for(i=0;i<ParNum;i++)
	{
		reg_data[i]=ReadData();
	}
	
	
	
if(reg_data[0]==0x85)
{

reg_data[0]=0x77;
reg_data[1]=0x89;	
}	
		
	
	
	
#ifdef  SPI_4W
	XBR0     |= 0x02;	
#endif	
	
	
Disp_ID_Lcd();

}


void LCD_Init(void)
{
	
	int i, j;
	#ifdef MCU	 //SPI RESET
	CS0=0;	
	
	RST=1;  
	Delay(10);

	RST=0;
	Delay(10);

	RST=1;
	Delay(20);
#endif
		
#ifdef SPI	 //SPI RESET
	SPI_CS=0;	
	SPI_RES=1;  
	Delay(10);
	SPI_RES=0;
	Delay(10);
	SPI_RES=1;
	Delay(50);
#endif

//WriteComm(0x11);
//Delay(1000);
//WriteComm(0x29);
//------------------------------------LCD_CODE------------------------------------------------------------------------------------------------------------------------//
	WriteComm(0xB9); //Set_EXTC
	WriteData(0xFF);
	WriteData(0x83);
	WriteData(0x69);
	
	WriteComm(0xB1); //Set Power
	WriteData(0x01);
	WriteData(0x00);
	WriteData(0x34);
	WriteData(0x07);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x0F);
	WriteData(0x2A);
	WriteData(0x32);
	WriteData(0x3F);
	WriteData(0x3F);
	WriteData(0x07);
	WriteData(0x10);
	WriteData(0x01);
	WriteData(0xE6);
	WriteData(0xE6);
	WriteData(0xE6);
	WriteData(0xE6);
	WriteData(0xE6);
	
	WriteComm(0xB2); // SET Display 480x800
	WriteData(0x00);
	WriteData(0x20);  //0x2b;0x20-MCU;0x29-DPI;RM,DM; RM=0:DPI IF;  RM=1:RGB IF;
	WriteData(0x03);
	WriteData(0x03);
	WriteData(0x70);
	WriteData(0x00);
	WriteData(0xFF);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x03);
	WriteData(0x03);
	WriteData(0x00);
	WriteData(0x01);
	
	WriteComm(0xB4); // SET Display CYC
	WriteData(0x00);
	WriteData(0x0C);
	WriteData(0xA0);
	WriteData(0x0E);
	WriteData(0x06);
	
	WriteComm(0xB6); // SET VCOM
	WriteData(0x2C);
	WriteData(0x2C);
	
	WriteComm(0xD5); //SET GIP
	WriteData(0x00);
	WriteData(0x05);
	WriteData(0x03);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x09);
	WriteData(0x10);
	WriteData(0x80);
	WriteData(0x37);
	WriteData(0x37);
	WriteData(0x20);
	WriteData(0x31);
	WriteData(0x46);
	WriteData(0x8A);
	WriteData(0x57);
	WriteData(0x9B);
	WriteData(0x20);
	WriteData(0x31);
	WriteData(0x46);
	WriteData(0x8A);
	WriteData(0x57);
	WriteData(0x9B);
	WriteData(0x07);
	WriteData(0x0F);
	WriteData(0x02);
	WriteData(0x00);
	
	WriteComm(0xE0); //SET GAMMA
	WriteData(0x00);
	WriteData(0x08);
	WriteData(0x0D);
	WriteData(0x2D);
	WriteData(0x34);
	WriteData(0x3F);
	WriteData(0x19);
	WriteData(0x38);
	WriteData(0x09);
	WriteData(0x0E);
	WriteData(0x0E);
	WriteData(0x12);
	WriteData(0x14);
	WriteData(0x12);
	WriteData(0x14);
	WriteData(0x13);
	WriteData(0x19);
	WriteData(0x00);
	WriteData(0x08);
	WriteData(0x0D);
	WriteData(0x2D);
	WriteData(0x34);
	WriteData(0x3F);
	WriteData(0x19);
	WriteData(0x38);
	WriteData(0x09);
	WriteData(0x0E);
	WriteData(0x0E);
	WriteData(0x12);
	WriteData(0x14);
	WriteData(0x12);
	WriteData(0x14);
	WriteData(0x13);
	WriteData(0x19);
	
	WriteComm(0xC1); //set DGC
	WriteData(0x01); //enable DGC function
	WriteData(0x02); //SET R-GAMMA
	WriteData(0x08);
	WriteData(0x12);
	WriteData(0x1A);
	WriteData(0x22);
	WriteData(0x2A);
	WriteData(0x31);
	WriteData(0x36);
	WriteData(0x3F);
	WriteData(0x48);
	WriteData(0x51);
	WriteData(0x58);
	WriteData(0x60);
	WriteData(0x68);
	WriteData(0x70);
	WriteData(0x78);
	WriteData(0x80);
	WriteData(0x88);
	WriteData(0x90);
	WriteData(0x98);
	WriteData(0xA0);
	WriteData(0xA7);
	WriteData(0xAF);
	WriteData(0xB6);
	WriteData(0xBE);
	WriteData(0xC7);
	WriteData(0xCE);
	WriteData(0xD6);
	WriteData(0xDE);
	WriteData(0xE6);
	WriteData(0xEF);
	WriteData(0xF5);
	WriteData(0xFB);
	WriteData(0xFC);
	WriteData(0xFE);
	WriteData(0x8C);
	WriteData(0xA4);
	WriteData(0x19);
	WriteData(0xEC);
	WriteData(0x1B);
	WriteData(0x4C);
	WriteData(0x40);
	WriteData(0x02); //SET G-Gamma
	WriteData(0x08);
	WriteData(0x12);
	WriteData(0x1A);
	WriteData(0x22);
	WriteData(0x2A);
	WriteData(0x31);
	WriteData(0x36);
	WriteData(0x3F);
	WriteData(0x48);
	WriteData(0x51);
	WriteData(0x58);
	WriteData(0x60);
	WriteData(0x68);
	WriteData(0x70);
	WriteData(0x78);
	WriteData(0x80);
	WriteData(0x88);
	WriteData(0x90);
	WriteData(0x98);
	WriteData(0xA0);
	WriteData(0xA7);
	WriteData(0xAF);
	WriteData(0xB6);
	WriteData(0xBE);
	WriteData(0xC7);
	WriteData(0xCE);
	WriteData(0xD6);
	WriteData(0xDE);
	WriteData(0xE6);
	WriteData(0xEF);
	WriteData(0xF5);
	WriteData(0xFB);
	WriteData(0xFC);
	WriteData(0xFE);
	WriteData(0x8C);
	WriteData(0xA4);
	WriteData(0x19);
	WriteData(0xEC);
	WriteData(0x1B);
	WriteData(0x4C);
	WriteData(0x40);
	WriteData(0x02); //SET B-Gamma
	WriteData(0x08);
	WriteData(0x12);
	WriteData(0x1A);
	WriteData(0x22);
	WriteData(0x2A);
	WriteData(0x31);
	WriteData(0x36);
	WriteData(0x3F);
	WriteData(0x48);
	WriteData(0x51);
	WriteData(0x58);
	WriteData(0x60);
	WriteData(0x68);
	WriteData(0x70);
	WriteData(0x78);
	WriteData(0x80);
	WriteData(0x88);
	WriteData(0x90);
	WriteData(0x98);
	WriteData(0xA0);
	WriteData(0xA7);
	WriteData(0xAF);
	WriteData(0xB6);
	WriteData(0xBE);
	WriteData(0xC7);
	WriteData(0xCE);
	WriteData(0xD6);
	WriteData(0xDE);
	WriteData(0xE6);
	WriteData(0xEF);
	WriteData(0xF5);
	WriteData(0xFB);
	WriteData(0xFC);
	WriteData(0xFE);
	WriteData(0x8C);
	WriteData(0xA4);
	WriteData(0x19);
	WriteData(0xEC);
	WriteData(0x1B);
	WriteData(0x4C);
	WriteData(0x40);
	
	
	WriteComm(0x2D); 
		for (i=0; i<=63; i++)
		{
		WriteData(i*8);   
		}

		for (i=0; i<=63; i++)
		{
		WriteData(i*4);   
		}

		for (i=0; i<=63; i++)
		{
		WriteData(i*8);   
		}
	
	WriteComm(0x3A); //Set COLMOD
	WriteData(0x55);

	WriteComm(0x11); //Sleep Out
	Delay(120);
	WriteComm(0x29); //Display On
		
	
	WriteComm(0x2A); //Partial Display Setting
	WriteData(0x00); 
	WriteData(0x00); 
	WriteData(0x01); 
	WriteData(0xDF); 
	
	WriteComm(0x2B); 
	WriteData(0x00); 
	WriteData(0x00); 
	WriteData(0x03); 
	WriteData(0x1F); 
	
	WriteComm(0x2C);



}

void Disp_Pro_Name(void)
{

		OLED_ShowString(0,0,"KM200_TEST      ");
		OLED_ShowString(0,2,"8369a_lg4.3 ");
		OLED_ShowString(0,4,"T43-L3   ");
}

