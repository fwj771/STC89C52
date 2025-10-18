#include <REGX51.H>
#include "KeyScan.h"
#include "DelayMs.h"

// 音符频率定义
#define NOTE_C5 523
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_REST 0

sbit BUZZER = P2^5;

unsigned int Reload;

typedef struct {
	unsigned int freq;
	unsigned int duration;
} MusicNote;

// 小星星简谱
const MusicNote Song[] = {
	{NOTE_C5, 500}, {NOTE_C5, 500},
	{NOTE_G5, 500}, {NOTE_G5, 500},
	{NOTE_A5, 500}, {NOTE_A5, 500},
	{NOTE_G5, 1000}
};

unsigned char length = sizeof(Song) / sizeof(MusicNote);
unsigned char i = 0;

extern volatile bit alarmState;

void Buzzer_Init(void)
{
	TMOD &= 0x0F;	// 清除T1的控制位
	TMOD |= 0x10;	// 定时器1方式1（16位定时）
	ET1 = 1;		// 允许定时器1中断
	EA = 1;			// 允许总中断
	TR1 = 0;		// 暂停计时器1
}

void DelayMs_CheckKey(unsigned int ms)
{
    unsigned int j;
    for(j = 0; j < ms; j++)
    {
        KeyScan();          // 每1ms都检测一次按键
        if(alarmState == 0) // 如果被关闭，则立刻退出
        {
            return;
        }
        DelayMs(1);         // 原本的1ms延时
    }
}

void SetFreq(unsigned int freq)
{
	if(freq == 0)
	{
		TR1 = 0;
		BUZZER = 1;
		return;
	}
	
	Reload = 65536 - (6000000 / freq / 2);
	TH1 = Reload >> 8;
	TL1 = Reload & 0xFF;
	TR1 = 1;
}

void Timer1_ISR(void) interrupt 3	// Timer1中断号是3
{
	TH1 = Reload >> 8;
	TL1 = Reload & 0xFF;
	BUZZER = !BUZZER;
}

void PlayNote(unsigned int freq, unsigned int duration)
{
    SetFreq(freq);
    DelayMs_CheckKey(duration);

    SetFreq(0);
    DelayMs_CheckKey(50);
}

void MusicPlay(void)
{
    while(alarmState)
    {
        for(i = 0; i < length; i++)
        {
            if(alarmState == 0)
            {
                SetFreq(0);
                TR1 = 0;
                return;
            }
						PlayNote(Song[i].freq, Song[i].duration);
        }
    }
    SetFreq(0);
    TR1 = 0;
}
