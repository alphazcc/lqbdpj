#ifndef _PERSON_H_
#define _PERSON_H_

#include <stc15f2k60s2.h>
#include <intrins.h>
#include <stdio.h>

#define uint unsigned int
#define uchar unsigned char
	
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

void allinit(void);
void Delay1ms();
void delayms(uint ms);
void Timer0Init(void);
void Timer1Init(void);
void Timer2Init(void);

uchar keyscan(void);
uchar keyscan_16(void);

void display(void);

uint get_distance(void);
void send_wave(void);

void UartInit(void);
void UartSendStr(uchar *Str);

#endif
