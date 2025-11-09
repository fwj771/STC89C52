#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "DS18B20.h"
#include "Buzzer.h"
#include "KeyScan.h"

signed char TH, TL;            // 报警阈值变量（由 DS18B20_LoadAlarm() 恢复）
unsigned int convertTimer = 0; // 转换计时器
float temperature;             // 当前温度

void main(void) {
    Buzzer_Init();
    LCD_Init();
    
    // 界面布局
    LCD_ShowString(1, 1, "T:");      // 温度显示区
    LCD_ShowString(1, 9, "TH");     // 高温报警上限
    LCD_ShowString(1, 13, "TL");    // 低温报警下限
    
    DS18B20_LoadAlarm();     // 掉电恢复报警值
    DS18B20_StartConvert();  // 开始首次转换

    // 初次显示 TH / TL
    LCD_ShowNum(1, 11, TH, 2);
    LCD_ShowNum(1, 15, TL, 2);

    while(1)
    {
        KeyScan();  // 按键实时响应（内部修改 TH、TL，并调用 DS18B20_SetAlarm）

        convertTimer++;
        DelayMs(1); // 主循环周期约 1ms
        
        // 每 750ms 读取一次温度
        if(convertTimer >= 750)
        {
            convertTimer = 0;

            temperature = DS18B20_ReadTemp(); // 获取温度
            DS18B20_StartConvert();           // 开始下一次转换

            // 温度显示（格式：T: 25.3）
            LCD_ShowNum(1,3,(int)temperature,2);            // 整数部分
            LCD_ShowChar(1,5,'.');
            LCD_ShowNum(1,6,(int)((temperature-(int)temperature)*10),1); // 小数1位

            // 显示当前报警上下限（防止按键修改后界面不更新）
            LCD_ShowNum(1, 11, TH, 2);
            LCD_ShowNum(1, 15, TL, 2);

            // 报警判断
            if(DS18B20_AlarmCheck(temperature))
            {
								LCD_ShowString(2, 1, "!!! ALARM !!!  "); 
                MusicPlay(); // 蜂鸣器响
                
            }
            else
            {
                LCD_ShowString(2, 1, "NORMAL         "); 
            }
        }
    }
}
