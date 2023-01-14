/*
  程序说明: DS1302驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <stc15f2k60s2.h>
#include <intrins.h>

unsigned char time[]={50,59,23,0,0,0,0};	//定义默认时间数组

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302((dat/10<<4)|(dat%10));		//进制转换
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
	unsigned char dat1,dat2;				//定义变量
	
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	
	dat1=temp/16;					//进制转换
	dat2=temp%16;
	temp=dat1*10+dat2;
	
	return (temp);			
}


/******************以下内容需自己编写******************/

void DS1302_Init(void)				//DS1302初始化
{
	unsigned char i,add;
	add=0x80;
	Write_Ds1302_Byte(0x8e,0x00);	//关闭写保护
	for(i=0;i<7;i++)
	{
		Write_Ds1302_Byte(add,time[i]);
		add+=2;
	}
	Write_Ds1302_Byte(0x8e,0x80);	//打开写保护
}

void DS1302_Get(void)				//时间读取
{
	unsigned char i,add;
	add=0x81;
	for(i=0;i<3;i++)
	{
		time[i]=Read_Ds1302_Byte(add);
		add+=2;
	}
}
