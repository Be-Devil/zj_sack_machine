#ifndef __RFID_H
#define __RFID_H	 
#include "sys.h" 

int rc522_read(u8 card_addr,u8 *RFID_DATA);
int myrc522_write(u8 card_addr,u8 *RFID_DATA);
#endif
