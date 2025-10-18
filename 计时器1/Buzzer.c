#include <REGX51.H>
#include "KeyScan.h"
#include "DelayMs.h"

// ����Ƶ�ʶ���
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

// С���Ǽ���
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
	TMOD &= 0x0F;	// ���T1�Ŀ���λ
	TMOD |= 0x10;	// ��ʱ��1��ʽ1��16λ��ʱ��
	ET1 = 1;		// ����ʱ��1�ж�
	EA = 1;			// �������ж�
	TR1 = 0;		// ��ͣ��ʱ��1
}

void DelayMs_CheckKey(unsigned int ms)
{
    unsigned int j;
    for(j = 0; j < ms; j++)
    {
        KeyScan();          // ÿ1ms�����һ�ΰ���
        if(alarmState == 0) // ������رգ��������˳�
        {
            return;
        }
        DelayMs(1);         // ԭ����1ms��ʱ
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

void Timer1_ISR(void) interrupt 3	// Timer1�жϺ���3
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
