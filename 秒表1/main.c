#include <REGX52.H>
#include "LCD1602.h"

// ʱ�����
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;
unsigned int ms = 0; 

// ���Ʊ�־
volatile bit updateDisplay = 0; // �ж����޸ģ���ѭ��Ҫ��ʱ��
bit isRunning = 1;

// ��ʾ����������ʷ��¼
unsigned char displayBuffer[6] = {0};
unsigned char lastDisplay[6] = {0xFF}; // ��ʼ��Ϊ�����ܵ�ֵ

// ��������
sbit KEY_STARTSTOP = P3^1;
sbit KEY_RESET = P3^0;

void DelayMs(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);  // 12MHz�µľ���ֵ
}

void Timer0_Isr(void) interrupt 1
{
    TL0 = 0x18;        // ��װ��ֵ(1ms@12MHz)
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
        
        // ������ʾ������
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
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	
	ET0=1;
	EA=1;
	PT0=0;
}

void LCD_UpdateDisplay(bit forceUpdate)
{
    // �̶��ı�����ֻ��Ҫ��ʼ��ʱ��ʾһ��
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
    
    // ���ֲ��ָ���
    if(forceUpdate) {
        // ǿ�Ƹ�����������
        LCD_ShowNum(1, 6, displayBuffer[0], 1);
        LCD_ShowNum(1, 7, displayBuffer[1], 1);
        LCD_ShowNum(1, 10, displayBuffer[2], 1);
        LCD_ShowNum(1, 11, displayBuffer[3], 1);
        LCD_ShowNum(1, 14, displayBuffer[4], 1);
        LCD_ShowNum(1, 15, displayBuffer[5], 1);
    } else {
        // �����±仯������
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
    // ���浱ǰ��ʾֵ
    for(i = 0; i < 6; i++) {
        lastDisplay[i] = displayBuffer[i];
    }
}
void KeyScan()
{
	static bit keyLock = 0;
	if(KEY_STARTSTOP == 0 && !keyLock)
	{
		DelayMs(20);  // ������ʱ
    if(KEY_STARTSTOP == 0)  // ȷ����Ч����
		{ 
      isRunning = !isRunning;
      updateDisplay = 1;
      keyLock = 1;  // ��������
    }    
	}
	else if(KEY_STARTSTOP == 1)
	{
    keyLock = 0;  // �����ͷź����
  }
	
	if(KEY_RESET == 0)
	{
    DelayMs(20);
    if(KEY_RESET == 0) 
		{
			 EA = 0;  // �ر��ж�
       hour = minute = second = ms = 0;
       displayBuffer[0] = displayBuffer[1] = 0;
       displayBuffer[2] = displayBuffer[3] = 0;
       displayBuffer[4] = displayBuffer[5] = 0;
			 isRunning = 1;
       LCD_UpdateDisplay(1);  // ǿ�Ƹ���
       EA = 1;  // �����ж�
      while(KEY_RESET == 0);  // �ȴ������ͷ�
    }
  }
	
}
void main()
{
    // ��ʼ��
    Timer0_Init();
    LCD_Init();
    
    // ��ʼ��ʾ������¸�ʽ��ȫһ�£�
    LCD_UpdateDisplay(1);  // ǿ��������ʾ     
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