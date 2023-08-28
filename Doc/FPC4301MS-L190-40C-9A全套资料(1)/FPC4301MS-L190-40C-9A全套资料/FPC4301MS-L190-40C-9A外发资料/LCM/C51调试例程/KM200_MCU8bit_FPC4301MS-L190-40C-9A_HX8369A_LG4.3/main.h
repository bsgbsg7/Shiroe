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
#define MCU8bit         //����ʵ����Ҫѡ�񣬲�Ҫͬʱ��
//#define MCU16bit   	    //
#endif
//------------------------------------------------------
//------------------------------------------------------
#ifdef  SPI
//#define SPI_3W       //3��SPI  �� RS
 #define SPI_4W       //4��SPI  �� RS
#endif
//------------------------------------------------------
#define AUTO_PIC    1     //1�����Զ��ܻ��棬0�ֶ��ܻ���
#define Auto_Delay  800		//�Զ��ܻ������ʱ��10ms����
//------------------------------------------------------
#define PIC_T             //��ͼƬ��ʾ
//------------------------------------------------------
//  ֻ��ѡ���һ��ͼƬ�����򱨴�
#define mugu            //��ʾСĢ��ͼƬ
//#define shuiguo         //��ʾˮ��ͼƬ
//#define renwu            //��ʾ����ͼƬ
//------------------------------------------------------
#define  READ_REG         //�򿪶����Ĵ�������  
#define  IC_REG  0xF4    //��Ҫ��ȡ�ļĴ���
#define  REG_NUM  4       //�Ĵ����������� �� С��ֻ����ʾ7�����ݣ�����7�Ļ�ʡ�Բ���ʾ
//------------------------------------------------------
#define    vcom_adj    1
#define  vcom_Step     2   //���Բ���
#define  vcom_default  50   //Ĭ��ֵ


//---------------���²�Ҫ�޸�-------------------------------------------------------------------------------------------

#define DBH  P3			    
#define DBL  P0	


#define KEY1_PRES 	3	//��Դ����
#define KEY2_PRES	  2	//��һ��
#define KEY3_PRES	  1	//��һ��
 
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

#define RED    0xF800		 //������ɫ���� 
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
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0

sbit OLED_CS=P1^5; //Ƭѡ
sbit OLED_RST =P1^7;//��λ
sbit OLED_DC =P1^6;//����/�������
sbit OLED_SCL=P2^1;//ʱ�� D0��SCLK?
sbit OLED_SDIN=P2^0;//D1��MOSI�� ����

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
