/*
  程序说明: 用户程序
  软件环境: Keil uVision 5 
  硬件环境: CT107D单片机综合实训平台
  日    期: 2020-11-13
  功能说明：各功能模块化程序
*/

#include "person.h"

//数码管
uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar Digbuf[]={10,10,11,11,11,11,11,11};
uchar Digcom=0;

uint key_state=0;	//按键
uchar uart_receive_buf[6],uart_num=0;	//串口
uchar uart_send_buf[10];
uint NE555_Falling=0,NE555_Time=0,NE555_Frequency=0;	//NE555

/************************超声波测距******************************/

#if 0

sbit TX=P1^0;
sbit RX=P1^1;

uint get_distance(void)		
{
	uint distance,time;
	
	send_wave();
	TR0=1;
	while(RX==1 && TF0==0);
	TR0=0;
	if(TF0==1)
	{
		TF0=0;
		distance=999;
	}
	else
	{
		time=TH0*256+TL0;
		distance=(uint)time*0.017;
		distance=distance/12;
	}
	TH0=0;TL0=0;
	
	return distance;
}

void send_wave(void)		//发送40KHz方波
{
	uchar i=8;
	do
	{
		TX=1;
		somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;somenop;
		TX=0;
		somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;somenop;
	}
	while(i--);
}

void Timer0Init(void)		//计数@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0;				//设置定时初值
	TH0 = 0;				//设置定时初值
	TF0 = 0;				//清除TF0标志
//	TR0 = 1;			//定时器0开始计时
}

#endif

/***************************************************************/

/*****************************串口******************************/

#if 0

void UartSendStr(uchar *Str)	//串口发送
{
	ES=0;
	while(*Str!=0)
	{
		SBUF=*Str;
		while(TI==0);
		TI=0;
		Str++;
	}
	ES=1;
}

void UartInt(void) interrupt 4	//串口中断
{
	if(RI)
	{
		RI=0;
		uart_receive_buf[uart_num++]=SBUF;
	}
}

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;			//设定定时初值
	TH1 = 0xFD;			//设定定时初值
	ET1 = 0;				//禁止定时器1中断
	TR1 = 1;				//启动定时器1
}

#endif

/***************************************************************/

/*************************独立按键*******************************/

#if 0

uchar keyscan(void)		
{
	uchar key=0;
	uchar time_span=10;
	
	if(P30==0)
	{
		delayms(5);
		if(P30==0)
		{
			key=7;
			while(P30==0);
		}
	}
	
	if(P31==0)
	{
		delayms(5);
		if(P31==0)
		{
			key=6;
			while(P31==0);
		}
	}
	
	if(P32==0)					//带有按键单、双击检测
	{
		delayms(5);
		if(P32==0)
		{
			key=5;
			while(P32==0);	//松手检测
			
			delayms(100);		//双击按键间隔
			while(time_span--)	//是否第二次按下
			{
				if(P32==0)
				{
					key=55;
					while(P32==0);	//松手检测
				}
			}
		}
	}
	
	if(P33==0)
	{
		delayms(5);
		if(P33==0)
		{
			key=4;
			while(P33==0);
		}
	}
			
	return key;
}

#endif

/****************************************************************/

/***************************矩阵按键****************************/

#if 0

