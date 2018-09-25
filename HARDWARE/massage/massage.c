#include "massage.h" 
#include "string.h"
#include "usart.h" 
#include "text.h"	
#include "id.h"

u8 Send_instruction[18];   
u8 ID_massage[18] = {0X7A,0X0F,0X01,0X03,0X28,0X00,0X23,0X00,0X17,0X47,0X37,0X30,0X34,0X36,0X31,0X32,0XEF,0X55};

void massage_flag(u8 *s, u8 z)
{
	u8 i,sum;
	
	if(heartbeat == 0)
	memset(Send_instruction,0,18);  
	for(i=0,sum=0; i<z ; i++,s++)
	{
			if(i == z-1)
				*s = 0x55;
			if(i == z-2)
				*s = sum;
			if(i > 0)
				sum = sum + *s;
			U2SendChar(*s);	
			if(heartbeat == 0)
			Send_instruction[i] = *s;
			Send_instruction_flag = 1;

	}
}

u8 massage_init()
{
	u8 i;
	Get_ChipID();
	ID_massage[16] = 0x12;
	for(i = 0; i<12 ; i++)
	{
	ID_massage[i+4] = temp[i];
	ID_massage[16] = (ID_massage[16] +  temp[i])%256;
	}
	massage_flag(ID_massage,18);	
	
	Show_Str(25,25,100,24,"run   ",24,0);
	return 0;
}





