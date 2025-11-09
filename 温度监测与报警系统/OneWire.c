#include <REGX52.H>
#include "OneWire.h"

sbit OneWire_DQ = P3^7;    // DS18B20 数据线

/**
  * @brief  OneWire 初始化与复位
  * @retval 0=DS18B20 存在，1=无器件响应
  */
unsigned char OneWireInit(void)
{
    unsigned char i;
    unsigned char presence;

    OneWire_DQ = 1;               // 释放总线
    OneWire_DQ = 0;               // 拉低复位
    i = 247; while (--i);         // 延时 ≈ 500us

    OneWire_DQ = 1;               // 释放总线，等待从机回应
    i = 32; while (--i);          // 延时 ≈ 70us

    presence = OneWire_DQ;        // 读取存在脉冲（0=存在，1=无）

    i = 247; while (--i);         // 延时 ≈ 500us（恢复总线）

    return presence;
}

/**
  * @brief  写 1 位数据
  * @param  BitValue: 0 or 1
  */
void OneWireWriteBit(unsigned char BitValue)
{
    unsigned char i;

    OneWire_DQ = 0;               // 起始信号
    i = 4; while (--i);           // 保持低电平 ≈ 10us

    OneWire_DQ = BitValue;        // 写数据位
    i = 29; while (--i);          // 保持 ≈ 60us

    OneWire_DQ = 1;               // 释放总线
}

/**
  * @brief  读 1 位数据
  * @retval 0 or 1
  */
unsigned char OneWireReadBit(void)
{
    unsigned char i;
    unsigned char BitValue;

    OneWire_DQ = 0;
    i = 2; while (--i);           // 起始低电平 ≈ 5us

    OneWire_DQ = 1;               // 释放总线准备读
    i = 2; while (--i);           // 等待从机输出 ≈ 5us

    BitValue = OneWire_DQ;        // 采样数据位
    i = 24; while (--i);          // 时隙结束 ≈ 50us

    return BitValue;
}

/**
  * @brief  写 1 字节数据（低位先传）
  */
void OneWireWriteByte(unsigned char Byte)
{
    unsigned char i;

    for(i=0;i<8;i++)
    {
        OneWireWriteBit(Byte & 0x01);
        Byte >>= 1;
    }
}

/**
  * @brief  读 1 字节数据（低位先读）
  */
unsigned char OneWireReadByte(void)
{
    unsigned char i, Byte = 0;

    for(i=0;i<8;i++)
    {
        if(OneWireReadBit()) Byte |= (0x01 << i);
    }
    return Byte;
}
