#include "massage.h"
#include "string.h"
#include "driver.h"
#include "delay.h"
#include "usart.h"
#include "RC522.h"
#include "timer.h"
#include "iwdg.h"
#include "text.h"	
#include "main.h"
#include "RFID.h"
#include "led.h"
#include "lcd.h"
#include "key.h"   
#include "spi.h"
#include "sz.h"
#include "id.h"

#if Text_Development_board
#include "w25qxx.h"    
#include "fontupd.h"
#endif

/**-------------------------------------
����˵��

RFID�ӿ�
PC10 CLK
PC11  MISO
PC12  MOSI
PD3 	SDA
PD2  	RST
PD6 	Reset
PD7   ����
��ť˵��
key_func	����
0					KEY0
1					KEY1
2					KEY2
3					�޹���
4					KET_UP
*/
u8 current_x = 0;
u8 key_func = 3;
u8 Connect_Success_Flag = 1;	
u8 flag1 = 1;
u8 RFID_flag = 0;
u8 t = 0,f = 0;
char *Str_temp;

u8 heartbeat_massage[6] = {0x7A,0x03,0x02,0x03,0x00,0x55};
u8 dice_Point_massage[7] = {0x7A,0x04,0x03,0x02,0x00,0x00,0x55};
u8 start_dice_massage[6] = {0x7A,0x03,0x04,0x02,0x00,0x55};
u8 stop_dice_massage[6] = {0x7A,0x03,0x05,0x02,0x00,0x55};
u8 Point_massage[7] = {0x7A,0x04,0x06,0x02,0x00,0x00,0x55};

