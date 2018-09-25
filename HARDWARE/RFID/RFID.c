#include "RFID.h"
#include "sys.h" 
#include "KEY.h" 
#include "RC522.h" 
#include "delay.h" 
#include "led.h" 

u8 WRITE_RFID[16]={0x04 ,0x02 ,0x03 ,0x04 ,0x05 ,0xD0 ,0xBF ,0xCB ,0xC4 ,0xD6 };
unsigned char read_rfid_my[16]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
u8 SN[4]; //����

u8 CardID[4];   //���M�����
u8 KEY_A[6]={0xff,0xff,0xff,0xff,0xff,0xff};   	// A��Կ

/***************************************************************
** ���ܣ�   RC522����
** ������	  card_addr     Ҫ��ȡ�����ݿ��ַ
**          RFID_DATA     ��Ŷ������ݵ�����           
** ����ֵ��   1 �����ɹ�
**            0 ����ʧ��
****************************************************************/
int rc522_read(u8 card_addr,u8 *RFID_DATA)
{
     int status;
	   unsigned char CT[2];//������
	   unsigned char READ_RFID[16]; //�������� 
	    u8 i;
	
	   u8 card_key=(card_addr/4)*4+3;  // ��ȡ���Ӧ��Կ����
	
		Reset = 1;
//		MFRC522_Initializtion();
		delay_ms(10);
		Reset = 0;
		delay_ms(10);
	MFRC522_Initializtion();
			if(card_addr>0x3f)	 // 	 S50�����ַ0~63��0x3f��
		  {
				return 0;
		  }
	
			status = PcdRequest(PICC_REQALL,CT);/*����*/
	

			if(status==MI_OK)							//�����ɹ�
		{
				status=MI_ERR;
				status = PcdAnticoll(SN);	/*����ײ*/			 
		}

			if (status==MI_OK)						//���nײ�ɹ�
			{
				status=MI_ERR;		
				status =PcdSelect(SN);
			}

			if(status==MI_OK)//�x���ɹ�
			{
				status=MI_ERR;
				status =PcdAuthState(0x60,card_key,KEY_A,SN);	//��֤A��Կ	  һ��ֻ��֤A
			}
			
			if(status==MI_OK)//��C�ɹ�
			{
				status=MI_ERR;
				status=PcdRead(card_addr,READ_RFID);	  //��ȡ����
			}
			 
			if(status==MI_OK)//�x���ɹ�
			{
				status=MI_ERR;				 
				for(i=0;i<16;i++)
				{
					RFID_DATA[i]= READ_RFID[i]; 
				} 
				return 1;
			}
			return 0;

}


int myrc522_write(u8 card_addr,u8 *RFID_DATA)
{
       int status;
	   unsigned char CT[2];//������
     unsigned char SN[4]; //����
	
	   u8 card_key=(card_addr/4)*4+3;  // ��ȡ���Ӧ��Կ����
	 
			if(card_addr>0x3f)	 // 	 S50�����ַ0~63��0x3f��
		  {
				return 0;
		  }
	
			status = PcdRequest(PICC_REQALL,CT);/*����*/
	

			if(status==MI_OK)							//�����ɹ�
		{
				status=MI_ERR;
				status = PcdAnticoll(SN);	/*����ײ*/			 
		}

			if (status==MI_OK)						//���nײ�ɹ�
			{
				status=MI_ERR;		
				status =PcdSelect(SN);
			}

			if(status==MI_OK)//�x���ɹ�
			{
				status=MI_ERR;
				status =PcdAuthState(0X60,card_key,KEY_A,SN);	//��֤A��Կ	  һ��ֻ��֤A
			}
			
			if(status==MI_OK)//��C�ɹ�
			{
				status=MI_ERR;
				status=PcdWrite(card_addr,RFID_DATA);	  //д������
				LED1 = ~LED1;
			}

			if(status==MI_OK)
			{
		
				return 1;

			}
			else 
			{
				return 0;
			}

}

















