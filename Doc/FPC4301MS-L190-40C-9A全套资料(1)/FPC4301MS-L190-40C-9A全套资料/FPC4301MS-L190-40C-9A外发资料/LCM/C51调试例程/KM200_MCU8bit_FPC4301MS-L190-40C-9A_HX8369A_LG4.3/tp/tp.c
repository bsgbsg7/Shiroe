#include "c8051F340.h"
#include <intrins.h>
#include <absacc.h>
 
#include<main.h>
#include<mmc_sd\mmc_sd.h>
#include<lcd\lcd.h>
#include <tp\tp.h>

#ifdef TP_TEST

void TP_Write(unsigned char cmd)
{
	unsigned char xdata i;

	TP_CS=0;
	TP_DCLK=0;

	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0)
		TP_DIN=0;
		else
		TP_DIN=1;
		TP_DCLK=1;
		cmd=cmd<<1;
		TP_DCLK=0;//be low ,before read
	}
}

unsigned int TP_Read(void)
{
	unsigned char xdata i;
	long int xdata 	val=0;

	TP_CS=0;
	TP_DOUT = 1;

	for(i=0;i<12;i++)
	{
		TP_DCLK=0;
		if(TP_DOUT)
		{
			val=val|(1<<(11-i));
		}
		TP_DCLK=1;
	}

	TP_CS=1;
	return val;
}

unsigned int TP_GetPosition(unsigned char cmd)
{
	unsigned int xdata temp;
	unsigned char xdata i=0,j=0,k=0;
	unsigned int  xdata TP[11];

	TP_PENIRQ = 1;

	while((i<11)&&(!TP_PENIRQ))
	{
		TP_CS=0;
		TP_Write(cmd+1);
		TP[i]=TP_Read();
		i++;
		TP_CS=1;
	}
	if(i!=0)
	{
		for(j=0;j<i;j++)
			for(k=0;k<i-j;k++)
				if (*(TP+k)>*(TP+k+1))
					{
					temp=*(TP+k);
					*(TP+k)=*(TP+k+1);
					*(TP+k+1)=temp;
				}
		return (TP[i/2]);
	}
	else
		return 0;
}

void TP_DrawLine(void)
{
	unsigned long int  xdata gx=0,gy=0;           //coordinate on glass
	unsigned long int  xdata tx=0,ty=0;           //coordinate value read from TP
	unsigned long int  xdata gxtemp=0,gytemp=0;            //coordinate on glass

	DispColor(BLACK);
	DrawGird(GRAY25);
	DispStr("TP TEST",0,0,BLACK,WHITE);

	TP_PENIRQ = 1;
	while (TP_PENIRQ) ;

	while(1)
	{

		while(KEY_STEP)
		{
			Delay(50);//!
			while(!TP_PENIRQ)
			{
				tx=TP_GetPosition(0xd0);//x-position measure
				gx=(tx-X_min)*(unsigned long int)(COL)/((X_max-X_min));
			
				ty=TP_GetPosition(0x90);//y-position measure
				gy=(ty-Y_min)*(unsigned long int)(ROW)/((Y_max-Y_min));
			
				Delay(15);//!
		
				//touch panel must be interrupted
				if((!TP_PENIRQ)&&(gx>=0&&gx<COL)&&(gy>=0&&gy<ROW))
				{
					PutPixel(gx,gy,RED);
				}
			}
			Delay(50);
		}

		DispColor(BLACK);
		DrawGird(GRAY25);
		DispStr("TP TEST",0,0,BLACK,WHITE);

	}

}

#endif




