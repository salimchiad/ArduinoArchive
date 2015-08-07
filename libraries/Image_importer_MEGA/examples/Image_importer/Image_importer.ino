// Image Importer to Flash memory
// for CTE LCD Modules
// (C) 2013 Coldtears electronics
// For use with CTE TFT LCD Modules w/Font and Icon Flash IC
// in combination with CTE TFT/SD Shield for Arduino DUE
// web: http://stores.shop.ebay.com/coldtears-electronics-store
// For Usage, please read the PDF in the download package
// 
// This ultility requires SDfat and a compatible SDcard
// web: http://code.google.com/p/sdfatlib/
// Please use the sdfat library in the package as it is slightly modified
//

#include <SdFat.h>
#include <UTFT.h>
#include <SdFatUtil.h>
#include <Importer.h>
#include "SPI.h"


#define BVS_13 10
#define BVS_15 12
#define BVS_19 14
#define BVS_22 18
#define BVS_28 22
#define BVS_34 28
#define BVS_43 38
#define BVS_52 53
#define BVS_74 78
#define BVS_112 122

UTFT myGLCD(CTE35,38,39,40,41);   // Remember to change the model parameter to suit your display module!

// SD chip select pin
const uint8_t chipSelect = SS;

#define BUF_SIZE 256
static char name[14];
static char flash_memory_type;
uint8_t buf[BUF_SIZE];

unsigned long Location;
unsigned long address;
char IMAGE_FILENAME[] = "0.CTE";
unsigned int MAX_sector;

// file system
SdFat sd;

// test file
SdFile file;

// Serial output stream
ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------


void setup() {
  delay(1000);
  Serial.begin(38400);
  SPI_Flash_init(45);
  myGLCD.SPI_Flash_init(45);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  myGLCD.InitLCD();
  myGLCD.show_color_bar(); 
  delay(1000);
  myGLCD.clrScr();
}


