#include <REGX52.H>
#include "DelayMs.h"
#include "DigitalTube.h"
#include "LCD1602.h"
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

extern bit setMode; 
extern bit isCorrect;

sbit KEY_CONFIRM = P3^1;
sbit KEY_CLEAN = P3^0;
sbit KEY_SET= P3^2;

void KeyScan(void)
{
	static bit keyLockConfirm = 0;
	static bit keyLockClean = 0;
	static bit keyLockSet = 0;
	
	if(KEY_CONFIRM == 0 && !keyLockConfirm)
	{
		DelayMs(20);
		if(KEY_CONFIRM == 0)
		{
			keyLockConfirm = 1;
			checkPassword();
		}
	}
	else if(KEY_CONFIRM == 1)
	{
		keyLockConfirm = 0;
	}
	
	if(KEY_CLEAN == 0 && !keyLockClean)
	{
		DelayMs(20);
		if(KEY_CLEAN == 0)
		{
			keyLockClean = 1;
			cleanPassword();
		}
	}
	else if(KEY_CLEAN == 1)
	{
		keyLockClean = 0;
	}
	
	if(KEY_SET == 0 && !keyLockSet)
	{
		DelayMs(20);
		if(KEY_SET == 0 && isCorrect)
		{
			keyLockSet = 1;
			setMode = !setMode;
		  cleanPassword();              
			if(setMode)
				LCD_ShowString(2,1,"SET MODE");
			else
				LCD_ShowString(2,1,"        "); 
		}
	}
	else if(KEY_SET == 1)
	{
		keyLockSet = 0;
	}
}