uchar keyscan_16(void)		
{
	uchar key=0;
	uchar temp;
	
	P3 = 0x7f;P44 = 0;P42 = 1;
	temp = P3&0x0f;
	if(temp != 0x0f)
	{
		delayms(5);
		temp = P3&0x0f;
		if(temp != 0x0f)
		{
			temp = P3;
			switch(temp)
			{
				case 0x7e: key = 7; break;
				case 0x7d: key = 6; break;
				case 0x7b: key = 5; break;
				case 0x77: key = 4; break;
			}
			while(temp != 0x0f)
			{
				temp = P3&0x0f;
			}
		}	
	}

	P3 = 0xbf;P44 = 1;P42 = 0;
	temp = P3&0x0f;
	if(temp != 0x0f)
	{
		delayms(5);
		temp = P3&0x0f;
		if(temp != 0x0f)
		{
			temp = P3;
			switch(temp)
			{
				case 0xbe: key = 11; break;
				case 0xbd: key = 10; break;
				case 0xbb: key = 9; break;
				case 0xb7: key = 8; break;
			}
			while(temp != 0x0f)
			{
				temp = P3&0x0f;
			}
		}	
	}

	P3 = 0xdf;P44 = 1;P42 = 1;
	temp = P3&0x0f;
	if(temp != 0x0f)
	{
		delayms(5);
		temp = P3&0x0f;
		if(temp != 0x0f)
		{
			temp = P3;
			switch(temp)
			{
				case 0xde: key = 15; break;
				case 0xdd: key = 14; break;
				case 0xdb: key = 13; break;
				case 0xd7: key = 12; break;
			}
			while(temp != 0x0f)
			{
				temp = P3&0x0f;
			}
		}
	}

	P3 = 0xef;P44 = 1;P42 = 1;
	temp = P3&0x0f;
	if(temp != 0x0f)
	{
		delayms(5);
		temp = P3&0x0f;
		if(temp != 0x0f)
		{
			temp = P3;
			switch(temp)
			{
				case 0xee: key = 19; break;
				case 0xed: key = 18; break;
				case 0xeb: key = 17; break;
				case 0xe7: key = 16; break;
			}
			while(temp != 0x0f)
			{
				temp = P3&0x0f;
			}
		}	
	}	
	
	return key;
}

#endif

/****************************************************************/

/************************NE555频率检测***************************/

#if 0

void Timer1Int(void) interrupt 3
{
	NE555_Time++;
	if(NE555_Time==250)		//500毫秒
	{
		NE555_Time=0;
		NE555_Frequency=NE555_Falling*2;
		NE555_Falling=0;
	}
}

void Timer0Int(void) interrupt 1
{
	NE555_Falling++;
}

void Timer1Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x40;			//设置定时初值
	TH1 = 0xA2;			//设置定时初值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
}

void Timer0Init(void)		//计数器@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x06;		//设置定时器模式
	TL0 = 0xFF;			//设置定时初值
	TH0 = 0xFF;			//设置定时重载值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

#endif

/*****************************************************************/

/****************************定时器2******************************/

#if 0

void Timer2Int(void) interrupt 12
{
	static uint intr2=0;

	//数码管显示
	P2&=0x1f;P2|=0xc0;		//打开位选
	P0=(1<<Digcom);
	P2&=0x1f;P2|=0xe0;		//打开段选
	P0=tab[Digbuf[Digcom]];
	if(++Digcom==8)
		Digcom=0;
	P2&=0x1f;					//关锁存

	//按键辅助
	intr2++;				
	if(intr2==50)
	{
		intr2=0;
		key_state++;		//按键按下时间
		if(key_state==150)
			key_state=0;
	}
}

void Timer2Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x40;			//设置定时初值
	T2H = 0xA2;			//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	
	IE2|=0x04;			//打开定时器2中断
}

#endif

/*****************************************************************/

/***************************数码管显示****************************/

#if 0

void display(void)
{
	for(Digcom=0;Digcom<8;Digcom++)
	{
		P2&=0x1f;P2|=0xc0;		//打开位选
		P0=(1<<Digcom);
		P2&=0x1f;P2|=0xe0;		//打开段选
		P0=tab[Digbuf[Digcom]];
		P2&=0x1f;							//关锁存
		delayms(1);
	}
	P2&=0x1f;P2|=0xc0;P0=0xff;	//选中所有数码管
	P2&=0x1f;P2|=0xe0;P0=0xff;	//关闭所有数码管
}

#endif

/*****************************************************************/

void allinit(void)	//初始化
{
	P2=0xa0;P0=0x00;	//关蜂鸣器、继电器
	P2=0x80;P0=0xff;	//关LED
	P2=0xc0;P0=0xff;	//关数码管
	P2=0xe0;P0=0xff;
}

void delayms(uint ms)	//延时函数
{
	while(ms--)
		Delay1ms();
}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
