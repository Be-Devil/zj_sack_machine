#include "SZ.h" 

void SZ_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure1;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_7;//LED0和LED1对应IO口
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure1);//初始化GPIO
	
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}


