#include <REGX52.H>
#include "DelayMs.h"

// 矩阵键盘引脚定义
sbit ROW1 = P1^7;
sbit ROW2 = P1^6;
sbit ROW3 = P1^5;
sbit ROW4 = P1^4;
sbit COL1 = P1^3;
sbit COL2 = P1^2;
sbit COL3 = P1^1;
sbit COL4 = P1^0;

// 键盘映射（4x4 矩阵）
unsigned char code KeyMap[4][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11},
    {12, 13, 14, 15}
};

void MatrixKey_Init()
{
    // 初始化所有行为高电平，列为高阻态
    P1 = 0xFF;
}

unsigned char MatrixKey_Scan()
{
    // 扫描第一列（COL1=0）
    COL1 = 0; COL2 = 1; COL3 = 1; COL4 = 1;
    if (ROW1 == 0) { DelayMs(5); if (ROW1 == 0) { while (ROW1 == 0); return KeyMap[0][0]; } }
    if (ROW2 == 0) { DelayMs(5); if (ROW2 == 0) { while (ROW2 == 0); return KeyMap[1][0]; } }
    if (ROW3 == 0) { DelayMs(5); if (ROW3 == 0) { while (ROW3 == 0); return KeyMap[2][0]; } }
    if (ROW4 == 0) { DelayMs(5); if (ROW4 == 0) { while (ROW4 == 0); return KeyMap[3][0]; } }
    
    // 扫描第二列（COL2=0）
    COL1 = 1; COL2 = 0; COL3 = 1; COL4 = 1;
    if (ROW1 == 0) { DelayMs(5); if (ROW1 == 0) { while (ROW1 == 0); return KeyMap[0][1]; } }
    if (ROW2 == 0) { DelayMs(5); if (ROW2 == 0) { while (ROW2 == 0); return KeyMap[1][1]; } }
    if (ROW3 == 0) { DelayMs(5); if (ROW3 == 0) { while (ROW3 == 0); return KeyMap[2][1]; } }
    if (ROW4 == 0) { DelayMs(5); if (ROW4 == 0) { while (ROW4 == 0); return KeyMap[3][1]; } }
    
    // 扫描第三列（COL3=0）
    COL1 = 1; COL2 = 1; COL3 = 0; COL4 = 1;
    if (ROW1 == 0) { DelayMs(5); if (ROW1 == 0) { while (ROW1 == 0); return KeyMap[0][2]; } }
    if (ROW2 == 0) { DelayMs(5); if (ROW2 == 0) { while (ROW2 == 0); return KeyMap[1][2]; } }
    if (ROW3 == 0) { DelayMs(5); if (ROW3 == 0) { while (ROW3 == 0); return KeyMap[2][2]; } }
    if (ROW4 == 0) { DelayMs(5); if (ROW4 == 0) { while (ROW4 == 0); return KeyMap[3][2]; } }
    
    // 扫描第四列（COL4=0）
    COL1 = 1; COL2 = 1; COL3 = 1; COL4 = 0;
    if (ROW1 == 0) { DelayMs(5); if (ROW1 == 0) { while (ROW1 == 0); return KeyMap[0][3]; } }
    if (ROW2 == 0) { DelayMs(5); if (ROW2 == 0) { while (ROW2 == 0); return KeyMap[1][3]; } }
    if (ROW3 == 0) { DelayMs(5); if (ROW3 == 0) { while (ROW3 == 0); return KeyMap[2][3]; } }
    if (ROW4 == 0) { DelayMs(5); if (ROW4 == 0) { while (ROW4 == 0); return KeyMap[3][3]; } }
    
    return 0xFF; // 无按键按下
}