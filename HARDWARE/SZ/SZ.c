#include "SZ.h" 

void SZ_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure1;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

  GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_7;//LED0��LED1��ӦIO��
  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure1);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}


