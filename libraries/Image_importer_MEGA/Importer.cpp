#include "Importer.h"
#include <pins_arduino.h>
#include <avr/pgmspace.h>
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
volatile uint8_t  * P_F_CS2;
uint8_t  B_F_CS2;
		
void SPI_Flash_init(int CSpin)
{
		P_F_CS2	= portOutputRegister(digitalPinToPort(CSpin));
		B_F_CS2	= digitalPinToBitMask(CSpin);
		pinMode(CSpin,OUTPUT);
		sbi(P_F_CS2, B_F_CS2);
                  pinMode(5,OUTPUT);
                  digitalWrite(5,HIGH);
}

unsigned char READ_ID()
{
	  unsigned char ID_0,ID_1;			
            cbi(P_F_CS2, B_F_CS2);
				delayMicroseconds(10);
                SPI_WriteByte(0x90);
				SPI_WriteByte(0x00);
				SPI_WriteByte(0x00);
				SPI_WriteByte(0x00);
				delayMicroseconds(10);
 
                ID_0=(SPI_ReadByte());
                ID_1=(SPI_ReadByte());
                sbi(P_F_CS2, B_F_CS2);
                if (ID_1==0x15) return 1;
                else if (ID_1==0x16) return 2;
                else return 0;
}

void SPI_WriteByte(byte data)
{
  SPDR = data;                                     // ??????
  while(!(SPSR & (1<<SPIF)));                      //??????
}

byte SPI_ReadByte(void)
{
  SPDR = 0x00;
  while(!(SPSR & (1<<SPIF)));                      // ??????
  return SPDR;                                     //????
}
void WRITE_DISABLE()
{
		cbi(P_F_CS2, B_F_CS2);
              SPI_WriteByte(0x04);
		  sbi(P_F_CS2, B_F_CS2);
Serial.println("Write DIS");

}
void WRITE_ENABLE()
{
		cbi(P_F_CS2, B_F_CS2);
SPI_WriteByte(0x06);
		  sbi(P_F_CS2, B_F_CS2);

}
void WRITE_STATUS_REGISTER(unsigned int temp1)
{
						char C = temp1>>8;
						cbi(P_F_CS2, B_F_CS2);
 						SPI_WriteByte(0x01);
 						SPI_WriteByte(temp1);
						SPI_WriteByte(C); 
						sbi(P_F_CS2, B_F_CS2);
					delayMicroseconds(10);

}
void SECTOR_ERASE(unsigned long add)
{
						char H = add>>16;
						char M = add>>8;
 								cbi(P_F_CS2, B_F_CS2);
 						SPI_WriteByte(0x20);
 						SPI_WriteByte(H);
 						SPI_WriteByte(M);	 					
						SPI_WriteByte(add);
								sbi(P_F_CS2, B_F_CS2);
						delayMicroseconds(10);
	Serial.print(" SECTOR_ERASE: ");
Serial.println(add,HEX);
}
void PAGE_PROGRAME(unsigned long add1, unsigned char* buf)
{
						char H1 = add1>>16;
						char M1 = add1>>8;
 								cbi(P_F_CS2, B_F_CS2);
 						SPI_WriteByte(0x02);
 						SPI_WriteByte(H1);
 						SPI_WriteByte(M1);
 						SPI_WriteByte(0);
						for (unsigned int k=0; k<256; k++)
 						{
							SPI_WriteByte(buf[k]);
  						}
								sbi(P_F_CS2, B_F_CS2);

		Serial.print(" PAGE_PROGRAME");	
Serial.println(add1,HEX);		
}
 

void READ_BYTES(unsigned long add)
{
// 	
	
						cbi(P_F_CS2, B_F_CS2);
						delayMicroseconds(1);
						char H = add>>16;
						char M = add>>8;
 						SPI_WriteByte(0x03);
 						SPI_WriteByte(H);
 						SPI_WriteByte(M);
 						SPI_WriteByte(0);
 						for (unsigned int k=0; k<1024; k++)
 						{
 							
   //unsigned char temp =SPI_ReadByte();
  // PrintHex8(&temp,1);
 						}
 			 			sbi(P_F_CS2, B_F_CS2);
}