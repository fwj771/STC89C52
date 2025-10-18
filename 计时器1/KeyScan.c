#include <REGX52.H>
#include "DelayMs.h"
/**
  * @brief  KeyScanģ��
  * @param  ��
  * @retval ��
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
//			//����
//		}
//	}
//	else if(KEY_ == 1)
//	{
//		keyLock = 0;
//	}
//}

// ����
sbit KEY_STARTSTOP = P3^0;
sbit KEY_SET = P3^1;
sbit KEY_RESET = P3^2;
sbit KEY_ALARM = P3^3;

// �ⲿ��������
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

	// -------- ���ü� SET --------
	if(KEY_SET == 0 && !keyLockSet)
	{
		DelayMs(20);
		if(KEY_SET == 0)
		{
			keyLockSet = 1;

			// �����������ģʽ �� ��������ģʽ
			if(!settingMode)
			{
				settingMode = 1;
				isRunning = 0;
				settingPos = 0;
				updateDisplay = 1;
			}
			// ����Ѿ�������ģʽ �� �˳�����ģʽ
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

	// -------- ��ͣ�� START/STOP --------
	if(KEY_STARTSTOP == 0 && !keyLockStart)
	{
		DelayMs(20);
		if(KEY_STARTSTOP == 0)
		{
			keyLockStart = 1;
			if(!settingMode) // ���ڷ�����ģʽ����Ч
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
	
	// -------- ��λ�� RESET --------
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
	// -------- ���Ӽ� ALARM --------
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
