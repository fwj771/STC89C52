#include <REGX52.H>
#include "LCD1602.h"

// 时间变量
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;
unsigned int ms = 0; 

// 控制标志
volatile bit updateDisplay = 0; // 中断里修改，主循环要随时读
bit isRunning = 1;

// 显示缓冲区和历史记录
unsigned char displayBuffer[6] = {0};
unsigned char lastDisplay[6] = {0xFF}; // 初始化为不可能的值

// 按键定义
sbit KEY_STARTSTOP = P3^1;
sbit KEY_RESET = P3^0;

void DelayMs(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);  // 12MHz下的经验值
}

void Timer0_Isr(void) interrupt 1
{
    TL0 = 0x18;        // 重装初值(1ms@12MHz)
    TH0 = 0xFC;
    
    if(isRunning && ++ms >= 1000)   // 1000ms=1s
    {
        ms = 0;
        if(++second >= 60)
        {
            second = 0;
            if(++minute >= 60)
            {
                minute = 0;
                if(++hour >= 24)
                {
                    hour = 0;
                }
            }
        }
        
        // 更新显示缓冲区
        displayBuffer[0] = hour / 10;
        displayBuffer[1] = hour % 10;
        displayBuffer[2] = minute / 10;
        displayBuffer[3] = minute % 10;
        displayBuffer[4] = second / 10;
        displayBuffer[5] = second % 10;
        
        updateDisplay = 1;
    }
}

void Timer0_Init(void)
{
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	
	ET0=1;
	EA=1;
	PT0=0;
}

void LCD_UpdateDisplay(bit forceUpdate)
{
    // 固定文本部分只需要初始化时显示一次
    static bit firstRun = 1;
		unsigned char i = 0;

    if(firstRun) {
        LCD_ShowString(1, 1, "TIME:");
        LCD_ShowString(1, 8, "H ");
        LCD_ShowString(1, 12, "M ");
        LCD_ShowString(1, 16, "S");			 
        LCD_ShowString(2, 1, "Running ");
        firstRun = 0;
    }
    
    // 数字部分更新
    if(forceUpdate) {
        // 强制更新所有数字
        LCD_ShowNum(1, 6, displayBuffer[0], 1);
        LCD_ShowNum(1, 7, displayBuffer[1], 1);
        LCD_ShowNum(1, 10, displayBuffer[2], 1);
        LCD_ShowNum(1, 11, displayBuffer[3], 1);
        LCD_ShowNum(1, 14, displayBuffer[4], 1);
        LCD_ShowNum(1, 15, displayBuffer[5], 1);
    } else {
        // 仅更新变化的数字
        if(displayBuffer[0] != lastDisplay[0]) LCD_ShowNum(1, 6, displayBuffer[0], 1);
        if(displayBuffer[1] != lastDisplay[1]) LCD_ShowNum(1, 7, displayBuffer[1], 1);
        if(displayBuffer[2] != lastDisplay[2]) LCD_ShowNum(1, 10, displayBuffer[2], 1);
        if(displayBuffer[3] != lastDisplay[3]) LCD_ShowNum(1, 11, displayBuffer[3], 1);
        if(displayBuffer[4] != lastDisplay[4]) LCD_ShowNum(1, 14, displayBuffer[4], 1);
        if(displayBuffer[5] != lastDisplay[5]) LCD_ShowNum(1, 15, displayBuffer[5], 1);
    }    
    if(isRunning) {
        LCD_ShowString(2, 1, "Running ");
    } else {
        LCD_ShowString(2, 1, "Stopped ");
    }
    // 保存当前显示值
    for(i = 0; i < 6; i++) {
        lastDisplay[i] = displayBuffer[i];
    }
}
void KeyScan()
{
	static bit keyLock = 0;
	if(KEY_STARTSTOP == 0 && !keyLock)
	{
		DelayMs(20);  // 消抖延时
    if(KEY_STARTSTOP == 0)  // 确认有效按键
		{ 
      isRunning = !isRunning;
      updateDisplay = 1;
      keyLock = 1;  // 锁定按键
    }    
	}
	else if(KEY_STARTSTOP == 1)
	{
    keyLock = 0;  // 按键释放后解锁
  }
	
	if(KEY_RESET == 0)
	{
    DelayMs(20);
    if(KEY_RESET == 0) 
		{
			 EA = 0;  // 关闭中断
       hour = minute = second = ms = 0;
       displayBuffer[0] = displayBuffer[1] = 0;
       displayBuffer[2] = displayBuffer[3] = 0;
       displayBuffer[4] = displayBuffer[5] = 0;
			 isRunning = 1;
       LCD_UpdateDisplay(1);  // 强制更新
       EA = 1;  // 开启中断
      while(KEY_RESET == 0);  // 等待按键释放
    }
  }
	
}
void main()
{
    // 初始化
    Timer0_Init();
    LCD_Init();
    
    // 初始显示（与更新格式完全一致）
    LCD_UpdateDisplay(1);  // 强制完整显示     
    while(1)
    {
			KeyScan();
      if(updateDisplay)
      {
        updateDisplay = 0;
        LCD_UpdateDisplay(0);
      }      
    }
}