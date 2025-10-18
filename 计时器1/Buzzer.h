#ifndef __Buzzer_H__
#define __Buzzer_H__

void Buzzer_Init(void);
void DelayMs_CheckKey(unsigned int ms);
void SetFreq(unsigned int freq);
void PlayNote(unsigned int freq, unsigned int duration);
void MusicPlay(void);

#endif