#include <REGX52.H>

void DelayMs(unsigned int ms) 
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);  // 12MHz�µľ���ֵ
}
