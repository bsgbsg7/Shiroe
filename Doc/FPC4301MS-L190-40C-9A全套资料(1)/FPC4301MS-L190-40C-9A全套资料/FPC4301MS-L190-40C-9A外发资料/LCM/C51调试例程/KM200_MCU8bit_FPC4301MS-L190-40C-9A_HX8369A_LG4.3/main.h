#ifndef __MAIN_h__
#define __MAIN_h__

typedef  unsigned char   uint8;
typedef  signed char     int8;
typedef  unsigned int    uint16;
typedef  signed int	     int16;   
typedef  unsigned long	 uint32; 
typedef  signed long	 int32; 


//------------------------------------------------------
#define ROW  800		     	//480
#define COL  480          //320
//------------------------------------------------------
#define MCU               ///
//#define SPI               //

//------------------------------------------------------
#ifdef  MCU
#define MCU8bit         //根据实际需要选择，不要同时打开
//#define MCU16bit   	    //
#endif
//------------------------------------------------------
//------------------------------------------------------
#ifdef  SPI
//#define SPI_3W       //3线SPI  无 RS
 #define SPI_4W       //4线SPI  有 RS
#endif
//------------------------------------------------------
#define AUTO_PIC    1     //1：打开自动跑画面，0手动跑画面
#define Auto_Delay  800		//自动跑画面的延时，10ms步进
//------------------------------------------------------
#define PIC_T             //打开图片显示
//------------------------------------------------------
//  只能选择打开一张图片，否则报错
#define mugu            //显示小蘑菇图片
//#define shuiguo         //显示水果图片
//#define renwu            //显示人物图片
//------------------------------------------------------
#define  READ_REG         //打开读屏寄存器功能  
#define  IC_REG  0xF4    //需要读取的寄存器
#define  REG_NUM  4       //寄存器参数个数 ， 小屏只能显示7个数据，大于7的会省略不显示
//------------------------------------------------------
#define    vcom_adj    1
#define  vcom_Step     2   //调试步进
#define  vcom_default  50   //默认值


//---------------以下不要修改-------------------------------------------------------------------------------------------

#define DBH  P3			    
#define DBL  P0	


#define KEY1_PRES 	3	//电源开关
#define KEY2_PRES	  2	//上一步
#define KEY3_PRES	  1	//下一步
 
//LCM
sbit	CS0 = P1^3;
sbit	RST = P1^0;
sbit	RS  = P1^4;
sbit	WR0 = P1^2;
sbit	RD0 = P1^1;



sbit	SPI_DI = P0^2;
sbit	SPI_CLK = P0^0;
sbit	SPI_CS = P0^5;		 
sbit	SPI_RS = P0^6;
sbit	SPI_RES = P0^4;



sbit	IOVCC_EN = P2^3;
sbit	VCI_EN   = P2^4;
sbit	BL_EN    = P2^5;
sbit	MTP_EN   = P2^6;
sbit	FMQ      = P2^2;

#define RED    0xF800		 //定义颜色常量 
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define GRAY   0xEF5D	     //0x2410
#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5	

//-----------OLED_--------------------------------------

#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

sbit OLED_CS=P1^5; //片选
sbit OLED_RST =P1^7;//复位
sbit OLED_DC =P1^6;//数据/命令控制
sbit OLED_SCL=P2^1;//时钟 D0（SCLK?
sbit OLED_SDIN=P2^0;//D1（MOSI） 数据

#define delay_ms  Delay

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 

#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;



void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);


//------------------------------------------------------
void  Delay(unsigned int dly);
void  WaitKey(void);
unsigned char KEY_Scan(void);
void  Suspend_Device(void);
void  Devices_Init(void);
void  MCU_Init(void); 
void FM_short(void);
void FM_Long(void);
void TEST_IMG(void);




#endif
