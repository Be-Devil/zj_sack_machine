#include "RFID.h"
#include "sys.h" 
#include "KEY.h" 
#include "RC522.h" 
#include "delay.h" 
#include "led.h" 

u8 WRITE_RFID[16]={0x04 ,0x02 ,0x03 ,0x04 ,0x05 ,0xD0 ,0xBF ,0xCB ,0xC4 ,0xD6 };
unsigned char read_rfid_my[16]={0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
u8 SN[4]; //卡号

u8 CardID[4];   //存放M卡类别
u8 KEY_A[6]={0xff,0xff,0xff,0xff,0xff,0xff};   	// A密钥

/***************************************************************
** 功能：   RC522读卡
** 参数：	  card_addr     要读取的数据块地址
**          RFID_DATA     存放读出数据的数组           
** 返回值：   1 读卡成功
**            0 读卡失败
****************************************************************/
int rc522_read(u8 card_addr,u8 *RFID_DATA)
{
     int status;
	   unsigned char CT[2];//卡类型
	   unsigned char READ_RFID[16]; //读卡数据 
	    u8 i;
	
	   u8 card_key=(card_addr/4)*4+3;  // 获取块对应秘钥块区
	
		Reset = 1;
//		MFRC522_Initializtion();
		delay_ms(10);
		Reset = 0;
		delay_ms(10);
	MFRC522_Initializtion();
			if(card_addr>0x3f)	 // 	 S50卡块地址0~63（0x3f）
		  {
				return 0;
		  }
	
			status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
	

			if(status==MI_OK)							//尋卡成功
		{
				status=MI_ERR;
				status = PcdAnticoll(SN);	/*防冲撞*/			 
		}

			if (status==MI_OK)						//防衝撞成功
			{
				status=MI_ERR;		
				status =PcdSelect(SN);
			}

			if(status==MI_OK)//選卡成功
			{
				status=MI_ERR;
				status =PcdAuthState(0x60,card_key,KEY_A,SN);	//验证A密钥	  一般只验证A
			}
			
			if(status==MI_OK)//驗證成功
			{
				status=MI_ERR;
				status=PcdRead(card_addr,READ_RFID);	  //读取数据
			}
			 
			if(status==MI_OK)//讀卡成功
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
	   unsigned char CT[2];//卡类型
     unsigned char SN[4]; //卡号
	
	   u8 card_key=(card_addr/4)*4+3;  // 获取块对应秘钥块区
	 
			if(card_addr>0x3f)	 // 	 S50卡块地址0~63（0x3f）
		  {
				return 0;
		  }
	
			status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
	

			if(status==MI_OK)							//尋卡成功
		{
				status=MI_ERR;
				status = PcdAnticoll(SN);	/*防冲撞*/			 
		}

			if (status==MI_OK)						//防衝撞成功
			{
				status=MI_ERR;		
				status =PcdSelect(SN);
			}

			if(status==MI_OK)//選卡成功
			{
				status=MI_ERR;
				status =PcdAuthState(0X60,card_key,KEY_A,SN);	//验证A密钥	  一般只验证A
			}
			
			if(status==MI_OK)//驗證成功
			{
				status=MI_ERR;
				status=PcdWrite(card_addr,RFID_DATA);	  //写入数据
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

















