#include "c8051F340.h"
#include "main.h"
#include "POWER.h"
#include <lcd\lcd.h>

		extern unsigned char POWER_VOL,PCH,vcom; 
void Power_on(void)
{
	
POWER_VOL=0;		
	FM_short();
IOVCC_EN = 1;
VCI_EN = 1;
BL_EN = 1;
Delay(50);		
LCD_Init();	

PCH=10;	//第一步画面
lcd_show(PCH);
#ifdef READ_REG		
DispRegValue(IC_REG,REG_NUM); //读ID,读寄存器参数
#endif

	
}


void Power_off(void)
{
vcom=vcom_default;
	sleep();	
	IOVCC_EN = 0;
	VCI_EN = 0;
	BL_EN = 0;
	MTP_EN = 0;
	FMQ =0;		
	POWER_VOL=1;
	OLED_ShowString(0,6,"                ");
	#ifdef READ_REG		
	Disp_ID_clr();//清除缓存的ID数据		
	#endif
	FM_short();	
}


void lcd_show(unsigned char ShowId)
{
    switch(ShowId)
    {
			
			case 10: DispScaleVer_Gray();break;
			case 11: DispColor(GRAY50);break;
			case 12: DispColor(WHITE);break;
			case 13: DispColor(RED);break;
			case 14: DispColor(GREEN);break;
			case 15: DispColor(BLUE);break;
			case 16: DispFrame();break;
	//		case 17: DispGrayHor16();break;//

				#ifdef PIC_T		
			case 17: DispPic(pic_eval);break;
				#endif	
			
			
			default:DispGrayHor16(); break;

    }
}

	
void VCOM_ADJ_(void)
{
  u8 table_V[]="0123456789";	  
	u8 gg,ss,bb;

//*******************************VCOM调试方式一*****************************************//
			
//WriteComm(0XC5);
//WriteData(0x00);
//WriteData(vcom);
//WriteData(0x80);
	
	WriteComm(0xC4); WriteData(vcom); 
//************************************************************************//	
	
bb=vcom/100;// 2
ss=(vcom%100)/10;
gg=(vcom%100)%10;
OLED_ShowString(0,6,"                ");	
OLED_ShowString(0,6,"     Dat:");		
OLED_ShowChar(86,6,table_V[bb]);
OLED_ShowChar(94,6,table_V[ss]);
OLED_ShowChar(102,6,table_V[gg]);	

}
						
					

