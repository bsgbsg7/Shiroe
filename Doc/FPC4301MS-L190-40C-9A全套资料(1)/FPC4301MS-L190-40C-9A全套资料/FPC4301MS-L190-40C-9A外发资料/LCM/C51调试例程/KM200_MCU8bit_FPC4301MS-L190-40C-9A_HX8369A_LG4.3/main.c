#include "c8051F340.h"
#include <intrins.h>
#include <absacc.h>
#include <stdlib.h>
#include <main.h>
#include <lcd\lcd.h>
#include <POWER.h>
#include "oledfont.h"  	

	unsigned char vcom;
	unsigned char POWER_VOL=1,PCH=10,at_jump=1; 
/********************************************************************************/
void main(void)
{ 
	

    unsigned char key;
		MCU_Init();
		OLED_Init();//初始化OLED  
		OLED_Clear(); 
		Disp_Pro_Name();

	while(1)
			{
						key=KEY_Scan();

						if(key==KEY2_PRES)
								{	
									FM_short();
									if(PCH==10) PCH=18; 								
									PCH--;
									lcd_show(PCH);
									at_jump=0;		

								}

						if(key==KEY3_PRES)
								{	
											if( vcom_adj==1)
											{								
														FM_short();		
														vcom+=vcom_Step;
														VCOM_ADJ_();	
											}
											
													if( vcom_adj==0)	
													{									
															FM_short();
															PCH++;								
															if(PCH==18) PCH=10; 
															lcd_show(PCH);
															at_jump=0;
													}
											
								}
								
						if(key==KEY1_PRES)
								{
											switch(POWER_VOL)
											{
												case 0: Power_off(); break;
												case 1: Power_on(); at_jump=1;  break;												
												default: break;
										}

									}
								
													if(POWER_VOL==0&&at_jump==1&&AUTO_PIC)		
											{
													
														lcd_show(PCH);	
												
												
												
											#ifdef READ_REG
														Disp_ID_Lcd();
												#endif
												
												
												
														PCH++;
														TEST_IMG();
														if(PCH==18) PCH=10;
											}

			}		
}

//----------------------------------------------------------------------
void Delay(unsigned int dly)
{
    unsigned int i,j,k;
	
	  for(i=0;i<dly;i++)
	{
  for(k=0;k<5;k++)
    	for(j=0;j<959;j++);
	}
}


void  MCU_Init(void)
{
    unsigned char n;
	PCA0MD &= ~0x40;

			//OSC
			OSCICN = 0x83; 	  //Internal oscillator enabled, SYSCLK = Internal Oscillator = 12MHz
			CLKMUL = 0x00;    //Select internal oscillator as clock multiplier source clock

			CLKMUL |= 0x80;   //Enable clock multiplier

			for(n=0;n<255;n++);

			CLKMUL |= 0xC0;           //Initialize the clock multiplier
			while(!(CLKMUL & 0x20));  //Wait for multiplier to lock
			CLKSEL = 0x00;            //Change SYSCLK to (4x Multiplier    ) = 48MHz  (48MHz is as fast as F34 will run); 0x03;
			RSTSRC   |= 0x02;  

	
	FLSCL = FLSCL | 0x90;
CLKSEL = CLKSEL | 0x03;
OSCICN = OSCICN | 0x03;

//  P0~P4					 //set high impedance ,	skipped by crossbar
//	P0MDIN |= 0xFF;          //数字输入(无模拟输入）
//	P0MDOUT = 0x00; 	 P0 |= 0XFF;      //P0口 置1  
//	P1MDOUT = 0x00;	   P1 |= 0XFF;      //P0口 置1  


	P0MDOUT |= 0xFF;	
	P1MDOUT |= 0xFF;	
	P2MDOUT |= 0xFF;
	P3MDOUT |= 0xFF;	
//	P4MDIN  |= 0xFF;    //数字输入(无模拟输入）
	
	
	P4MDOUT = 0x00;	 //Set Input Mode Before Read 
	P4MDIN  |= 0xFF; 
	
	

	

	 
//		XBR0     |= 0x02;	
			XBR1     |= 0x40;       // Enable Crossbar
			
	IOVCC_EN = 0;
		VCI_EN = 0;
		 BL_EN = 0;
		MTP_EN = 0;							
FM_Long();

#ifdef SPI_4W
	XBR0     |= 0x02;	
	XBR1     |= 0x40;       // Enable Crossbar
	CLKSEL = 0x03;
	SPI0CFG   = 0x70;
	SPI0CN    = 0x0f;
	//SPI0CKR   = (48000000/(2*400000)) - 1;  
	SPI0CKR=0x00;
#endif	
Power_off();
}



void FM_short(void)
{
			FMQ=1;
			Delay(10);	
			FMQ=0;	
}

void FM_Long(void)
{
			FMQ=1;
			Delay(200);	
			FMQ=0;	
}

unsigned char KEY_Scan(void)
{	 
	unsigned char KEY_NUM;  
	static u8 key_up=1;//按键按松开标志
			KEY_NUM=P4;
	if((!((KEY_NUM&0xe0)==0xe0))&&key_up)
	{
				KEY_NUM|=0xE0;
				Delay(15);//去抖动 
						
					KEY_NUM=P4;
			if(!((KEY_NUM&0xe0)==0xe0))
			{	   
				
					key_up=0;
	
			     if((KEY_NUM&0x80)==0)return KEY1_PRES;	
			else if((KEY_NUM&0x40)==0)return KEY2_PRES;
      else if((KEY_NUM&0x20)==0)return KEY3_PRES;
			}
			
	}else if((KEY_NUM&0xe0)==0xe0)key_up=1; 	 
 	return 0;// 无按键按下
}


void TEST_IMG(void)//自动播放画面的同时按键有效
{
	unsigned char f,i,dly;
dly=Auto_Delay/10;
		for(i=0;i<dly;i++)//此循环决定了延时长短
	{
		Delay(10);
		f=KEY_Scan();	
		
			if(f==KEY2_PRES)
		{
				at_jump=0;
				FM_short();
				lcd_show(12);		
				PCH=12;
			i=dly;
		}	
		
		if(f==KEY3_PRES)
		{
				at_jump=0;
				FM_short();
				lcd_show(11);		
					PCH=11;
			i=dly;
		}
		
			if(f==KEY1_PRES)
			{				
				switch(POWER_VOL)
				{
					case 0: Power_off(); break;
					case 1: Power_on(); at_jump=1;  break;												
					default: break;
				}
							i=dly;
			}
	}


}
//-----------------------OLED----------------------------

//初始化SSD1306					    
void OLED_Init(void)
{
 
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  


void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			{
		   OLED_SDIN_Set();
			}
else
		   OLED_SDIN_Clr();
				OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 





	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			
//			else {	
//				OLED_Set_Pos(x,y+1);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
//				
//			}
			
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}












//---------------------------------------------------------
