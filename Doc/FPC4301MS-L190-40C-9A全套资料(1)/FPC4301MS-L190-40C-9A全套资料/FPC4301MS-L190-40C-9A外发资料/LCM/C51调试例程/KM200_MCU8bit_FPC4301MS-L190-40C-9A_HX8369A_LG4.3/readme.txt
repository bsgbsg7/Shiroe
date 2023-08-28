/*******************************************************
数据端口定义： 
P2.7  P2.6	P2.5  P2.4  P2.3  P2.2  P2.1  P2.0
DB15  DB14  DB13  DB12  DB11  DB10  DB9   DB8 

P4.7  P4.6	P4.5  P4.4  P4.3  P4.2  P4.1  P4.0
DB7   DB6   DB5   DB4   DB3   DB2   DB1   DB0 

P1.2    P1.3
DB16    DB17
--------------------------------------------------------
控制端口定义：

//MCU | SPI
P0.5     P1.6     P1.7    P3.1   P3.0   
RST      RD0      WR0     RS     CS0	 
         SDA(SDI) SCL(SCK)

//SPI+RGB
P3.4     P1.0      P1.1     P1.2      P1.3
SPI_RS   SPI_CS    SPI_CLK  SPI_DI    SPI_RES

//TP
P1^4	P1^5	  P3^2	  P3^3     P3^4     P3^5	   
TP_CS	TP_DCLK   TP_DIN  TP_DOUT  TP_BUSY  TP_PENIRQ  

//
P0.4  P3.6	    P3.7
TE	  KEY_STEP  KEY

********************************************************/