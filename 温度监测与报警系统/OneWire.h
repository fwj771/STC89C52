#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

unsigned char OneWireInit(void);
void OneWireWriteBit(unsigned char Bit);
unsigned char OneWireReadBit(void);
void OneWireWriteByte(unsigned char byte);
unsigned char OneWireReadByte(void);

#endif
