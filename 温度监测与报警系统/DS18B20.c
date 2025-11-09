#include <REGX52.H>
#include "OneWire.h"
#include "Delay.h"

extern signed char TH, TL ;  

/**
  * @brief 启动温度转换（非阻塞）
  */
void DS18B20_StartConvert(void)
{
    OneWireInit();
    OneWireWriteByte(0xCC);     // Skip ROM
    OneWireWriteByte(0x44);     // Convert T
}

/**
  * @brief 读取温度寄存器，返回摄氏温度
  */
float DS18B20_ReadTemp(void)
{
    unsigned char LSB, MSB;
    int tempValue;
    float temperature;

    OneWireInit();
    OneWireWriteByte(0xCC);     // Skip ROM
    OneWireWriteByte(0xBE);     // Read Scratchpad

    LSB = OneWireReadByte();
    MSB = OneWireReadByte();

    tempValue = (MSB << 8) | LSB;
    temperature = tempValue / 16.0;

    return temperature;
}

/**
  * @brief 从 EEPROM 载入报警阈值（掉电记忆）
  */
void DS18B20_LoadAlarm(void)
{
    unsigned char discard;

    OneWireInit();
    OneWireWriteByte(0xCC);
    OneWireWriteByte(0xB8);     // Recall E2 → 将 EEPROM 加载到 Scratchpad

    OneWireInit();
    OneWireWriteByte(0xCC);
    OneWireWriteByte(0xBE);     // Read Scratchpad

    discard = OneWireReadByte();   // byte0 温度 LSB（丢弃）
    discard = OneWireReadByte();   // byte1 温度 MSB（丢弃）
    TH = OneWireReadByte();        // byte2 高温报警上限
    TL = OneWireReadByte();        // byte3 低温报警下限
}

/**
  * @brief 设置报警阈值，并写入 EEPROM（掉电不丢失）
  */
void DS18B20_SetAlarm(signed char th, signed char tl)
{
    TH = th;
    TL = tl;

    OneWireInit();
    OneWireWriteByte(0xCC);
    OneWireWriteByte(0x4E);     // Write Scratchpad
    OneWireWriteByte(TH);       // 写入高温报警上限
    OneWireWriteByte(TL);       // 写入低温报警下限
    OneWireWriteByte(0x7F);     // 配置分辨率=12位

    OneWireInit();
    OneWireWriteByte(0xCC);
    OneWireWriteByte(0x48);     // Copy Scratchpad → 保存入 EEPROM
}

/**
  * @brief 报警判定
  * @return 1=报警, 0=正常
  */
bit DS18B20_AlarmCheck(float temperature)
{
    return (temperature >= TH || temperature <= TL);
}
