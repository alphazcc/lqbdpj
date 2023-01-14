/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include <stc15f2k60s2.h>
#include <intrins.h>

sbit DQ = P1^4;  //单总线接口

void Delay2us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}

//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	t=t*10;			//更改时序
	while(t--);
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;Delay2us();
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20设备初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}


/******************以下内容按需要自己编写******************/

unsigned int rd_temperature(void)	//获取温度
{
	unsigned char MSB,LSB;
	unsigned int temp;
	
	init_ds18b20();				//DS18B20设备初始化
	Write_DS18B20(0xcc);	//跳过ROM操作命令，忽略64为ROM地址
	Write_DS18B20(0x44);	//发送启动温度转换命令
	Delay_OneWire(20);		//总线延时
	
	init_ds18b20();				//DS18B20设备初始化
	Write_DS18B20(0xcc);	//跳过ROM操作命令，忽略64为ROM地址
	Write_DS18B20(0xbe);	//发送读温度寄存器命令
	
	LSB = Read_DS18B20();	//先读低八位
	MSB = Read_DS18B20();	//再读高八位
	
	temp = (MSB<<8)|(LSB); 	//高8位为MSB的值，低8位为LSB的值  
	
	return temp;				//返回温度值
}

unsigned int get_temper(void)	//温度转换
{
	unsigned int temper;
	float tp;
	
	temper = rd_temperature();	//获取温度
	
	if(temper < 0)			//考虑负温度的情况
	{
		temper = temper-1;
		temper = ~temper;
	}
	tp = temper*0.0625;		//16位温度转换成10进制的温度
	temper = tp*100+0.5;	//保留两个小数点，并四舍五入
	
	return temper;
}

//以下两个函数极少用到
void Write_DS18B20_Data(unsigned char dataH,unsigned char dataL)
{
	init_ds18b20();			//DS18B20设备初始化
	
	Write_DS18B20(0xcc);	//跳过ROM操作，忽略64为ROM地址
	Write_DS18B20(0x4e);	//写暂存器指令4E
	Write_DS18B20(dataH);	//写高速缓存器TH高温限值dataH度
	Write_DS18B20(dataL);	//写高速缓存器TL低温限值dataL度
	Write_DS18B20(0x7f);	//写配置寄存器4
							//0x1f ： 0.5000°C  转换时间93.75ms
							//0x3f ： 0.2000°C  转换时间187.5ms
							//0x5f ： 0.1250°C  转换时间375ms
							//0x7f ： 0.0625°C  转换时间750ms
	Delay_OneWire(20);		//总线延时
	
	init_ds18b20();				//DS18B20设备初始化
	Write_DS18B20(0xcc);	//跳过ROM操作，忽略64为ROM地址
	Write_DS18B20(0x48);	//将写入的3字节暂存器的值写入到EEPROM，防止掉电丢失
	Delay_OneWire(20);		//总线延时

	init_ds18b20();				//DS18B20设备初始化
	Write_DS18B20(0xcc);	//跳过ROM操作，忽略64为ROM地址
	Write_DS18B20(0xb8);	//重调EEPROM指令
	Delay_OneWire(20);		//总线延时
}

unsigned int Read_DS18B20_Data(void)
{
	unsigned char tp,dataH,dataL;
	unsigned int temp;
	
	init_ds18b20();					//DS18B20设备初始化
	Write_DS18B20(0xcc);		//跳过ROM操作命令
	Write_DS18B20(0xbe);		//发送读温度寄存器命令
	
	tp = Read_DS18B20();		//先读低八位
	tp = Read_DS18B20();		//再读高八位
	
	dataH = Read_DS18B20();		//读高八位暂存器
	dataL = Read_DS18B20();		//读低八位暂存器
	
	temp = (dataH<<8)|(dataL); 	//高8位为addrH的值，低8位为addrL的值  
	
	return temp;				//返回数据值
}
