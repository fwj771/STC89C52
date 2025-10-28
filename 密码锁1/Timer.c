#include <REGX52.H>
extern bit flashFlag;
/**
  * @brief  定时器0初始化 1ms
  * @param  无
  * @retval 无
  */
void Timer0_Init(void)
{
  TMOD &= 0xF0;     // 清除T0的控制位
  TMOD |= 0x01;     // 设置T0为模式1（16位定时器）
  TL0 = 0x18;       // 设置定时初值（1ms@12MHz）
  TH0 = 0xFC;       // 设置定时初值
  TF0 = 0;          // 清除溢出标志
  TR0 = 1;          // 启动定时器0
  
  ET0 = 1;          // 允许定时器0中断
  EA = 1;           // 开启总中断
  PT0 = 0;          // 设置定时器0中断优先级为低
	
}

/**
  * @brief  定时器0中断服务函数模板
  * @param  无
  * @retval 无
  */
//void Timer0_Isr(void) interrupt 1
//{
//	static unsigned int msCount = 0;

//	TL0 = 0x18;
//	TH0 = 0xFC;

//	if(++msCount >= 1000) 1秒
//	{
//		msCount = 0;
//    功能
//	}

//}
void Timer0_Isr(void) interrupt 1
{
	static unsigned int msCountFlash = 0;

	TL0 = 0x18;
	TH0 = 0xFC;

	if(++msCountFlash >= 500) 
	{
		msCountFlash = 0;
    flashFlag = !flashFlag;
	}

}
