#include <REGX52.H>
#include "LCD1602.h"
#include "MatrixKey.h"
#include "DelayMs.h"
#include "KeyScan.h"
#include "Timer.h"
#include "Buzzer.h"

// ---------------------- ȫ�ֱ��� ----------------------
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;

sbit BUZZER = P2^5;     // ��������������

// ���Ʊ�־
volatile bit updateDisplay = 0;
bit isRunning = 0;
bit settingMode = 0;
unsigned char settingPos = 0; // 0~5 ��Ӧ ʱʮ��ʱ������ʮ���ָ�����ʮ�����
volatile bit alarmState = 0;

// ��˸���
volatile bit blinkState = 0;

// ��־λ�������жϺ�ʱ��
volatile bit flag_1s = 0;
volatile bit flag_500ms = 0;

// ---------------------- �������� ----------------------
void Display(void)
{
	LCD_ShowString(1, 1, "Time:");
	LCD_ShowNum(1, 6, hour, 2);
	LCD_ShowString(1, 8, "H ");
	LCD_ShowNum(1, 10, minute, 2);
	LCD_ShowString(1, 12, "M ");
	LCD_ShowNum(1, 14, second, 2);
	LCD_ShowString(1, 16, "S");
}

// ��������
void SettingInput(void)
{
	unsigned char keyNum = MatrixKey_Scan();

	if(keyNum != 0xFF)
	{
		if(keyNum <= 9)
		{
			switch(settingPos)
			{
				case 0:
					hour = keyNum * 10 + (hour % 10);
					break;
				case 1:
					hour = (hour / 10) * 10 + keyNum;
					break;
				case 2:
					minute = keyNum * 10 + (minute % 10);
					break;
				case 3:
					minute = (minute / 10) * 10 + keyNum;
					if(minute > 59) minute = 59;
					break;
				case 4:
					second = keyNum * 10 + (second % 10);
					break;
				case 5:
					second = (second / 10) * 10 + keyNum;
					if(second > 59) second = 59;
				  blinkState = 0;    //�����ո�
				  settingMode = 0;
					break;
			}
			if(settingPos < 5) settingPos++;
			updateDisplay = 1;
		}
	}
}

// ������˸״̬��ʾ��ǰλ
void BlinkCurrentPos(void)
{
	if(!blinkState)
	{
		// ��������ʾ״̬
		Display();
	}
	else
	{
		// ��˸���ص�ǰλ
		switch(settingPos)
		{
			case 0: LCD_ShowString(1, 6, " "); break;
			case 1: LCD_ShowString(1, 7, " "); break;
			case 2: LCD_ShowString(1, 10, " "); break;
			case 3: LCD_ShowString(1, 11, " "); break;
			case 4: LCD_ShowString(1, 14, " "); break;
			case 5: LCD_ShowString(1, 15, " "); break;
		}
	}
}

// ����ʱ�߼�
void CountDown(void)
{
	if(second > 0)
		second--;
	else
	{
		if(minute > 0)
		{
			minute--;
			second = 59;
		}
		else
		{
			if(hour > 0)
			{
				hour--;
				minute = 59;
				second = 59;
			}
			else
			{
				isRunning = 0; // ���� 00:00:00 ֹͣ
			  alarmState = 1;
			}
		}
	}
	updateDisplay = 1;
}

// ---------------------- ������ ----------------------
void main(void)
{
	MatrixKey_Init();
	Timer0_Init();
	LCD_Init();
	Display();
	Buzzer_Init();
	while(1)
	{
		KeyScan();

		if(settingMode)
		{
			SettingInput();
			BlinkCurrentPos();
		}
		else if(updateDisplay)
		{
			Display();
		}

		updateDisplay = 0;

		// ÿ�����һ�ε���ʱ
		if(flag_1s)
		{
			flag_1s = 0;
			if(isRunning) CountDown();
		}

		// 500ms ��˸����
		if(flag_500ms)
		{
			flag_500ms = 0;
			blinkState = !blinkState;
		}
		
		if(alarmState)
		{
			MusicPlay();
		}
	}
}