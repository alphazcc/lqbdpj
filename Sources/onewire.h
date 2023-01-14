#ifndef __ONEWIRE_H
#define __ONEWIRE_H

unsigned int rd_temperature(void);
unsigned int get_temper(void);

unsigned int Read_DS18B20_Data(void);
void Write_DS18B20_Data(unsigned char dataH,unsigned char dataL);

#endif
