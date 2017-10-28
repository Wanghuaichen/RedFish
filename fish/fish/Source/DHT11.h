#ifndef _DHT11__H_
#define _DHT11__H

#include "Fish.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)
#define DHT11_Out PBout(9)
#define DHT11_In  PBin(9)

#define uchar unsigned char 
#define uint  unsigned int

unsigned char StartDHT11(void);
void DHT11_PortIN(void);
void DHT11_PortOUT(void);
u8 ReadDHT11(void);
void com(void);
//**********************************

//**********************************

extern u8 tdata[4];
extern u8 sbuf;
extern u8 check;

#endif
