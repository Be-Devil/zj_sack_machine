#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
extern u8 WRITE_RFID[16];
extern u8 read_rfid_my[16];
extern u8 USART2_RX[100];	
extern u8 RFID_flag;
extern u8 temp[12];
extern u8 ID_massage[18];
extern u8 heartbeat_massage[6];
extern u8 Connect_Success_Flag;
extern u16 heartbeat;
extern u8 Send_instruction[18];
extern u8 Send_instruction_flag;
extern u8 key_func ;			
#endif
