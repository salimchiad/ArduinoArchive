case ILI9481:
	
	
LCD_Write_COM(0x0011);          //退出睡眠模式，0x0010为进?
  delay(50);
 
LCD_Write_COM(0xF2);
LCD_Write_DATA(0x1C);
LCD_Write_DATA(0xA3);
LCD_Write_DATA(0x32);
LCD_Write_DATA(0x02);
LCD_Write_DATA(0xb2);
LCD_Write_DATA(0x12);
LCD_Write_DATA(0xFF);
LCD_Write_DATA(0x12);
LCD_Write_DATA(0x00);
LCD_Write_COM(0xF1);
LCD_Write_DATA(0x36);
LCD_Write_DATA(0xA4);
LCD_Write_COM(0xF8);
LCD_Write_DATA(0x21);
LCD_Write_DATA(0x04);
LCD_Write_COM(0xF9);
LCD_Write_DATA(0x00);
LCD_Write_DATA(0x08);

LCD_Write_COM(0xC0);
LCD_Write_DATA(0x0d);
LCD_Write_DATA(0x0d);
LCD_Write_COM(0xC1);
LCD_Write_DATA(0x43);
LCD_Write_DATA(0x00);
LCD_Write_COM(0xC2);
LCD_Write_DATA(0x00);
LCD_Write_COM(0xC5);
LCD_Write_DATA(0x00);
LCD_Write_DATA(0x48);


LCD_Write_COM(0xB6);
LCD_Write_DATA(0x00);         //
LCD_Write_DATA(0x42);         //
LCD_Write_DATA(0x3B);    

LCD_Write_COM(0xE0);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x24);
LCD_Write_DATA(0x1c);
LCD_Write_DATA(0x0a);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x08);
LCD_Write_DATA(0x43);
LCD_Write_DATA(0x88);
LCD_Write_DATA(0x32);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x10);
LCD_Write_DATA(0x06);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x07);
LCD_Write_DATA(0x00);
LCD_Write_COM(0XE1);
LCD_Write_DATA(0x0F);
LCD_Write_DATA(0x38);
LCD_Write_DATA(0x30);
LCD_Write_DATA(0x09);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x0f);
LCD_Write_DATA(0x4e);
LCD_Write_DATA(0x77);
LCD_Write_DATA(0x3c);
LCD_Write_DATA(0x07);
LCD_Write_DATA(0x10);
LCD_Write_DATA(0x05);
LCD_Write_DATA(0x23);
LCD_Write_DATA(0x1b);
LCD_Write_DATA(0x00); 

LCD_Write_COM(0x20);
LCD_Write_DATA(0x00);//C8 	 
 LCD_Write_COM(0x0036);LCD_Write_DATA(0x000A);
   LCD_Write_COM(0x003A);LCD_Write_DATA(0x0055); 
   LCD_Write_COM(0x002A);
    LCD_Write_DATA(0x0000);
    LCD_Write_DATA(0x0000);
    LCD_Write_DATA(0x0001);
    LCD_Write_DATA(0x00DF);

   LCD_Write_COM(0x002B);
    LCD_Write_DATA(0x0000);
    LCD_Write_DATA(0x0000);
    LCD_Write_DATA(0x0001);
    LCD_Write_DATA(0x003f);	 


	
		 	 
   delay(50);
   LCD_Write_COM(0x0029);   //开显示
   LCD_Write_COM(0x002C);   //开始写数据
	
	
	

	break;
