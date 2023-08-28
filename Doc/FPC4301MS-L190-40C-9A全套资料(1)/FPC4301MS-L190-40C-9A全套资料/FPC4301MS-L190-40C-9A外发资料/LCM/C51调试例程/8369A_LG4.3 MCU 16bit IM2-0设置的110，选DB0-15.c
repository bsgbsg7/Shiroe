
void WriteComm(unsigned char i)
{	

		RS=0;	
		CS0=0;
		RD0=1;
		WR0=0;  
		DBH=i>>8;
		DBL=i;	
		WR0=1; 	
		CS0=1; 
	
}

void WriteData(unsigned char i)
{
	
		RS=1;		
		CS0=0;
		RD0=1;
		WR0=0;  	
		DBH=i>>8;
		DBL=i; 	
		WR0=1; 
		CS0=1; 

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


void LCD_Init(void)
{

	RST=1;  
	Delay(10);

	RST=0;
	Delay(10);

	RST=1;
	Delay(120);

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


