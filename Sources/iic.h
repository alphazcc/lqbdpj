#ifndef _IIC_H
#define _IIC_H

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

void EEPROM_Write(unsigned char add,unsigned char dat);
void DAC_Write(unsigned char dat);
unsigned char EEPROM_Read(unsigned char add);
unsigned char ADC_Read(unsigned char add);

#endif
