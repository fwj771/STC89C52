#include <REGX52.H>


// �ⲿ��������
extern volatile bit flag_1s;
extern volatile bit flag_500ms;
extern volatile bit blinkState;


/**
  * @brief  ��ʱ��0��ʼ�� 1ms
  * @param  ��
  * @retval ��
  */
void Timer0_Init(void)
{
  TMOD &= 0xF0;     // ���T0�Ŀ���λ
  TMOD |= 0x01;     // ����T0Ϊģʽ1��16λ��ʱ����
  TL0 = 0x18;       // ���ö�ʱ��ֵ��1ms@12MHz��
  TH0 = 0xFC;       // ���ö�ʱ��ֵ
  TF0 = 0;          // ��������־
  TR0 = 1;          // ������ʱ��0
  
  ET0 = 1;          // ����ʱ��0�ж�
  EA = 1;           // �������ж�
  PT0 = 0;          // ���ö�ʱ��0�ж����ȼ�Ϊ��
	
}

/**
  * @brief  ��ʱ��0�жϷ�����ģ��
  * @param  ��
  * @retval ��
  */
//void Timer0_Isr(void) interrupt 1
//{
//	static unsigned int msCount = 0;

//	TL0 = 0x18;
//	TH0 = 0xFC;

//	if(++msCount >= 1000) 1��
//	{
//		msCount = 0;
//    ����
//	}

//}
void Timer0_Isr(void) interrupt 1
{
	static unsigned int msCount = 0;
	static unsigned int blinkCount = 0;

	TL0 = 0x18;
	TH0 = 0xFC;

	if(++msCount >= 1000)
	{
		msCount = 0;
		flag_1s = 1;  // ÿ�봥��һ��
	}

	if(++blinkCount >= 500) 
	{
		blinkCount = 0;
		flag_500ms = 1; // ÿ500ms������˸
	}
	       
}
