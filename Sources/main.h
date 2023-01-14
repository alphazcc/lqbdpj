#ifndef _MAIN_H_
#define _MAIN_H_

#include "person.h"
#include "iic.h"
#include "onewire.h"
#include "ds1302.h"

#define uint unsigned int
#define uchar unsigned char
	
extern uchar Digbuf[8];
extern uchar time[7];
extern uint NE555_Frequency;
extern uchar uart_receive_buf[6],uart_num;
extern uint key_state;
extern uchar uart_send_buf[10];

uint text_num=125;
uchar get_ADC=0;
uint temperature=0;
uchar get_dis_flag=0;
uint distance=0;

void check_key(void);
void Uart_check(void);

#endif
