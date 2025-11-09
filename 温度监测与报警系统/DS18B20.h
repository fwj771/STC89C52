#ifndef __DS18B20_H__
#define __DS18B20_H__

void DS18B20_StartConvert(void);
float DS18B20_ReadTemp(void);
void DS18B20_LoadAlarm(void);
void DS18B20_SetAlarm(signed char TH, signed char TL);
bit DS18B20_AlarmCheck(float Temperature);

#endif
