#include <REGX51.H>
#include "Buzzer.h"
#include "DelayMs.h"

#define NOTE_C5 523
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_REST 0

typedef struct {
	unsigned int freq;
	unsigned int duration;
} MusicNote;

const MusicNote LittleStar[] = {
	{NOTE_C5, 500}, {NOTE_C5, 500},
	{NOTE_G5, 500}, {NOTE_G5, 500},
	{NOTE_A5, 500}, {NOTE_A5, 500},
	{NOTE_G5, 1000}
};

unsigned char index = 0;
//计算数组 LittleStar 一共有多少个音符
unsigned char length = sizeof(LittleStar) / sizeof(MusicNote);

void MusicPlay(void)
{
	if(index < length)
	{
		SetFreq(LittleStar[index].freq);
		DelayMs(LittleStar[index].duration);
		SetFreq(0);
		DelayMs(50);
		index++;
	}
	else
	{
		index = 0; 
	}
}
