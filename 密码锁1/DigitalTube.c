#include <REGX52.H>
#include "DelayMs.h"
#include "MatrixKey.h"
#include "LCD1602.h"

sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

unsigned char code digitalTube[17] = {
  0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
  0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x08//下划线
}; // 段码表

unsigned char i, j;
unsigned char DisplayNum[8] = {16,16,16,16,16,16,16,16};	
unsigned char Password[8]   = {1,2,3,4,5,6,7,8};
unsigned char DigitalPos = 0;

bit isCorrect = 0;
bit flashFlag = 1;
extern bit setMode;
void DigDisplay()
{
	for(i=0;i<8;i++)
	{
		switch(i)
		{
			case 0: LSA=1;LSB=1;LSC=1;break;
			case 1: LSA=0;LSB=1;LSC=1;break;
			case 2: LSA=1;LSB=0;LSC=1;break;
			case 3: LSA=0;LSB=0;LSC=1;break;
			case 4: LSA=1;LSB=1;LSC=0;break;
			case 5: LSA=0;LSB=1;LSC=0;break;
			case 6: LSA=1;LSB=0;LSC=0;break;
			case 7: LSA=0;LSB=0;LSC=0;break;
		}
		if(i == DigitalPos && DisplayNum[i] == 16) // 当前输入位闪烁
		{
			P0 = flashFlag ? digitalTube[16] : 0x00;
		}
		else
		{
			P0 = digitalTube[DisplayNum[i]];
		}		
		DelayMs(2);
		P0 = 0x00;
	}
}

void cleanPassword()
{
	for(j=0; j<8; j++)
	{
		DisplayNum[j] = 16;
	}
	DigitalPos = 0;
	LCD_ShowString(2, 1, "        "); 
}

void checkPassword()
{
	if(DigitalPos < 8)
  {
    LCD_ShowString(2, 1, "INPUT..."); 
    return;
  }
	isCorrect = 0;  
	for(j=0; j<8; j++)
	{
		if(Password[j] != DisplayNum[j])
		{
			break;
		}
		if(j == 7)
		{
			isCorrect = 1;
		}
	}

	if(isCorrect)
	{
		cleanPassword();
		LCD_ShowString(2, 1, "ENTER   ");
	}
	else
	{
		cleanPassword();
		LCD_ShowString(2, 1, "WRONG   ");
	}
}


void DigitalInput()
{
	unsigned char Num = MatrixKey_Scan();

	if(Num != 0xFF && DigitalPos < 8)
	{
		if(Num <= 9)
		{
			DisplayNum[DigitalPos] = Num;			
			if(DigitalPos < 8) DigitalPos++;
		}
	}
}

void setPassword()
{
    static unsigned char tempPassword[8]; 
    static bit firstInput = 0;    
	
    if(DigitalPos == 8) 
    {
        if(firstInput == 0)
        {
            for(j = 0; j < 8; j++)
            {
                tempPassword[j] = DisplayNum[j];
            }
            cleanPassword();
            LCD_ShowString(2, 1, "AGAIN!  "); 
            firstInput = 1;
        }
        else
        {
            bit match = 1;
            for(j = 0; j < 8; j++)
            {
                if(tempPassword[j] != DisplayNum[j])
                {
                    match = 0;
                    break;
                }
            }

            cleanPassword();

            if(match)
            {
                for(j = 0; j < 8; j++)
                {
                    Password[j] = tempPassword[j];
                }
                LCD_ShowString(2, 1, "SAVED!  ");
                setMode = 0;
            }
            else
            {
                LCD_ShowString(2, 1, "NOT SAME");
            }
            firstInput = 0;
        }
    }
}
