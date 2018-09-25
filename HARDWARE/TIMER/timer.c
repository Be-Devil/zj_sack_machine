#include "timer.h"
#include "led.h"
#include "key.h"
#include "main.h"
#include "RC522.h"
#include "RFID.h"
#include "lcd.h"
#include "text.h" 
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "sz.h"
#include "massage.h"
#include "iwdg.h"
#include "id.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
u16 timer=0;
u16 s_timer=0;
u16 heartbeat=0;
u8 Send_instruction_flag =0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_Cmd(TIM2,DISABLE); //使能定时器3
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,DISABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM2_IRQHandler(void)
{
	
	char *Str_temp;
		if(RFID_flag)
		{
				LCD_Clear(WHITE);
				memset(read_rfid_my,0,16);  	
			if(rc522_read(12,read_rfid_my) == 0)
			{
					SZ = 1;
					delay_ms(70);
					SZ = 0;
			}
			else 
			{
				Str_temp = malloc(40);
				sprintf(Str_temp,"%d%s",read_rfid_my[0],"点");
				Show_Str(75,75,110,48,(u8 *)Str_temp,48,1);
				free(Str_temp);
				RFID_flag = 0;
				LED1 = ~LED1;
				TIM_Cmd(TIM2,DISABLE); 
			}		
		}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	u8 a = 3;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		IWDG_Feed();															 //喂狗
		
		a =  KEY_Scan(0);
		if(a != 3)
			key_func = a;														 //按键扫描
		
		if(timer++%20 == 0)											 //心跳
		{
			LED0 = ~LED0;
			if(s_timer++%5 == 0)											 //心跳
			{
				heartbeat = 1;
				massage_flag(heartbeat_massage,6);
				heartbeat = 0;
			}
		}
		switch (timer%100)
		{
			case 0:			
						Show_Str(25,25,100,24,"run   ",24,0);
						break;
			case 30:			
						Show_Str(25,25,100,24,"run.  ",24,0);
						break;
			case 60:			
						Show_Str(25,25,100,24,"run..",24,0);
						break;
			case 90:			
						Show_Str(25,25,100,24,"run...",24,0);
						break;
		}

	}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

