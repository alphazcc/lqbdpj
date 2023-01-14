/*****************************************************************************/
//	程序名称：	蓝桥杯单片机工程模板
//	软件环境： 	Keil uVision 5 
//	创建日期：	2020-11-13
//	版本：			V3.0
//	作者：			小菜鸟
//	参考资料：	蓝桥杯电子类官方资源包
//	单片机型号：IAP15F2K61S2
//	测试平台：	单片机综合实训平台 CT107D
//	功能说明：	涵盖开发板所有基本功能
//	说明：			请勿用于商业用途，由于本代码造成的任何损失与作者无关
/*****************************************************************************/

/*
	所有模块功能程序模块已经收纳进本工程，有警告是因为有函数没有用到
	测试时将测试部分对应程序模块选择编译，比赛时根据需要选择编写程序模块
	思路千千万，编程方法不唯一，不喜请轻喷……
	有问题加群讨论！QQ：1097948690
	或联系 QQ：786347269
*/

#include "main.h"

void main(void)
{

	allinit();
//	DS1302_Init();
//	Timer0Init();
//	ET0=1;
//	Timer1Init();
//	ET1=1;
//	UartInit();	
//	ES=1;
//	Timer2Init();
//	EA=1;
	
//	text_num=EEPROM_Read(0x01)*256+EEPROM_Read(0x02);
//	text_num++;
//	EEPROM_Write(0x01,text_num/256);delayms(5);
//	EEPROM_Write(0x02,text_num%256);delayms(5);
		
	while(1)
	{
//		Digbuf[5]=text_num/100;
//		Digbuf[6]=text_num%100/10;
//		Digbuf[7]=text_num%10;
						
//		if(get_dis_flag==1)
//		{
//			get_dis_flag=0;
//			distance=get_distance();
//		}
//		Digbuf[5]=distance/100;
//		Digbuf[6]=distance%100/10;
//		Digbuf[7]=distance%10;
//		delayms(800);
//		get_dis_flag=1;
		
//		Digbuf[3]=NE555_Frequency/10000;
//		Digbuf[4]=NE555_Frequency%10000/1000;
//		Digbuf[5]=NE555_Frequency%1000/100;
//		Digbuf[6]=NE555_Frequency%100/10;
//		Digbuf[7]=NE555_Frequency%10;		
		
//		temperature=get_temper();
//		Digbuf[4]=temperature/1000;
//		Digbuf[5]=temperature%1000/100;
//		Digbuf[6]=temperature%100/10;
//		Digbuf[7]=temperature%10;

//		DS1302_Get();
//		Digbuf[0]=time[2]/10;
//		Digbuf[1]=time[2]%10;
//		Digbuf[2]=10;
//		Digbuf[3]=time[1]/10;
//		Digbuf[4]=time[1]%10;
//		Digbuf[5]=10;
//		Digbuf[6]=time[0]/10;
//		Digbuf[7]=time[0]%10;
		
//		get_ADC=ADC_Read(0x01);
//		Digbuf[5]=get_ADC/100;
//		Digbuf[6]=get_ADC%100/10;
//		Digbuf[7]=get_ADC%10;
		
//		check_key();
//		Uart_check();

//		display();
	}
}

/************************按键检测*******************************/

#if 0

void check_key(void)		
{
	uint get_key=0;
	uint key_down=0;
	bit key_long_down=0;
	
	key_state=0;	
	get_key=keyscan();
//	get_key=keyscan_16();
	key_down=key_state;
		
	if(key_down>=8)
		key_long_down=1;
	else
		key_long_down=0;
	
	if(get_key==4 && key_long_down==0)			//短按S4
	{
		P2=0x80;P0=0xfe;		//LED1点亮
		get_dis_flag=1;			//触发超声波测距
	}	
	else if(get_key==4 && key_long_down==1)	//长按S4
	{
		key_long_down=0;
		P2=0x80;P0=0xfd;		//LED2点亮
		get_dis_flag=1;			//触发超声波测距
	}
	
	if(get_key==5)				//单击S5
	{
		P2=0x80;P0=0xfb;		//LED3点亮
		get_dis_flag=1;			//触发超声波测距
	}
	else if(get_key==55)	//双击S5
	{
		P2=0x80;P0=0xf7;		//LED4点亮
		get_dis_flag=1;			//触发超声波测距
	}
	
//	if(get_key!=0)			//显示键值
//	{
//		Digbuf[6]=get_key/10;
//		Digbuf[7]=get_key%10;
//	}
}

#endif

/***************************************************************/

/************************串口检测*******************************/

#if 0

void Uart_check(void)		
{
	if(uart_num>0)
	{
		if(uart_receive_buf[uart_num-1]==0xa)
		{
			if((uart_receive_buf[0]=='S') && (uart_receive_buf[1]=='T') && (uart_receive_buf[2]==0xd))
			{
				sprintf(uart_send_buf,"Uart TEXT\r\n");
			}
			else
				sprintf(uart_send_buf,"ERROR\r\n");
			
			UartSendStr(uart_send_buf);
			uart_num=0;
		}
		else			
		{
			if(uart_num==6)
			{
				sprintf(uart_send_buf,"ERROR\r\n");
				UartSendStr(uart_send_buf);
				uart_num=0;
			}
		}	
	}
}

#endif

/***************************************************************/
