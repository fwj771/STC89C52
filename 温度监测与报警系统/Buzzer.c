#include <REGX51.H>
#include "Delay.h"
#include "KeyScan.h"

/*==================================*
 *           蜂鸣器配置             *
 *==================================*/
sbit BUZZER = P2^5;    // 蜂鸣器引脚（翻转输出产生声音）


/*==================================*
 *        音符定时器重装载表        *
 *   （已校准，可直接使用，无需改） *
 *==================================*/
code unsigned int NoteReload[] = {
    63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,64580,64633,
    64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,65058,65085,65110,65134,
    65157,65178,65198,65217,65235,65252,65268,65283
};


/*==================================*
 *             音符定义              *
 *==================================*/
#define NOTE_C5   13    // 523 Hz
#define NOTE_G5   20    // 784 Hz
#define NOTE_A5   22    // 880 Hz
#define NOTE_REST  0    // 休止符（静音）


/*==================================*
 *           乐谱结构体              *
 *==================================*/
typedef struct
{
    unsigned char Note;        // 音符值
    unsigned int  Duration;    // 持续时间（ms）
} MusicNote;


/*==================================*
 *            曲谱示例               *
 *==================================*/
const MusicNote Song[] = {
    {NOTE_C5, 500}, {NOTE_C5, 500},
    {NOTE_G5, 500}, {NOTE_G5, 500},
    {NOTE_A5, 500}, {NOTE_A5, 500},
    {NOTE_G5, 1000}
};

unsigned char SongLength = sizeof(Song) / sizeof(MusicNote);
unsigned int ReloadValue;


/**
  * @brief  蜂鸣器初始化（定时器1方式1）
  * @param  无
  * @retval 无
  */
void Buzzer_Init(void)
{
    TMOD &= 0x0F;     // 清 T1 控制位
    TMOD |= 0x10;     // T1 = 方式1
    ET1 = 1;          // 开 T1 中断
    EA  = 1;          // 开总中断
    TR1 = 0;          // 初始关闭
}


/**
  * @brief  设置当前音符
  * @param  Note: 音符索引
  */
void Buzzer_SetNote(unsigned char Note)
{
    if(Note == NOTE_REST)
    {
        TR1 = 0;
        BUZZER = 1;   // 静音
        return;
    }

    ReloadValue = NoteReload[Note - 1];
    TH1 = ReloadValue >> 8;
    TL1 = ReloadValue & 0xFF;
    TR1 = 1;         // 开始输出方波
}


/**
  * @brief  定时器1中断：不断翻转蜂鸣器输出
  */
void Timer1_ISR(void) interrupt 3
{
    TH1 = ReloadValue >> 8;
    TL1 = ReloadValue & 0xFF;
    BUZZER = ~BUZZER;
}


/**
  * @brief  播放单个音符
  * @param  Note: 音符
  * @param  Duration: 持续时间(ms)
  */
void Buzzer_PlayNote(unsigned char Note, unsigned int Duration)
{
    Buzzer_SetNote(Note);
    DelayMs(Duration);

    Buzzer_SetNote(NOTE_REST);
    DelayMs(50); // 音符间短暂停顿
}


/**
  * @brief  播放整首乐曲
  * @param  无
  * @retval 无
  */
void MusicPlay(void)
{
    unsigned char i;

    for(i = 0; i < SongLength; i++)
    {
        Buzzer_PlayNote(Song[i].Note, Song[i].Duration);
    }

    Buzzer_SetNote(NOTE_REST);
    TR1 = 0;   // 停止输出
}
