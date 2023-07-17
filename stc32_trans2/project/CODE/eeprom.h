#ifndef __EEPROM_H
#define __EEPROM_H

void extern_iap_write_float(float dat,uint8 num,uint8 pointnum,uint16 addr);


float StrToDouble(const char *s);
float iap_read_float(uint8 len, uint16 addr);
int16 iap_read_int16(uint8 len, uint16 addr);

#endif