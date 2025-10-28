#include <REGX52.H>
#include "Timer.h"
#include "LCD1602.h"
#include "DelayMs.h"
#include "KeyScan.h"
#include "MatrixKey.h"
#include "DigitalTube.h"

bit setMode = 0; 

void main()
{
	Timer0_Init();
	MatrixKey_Init();
	LCD_Init();
	LCD_ShowString(1, 1, "-PASSWORD-");
	while(1)
	{ 
		DigDisplay();		
		DigitalInput();
		KeyScan();	
		if(setMode)
		{
			setPassword();
		}
	}
}
