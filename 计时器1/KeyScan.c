#include <REGX52.H>
#include "DelayMs.h"
/**
  * @brief  KeyScan模板
  * @param  无
  * @retval 无
  */
//sbit KEY_ = P ^ ;

//void KeyScan(void)
//{
//	static bit keyLock = 0;

//	if(KEY_ == 0 && !keyLock)
//	{
//		DelayMs(20);
//		if(KEY_ == 0)
//		{
//			keyLock = 1;
//			//功能
//		}
//	}
//	else if(KEY_ == 1)
//	{
//		keyLock = 0;
//	}
//}

// 按键
sbit KEY_STARTSTOP = P3^0;
sbit KEY_SET = P3^1;
sbit KEY_RESET = P3^2;
sbit KEY_ALARM = P3^3;

// 外部变量声明
extern unsigned char hour;
extern unsigned char minute;
extern unsigned char second;
extern bit isRunning;
extern bit settingMode;
extern unsigned char settingPos;
extern volatile bit updateDisplay;
extern volatile bit alarmState;

void KeyScan(void)
{
	static bit keyLockStart = 0;
	static bit keyLockSet = 0;
	static bit keyLockReset = 0;
	static bit keyLockAlarm = 0;

	// -------- 设置键 SET --------
	if(KEY_SET == 0 && !keyLockSet)
	{
		DelayMs(20);
		if(KEY_SET == 0)
		{
			keyLockSet = 1;

			// 如果不在设置模式 → 进入设置模式
			if(!settingMode)
			{
				settingMode = 1;
				isRunning = 0;
				settingPos = 0;
				updateDisplay = 1;
			}
			// 如果已经在设置模式 → 退出设置模式
			else
			{
				settingMode = 0;
				updateDisplay = 1;
			}
		}
	}
	else if(KEY_SET == 1)
	{
		keyLockSet = 0;
	}

	// -------- 启停键 START/STOP --------
	if(KEY_STARTSTOP == 0 && !keyLockStart)
	{
		DelayMs(20);
		if(KEY_STARTSTOP == 0)
		{
			keyLockStart = 1;
			if(!settingMode) // 仅在非设置模式下有效
			{
				isRunning = !isRunning;
				updateDisplay = 1;
			}
		}
	}
	else if(KEY_STARTSTOP == 1)
	{
		keyLockStart = 0;
	}
	
	// -------- 复位键 RESET --------
	if(KEY_RESET == 0 && !keyLockReset)
	{
		DelayMs(20);
		if(KEY_RESET == 0)
		{
			keyLockReset = 1;
			if(!isRunning)
			{
				hour = minute = second = 0;
				settingPos = 0;
				updateDisplay = 1;
			}
		}
	}
	else if(KEY_RESET == 1)
	{
		keyLockReset = 0;
	}
	// -------- 闹钟键 ALARM --------
	if(KEY_ALARM == 0 && !keyLockAlarm)
	{
		DelayMs(20);
		if(KEY_ALARM == 0)
		{
			alarmState = 0;
		}
	}
	else if(KEY_ALARM == 1)
	{
		keyLockAlarm = 0;
	}
}