int main(void)
{	
	u8 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//����ϵͳ�ж����ȼ�����2
	delay_init(168);  														  			//��ʼ����ʱ����
	uart_init(9600);														  				//��ʼ�����ڲ�����Ϊ9600							PA9/PA10  
	uart2_init(9600);														 	  			//��ʼ�����ڲ�����Ϊ9600					    PA2/PA3  
	massage_init();													 							//��ʼ��������ID
	LED_Init();																		  			//��ʼ��LED 													PF9/PF10
	SZ_Init();																		  			//��ʼ������ 													PD7
	KEY_Init();																		  			//��ʼ����ť 													PA0/PE2/PE3/PE4
 	LCD_Init();																		  			//��ʼ��LCD														PB15/PD0,1,4,5,8,9,10,14,15/PE7~15/PF12/PG12
	RC522_SPI3_Init();														  			//��ʼ��SPI
	MFRC522_Initializtion();											  			//��ʼ��RFID
	IWDG_Init(4,500); 														  			//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
	font_init();																 					//��ʼ�����ֿ�												PB14/PG7
	Driver_Init();																				//��������ʼ��
	
	TIM2_Int_Init(500-1,8400-1); 									  			//��ʼ����ʱ��2    5000*8400=0.5s
	TIM3_Int_Init(500-1,8400-1); 									  			//��ʼ����ʱ��3    5000*8400=0.5s
	TIM8_OPM_RCR_Init(999,168-1); 												//1MHz����Ƶ��  ������+�ظ�����ģʽ  

	while(1) 																							//��ʼ����ɣ�������λ������
	{
		switch (key_func)
		{
			case 0X00:
								SZ = 0;
								delay_ms(4000);
								SZ = 1;
								delay_ms(1000);
								RFID_flag = 0;
								while(RFID_flag == 0)
								{
										LCD_Clear(WHITE);
										memset(read_rfid_my,0,16);  	
										if(rc522_read(12,read_rfid_my) == 0 )
										{
											if(t++ == 2)
											{
												t = 0;
												SZ = 0;
												delay_ms(50);
												SZ = 1;
											}
										}
										else 
										{
												Str_temp = malloc(40);
											if(read_rfid_my[0] == 7)
											sprintf(Str_temp,"%s","shitou");
											else if(read_rfid_my[0] == 8)
											sprintf(Str_temp,"%s","jiandao");
											else if(read_rfid_my[0] == 9)
											sprintf(Str_temp,"%s","bu");
											else 
												sprintf(Str_temp,"%d",read_rfid_my[0]);
												Show_Str(75,75,110,24,(u8 *)Str_temp,24,1);
												free(Str_temp);
												RFID_flag = 0;
												LED1 = ~LED1;
												RFID_flag = 1;
										}
								}
								
								for(i = 0;i<read_rfid_my[0];i++,current_x++)
								{
									if(current_x>16)
									{
										current_x = 0;
										f = ~f;
									}
										Locate_Rle(1000,2000,f);//����KEY0����500Hz��Ƶ�� ˳ʱ�뷢200����
								}
								key_func = 3;
								break;
			case 0X01:
								Locate_Rle(1000,2000,1);//����KEY0����500Hz��Ƶ�� ˳ʱ�뷢200����
								key_func = 3;
								break;
			case 0X02:
								key_func = 3;
								break;
			case 0X04:
								for(i = 0;i < 3;i++,current_x++)
								{
									if(current_x>16)
									{
										current_x = 0;
										f = 1;
									}
										Locate_Rle(1000,2000,f);//����KEY0����500Hz��Ƶ�� ˳ʱ�뷢200����
								}
								key_func = 3;
								break;
		}
		
		switch (USART2_RX[2])
		{
			case 0X03:
								memset(USART2_RX,0,30);   //�������		
								SZ = 0;
								delay_ms(4000);
								SZ = 1;
								delay_ms(1000);
								RFID_flag = 0;
								while(RFID_flag == 0)
								{
										delay_ms(500);
										LCD_Clear(WHITE);
										memset(read_rfid_my,0,16);  	
										if(rc522_read(12,read_rfid_my) == 0)
										{
												SZ = 0;
												delay_ms(50);
												SZ = 1;
										}
										else 
										{
												Str_temp = malloc(40);
											if(read_rfid_my[0] == 7)
											sprintf(Str_temp,"%s","shitou");
											else if(read_rfid_my[0] == 8)
											sprintf(Str_temp,"%s","jiandao");
											else if(read_rfid_my[0] == 9)
											sprintf(Str_temp,"%s","bu");
											else 
												sprintf(Str_temp,"%d",read_rfid_my[0]);
												Show_Str(75,75,110,24,(u8 *)Str_temp,24,1);
												free(Str_temp);
												RFID_flag = 0;
												LED1 = ~LED1;
												RFID_flag = 1;
										}
								}
								
								dice_Point_massage[4] = read_rfid_my[0];
								while(USART2_RX[2] != 0x10)
								{
								massage_flag(dice_Point_massage,7);
								delay_ms(10);
								}
								break;
			
			case 0X04:
								memset(USART2_RX,0,30);   //�������		
								massage_flag(start_dice_massage,6);
								SZ = 0;
								break;
			
			case 0X05:
								memset(USART2_RX,0,30);   //�������		
								massage_flag(stop_dice_massage,6);
								SZ = 1;
								break;
			
			case 0X06:
								memset(USART2_RX,0,30);   //�������		
								RFID_flag = 0;
								while(RFID_flag == 0)
								{
										delay_ms(500);
										LCD_Clear(WHITE);
										memset(read_rfid_my,0,16);  	
										if(rc522_read(12,read_rfid_my) == 0)
										{
												SZ = 0;
												delay_ms(50);
												SZ = 1;
										}
										else 
										{
												Str_temp = malloc(40);
											if(read_rfid_my[0] == 7)
											sprintf(Str_temp,"%s","shitou");
											else if(read_rfid_my[0] == 8)
											sprintf(Str_temp,"%s","jiandao");
											else if(read_rfid_my[0] == 9)
											sprintf(Str_temp,"%s","bu");
											else 
												sprintf(Str_temp,"%d",read_rfid_my[0]);
												Show_Str(75,75,110,24,(u8 *)Str_temp,24,1);
												free(Str_temp);
												RFID_flag = 0;
												LED1 = ~LED1;
												RFID_flag = 1;
										}
								}
								Point_massage[4] = read_rfid_my[0];
								while(USART2_RX[2] != 0x10)
								{
								massage_flag(Point_massage,7);
								delay_ms(10);
								}
								break;
			
		}		
	}

	 
}