void loop() {

  delay(400);  
  myGLCD.setColor(255, 255, 0);   
  myGLCD.setBackColor(0, 0, 0);   
  myGLCD.Set_character_spacing(1); 
  myGLCD.Put_Text("Image importer via SD card",5,5,BVS_28);
  char charBuf[1]; charBuf[0]=169;charBuf[1]=0;
  myGLCD.setColor(255, 0, 255);   
  myGLCD.Put_Text(charBuf,5,40,BVS_28);
  myGLCD.Put_Text(" Coldtears electronics",30,40,BVS_28);
  myGLCD.setColor(255, 255, 255); 
  
  
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
  {  
    myGLCD.Put_Text("SD Access Error",5,80,BVS_19);
  sd.initErrorHalt();
  }
  else { 
    myGLCD.Put_Text("SD Access OK",5,80,BVS_19);
  }
  delay(1000);  if (READ_ID()==0) {myGLCD.Put_Text("Flash memory Error",5,100,BVS_19); while(1){} }
  else if (READ_ID()==1) {myGLCD.Put_Text("Flash: 32Mbit,   Max Img location 799",2,100,BVS_19); flash_memory_type=1; MAX_sector=1023;}
  else if (READ_ID()==2) {myGLCD.Put_Text("Flash: 64Mbit,   Max Img location 1823",2,100,BVS_19); flash_memory_type=2; MAX_sector=2047;}
  delay(1000); 
 
  if (!file.open(IMAGE_FILENAME, O_RDWR)) {
    myGLCD.Put_Text("file open failed",5,120,BVS_19);
    error("open failed");
  }
  else{
  myGLCD.Put_Text("file open OK",5,120,BVS_19);
  }
  delay(1000); 
char temp_buf[10];
  file.read(temp_buf, sizeof(temp_buf));
  file.rewind();
  unsigned int ass=temp_buf[6]*256+temp_buf[7];
  Location=ass;                //DO NOT edit this, or else it may overwrite Font data
  address=(Location+224)*4096; //DO NOT edit this, or else it may overwrite Font data
  unsigned long filesize = file.fileSize();
  unsigned long no_of_sectors = filesize/4096;
  if (filesize%4096>0) no_of_sectors=no_of_sectors+1;
  unsigned long no_of_page = filesize/256;
  if (no_of_page%256>0) no_of_page=no_of_page+1;
 

  if (Location+224+no_of_sectors>MAX_sector)
  { myGLCD.Put_Text("Not enough space",5,120,BVS_19); while(1){}}
   
  file.getFilename(name);
  myGLCD.Put_Text((String)filesize+"bytes, Filename:",1,120,BVS_19);
  myGLCD.Put_Text(name,200,120,BVS_19);
  myGLCD.Put_Text("Img location: " + (String)(Location) + "-" + (String)(Location+no_of_sectors-1),120,80,BVS_19);
  
  delay(1000); 
  unsigned char countdown=10;
  myGLCD.setColor(255, 100, 100); 
  myGLCD.Put_Text("unplug power NOW to cancel..",1,140,BVS_22); 
  myGLCD.Put_Text(String(countdown)+"       ",280,140,BVS_22);  delay(1000); 
  while(countdown>0)
  { 
  if (countdown%2==1)   myGLCD.Put_Text("unplug power NOW to cancel..",1,140,BVS_22);
  else myGLCD.Put_Text("                                                                                          ",1,140,BVS_22);
  myGLCD.Put_Text(String(countdown)+"       ",280,140,BVS_22);
  countdown--;
  delay(1000); 
  }
   myGLCD.setColor(255, 255, 255); 
  unsigned long temp_add = address;
  unsigned long m;
  String total_sectors = " / "+ (String)(no_of_sectors-1);
  for (m=0; m<no_of_sectors;m++)
  {
   WRITE_ENABLE();	
   if (flash_memory_type==1) WRITE_STATUS_REGISTER(48); 
   else if (flash_memory_type==2) WRITE_STATUS_REGISTER(44); 
   delay(30);WRITE_ENABLE();	
   SECTOR_ERASE(temp_add);
   temp_add=temp_add+0x001000;delay(300);
   myGLCD.Put_Text("Erasing Sector: ",1,165,BVS_19);
   String displaya = String(m) + total_sectors;
   myGLCD.Put_Text(displaya,150,165,BVS_19);
  }

  temp_add = address;	
  volatile unsigned int counter=0;
  String total_page = " / "+ (String)no_of_page;
  for(unsigned long  z=0;z<no_of_page;z++)
  {		 
        unsigned int bytes_read=file.read(buf, sizeof(buf));
        if (bytes_read!=256)
         {
            for(unsigned int z=bytes_read;z<256;z++) buf[z]=0xFF;
         }   	
         WRITE_ENABLE();	
	 PAGE_PROGRAME(temp_add,buf);
         temp_add=temp_add+256;delay(20);   
         counter++;
         myGLCD.Put_Text("Writing Page: ",1,185,BVS_19);
         myGLCD.Put_Text((String)counter + total_page  ,150,185,BVS_19);
  }				

   myGLCD.Put_Text("Write File OK, Display image soon....",1,205,BVS_22);
   delay(1000); 
   WRITE_ENABLE(); WRITE_STATUS_REGISTER(252);  delay(100); 
   countdown=5;
   while(countdown>0)
  { 
    myGLCD.Put_Text(String(countdown)+"    ",300,205,BVS_22);
    countdown--;
    delay(1000); 
  }
 myGLCD.Load_image(0,0,(Location)); 
 delay(2000);
 file.close();
 Location=Location+no_of_sectors;
 address=(Location+224)*4096;
 while(1){}                                                  //disable this for batch files import
 //IMAGE_FILENAME[0]=IMAGE_FILENAME[0]++; //myGLCD.clrScr();   //enable this for batch files import
}
