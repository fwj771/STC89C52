#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "DS18B20.h"

extern signed char TH, TL;    // 全局报警阈值变量

sbit KEY_THADD = P3^1;        // 高温上限 + 按键
sbit KEY_THSUB = P3^0;        // 高温上限 - 按键
sbit KEY_TLADD = P3^2;        // 低温下限 + 按键
sbit KEY_TLSUB = P3^3;        // 低温下限 - 按键

/**
  * @brief  按键扫描（单击调节 TH / TL）
  * @note   内部自动消抖 + 自动更新 EEPROM + LCD 显示同步
  */
void KeyScan(void)
{
    static bit lock_THadd = 0;
    static bit lock_THsub = 0;
    static bit lock_TLadd = 0;
    static bit lock_TLsub = 0;

    // ---------- 高温上限 TH++ ----------
    if(KEY_THADD == 0 && !lock_THadd)
    {
        DelayMs(20);
        if(KEY_THADD == 0)
        {
            lock_THadd = 1;

            if(TH < 99) TH++;          // 防止超范围
            DS18B20_SetAlarm(TH, TL);  // 写入 EEPROM
            LCD_ShowNum(1, 11, TH, 2); // 刷新显示
        }
    }
    else if(KEY_THADD == 1) lock_THadd = 0;


    // ---------- 高温上限 TH-- ----------
    if(KEY_THSUB == 0 && !lock_THsub)
    {
        DelayMs(20);
        if(KEY_THSUB == 0)
        {
            lock_THsub = 1;

            if(TH > -55) TH--;        // DS18B20 下限为 -55℃
            DS18B20_SetAlarm(TH, TL);
            LCD_ShowNum(1, 11, TH, 2);
        }
    }
    else if(KEY_THSUB == 1) lock_THsub = 0;


    // ---------- 低温下限 TL++ ----------
    if(KEY_TLADD == 0 && !lock_TLadd)
    {
        DelayMs(20);
        if(KEY_TLADD == 0)
        {
            lock_TLadd = 1;

            if(TL < 98) TL++;         // TL 要小于 TH，留 1°C 余量可选
            DS18B20_SetAlarm(TH, TL);
            LCD_ShowNum(1, 15, TL, 2);
        }
    }
    else if(KEY_TLADD == 1) lock_TLadd = 0;


    // ---------- 低温下限 TL-- ----------
    if(KEY_TLSUB == 0 && !lock_TLsub)
    {
        DelayMs(20);
        if(KEY_TLSUB == 0)
        {
            lock_TLsub = 1;

            if(TL > -55) TL--;
            DS18B20_SetAlarm(TH, TL);
            LCD_ShowNum(1, 15, TL, 2);
        }
    }
    else if(KEY_TLSUB == 1) lock_TLsub = 0;
}
