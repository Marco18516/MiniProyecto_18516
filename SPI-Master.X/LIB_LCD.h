/* 
 * File: LCD.h  
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
//#ifndef __LCD_H_
//#define	__LCD_H_

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef D0
#define D0 RD0
#endif

#ifndef D1
#define D1 RD1
#endif

#ifndef D2
#define D2 RD2
#endif

#ifndef D3
#define D3 RD3
#endif

#ifndef D4
#define D4 RD4
#endif

#ifndef D5
#define D5 RD5
#endif

#ifndef D6
#define D6 RD6
#endif

#ifndef D7
#define D7 RD7
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
void Puerto(uint8_t x){
	if(x & 1){D0 = 1;}else{D0 = 0;}
    if(x & 2){D1 = 1;}else{D1 = 0;}
    if(x & 4){D2 = 1;}else{D2 = 0;}
    if(x & 8){D3 = 1;}else{D3 = 0;}
    if(x & 16){D4 = 1;}else{D4 = 0;}
    if(x & 32){D5 = 1;}else{D5 = 0;}
    if(x & 64){D6 = 1;}else{D6 = 0;}
    if(x & 128){D7 = 1;}else{D7 = 0;}
}

void LCD_CMD(char a){
    RS = 1;
    Puerto(a);
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_us(50);
}
void datosLCD(uint8_t x){
    RS = 0;
    Puerto(x);
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_ms(2);   
}
void LCD_clean(void){
    datosLCD(0);
    datosLCD(1);
}

void Lcd_Init(){
    __delay_ms(20);
    datosLCD (0x30);
    __delay_ms(5);
    datosLCD (0x30);
    __delay_us(100);
    datosLCD (0x30);
    __delay_us(100);
    datosLCD (0x38);
    __delay_us(60);
    datosLCD (0x08);
    __delay_us(60);
    datosLCD (0x01);
    __delay_ms(5);
    datosLCD (0x06);
    __delay_us(60);
    datosLCD (0x0C);     
    __delay_us(60);
}

void Lcd_Set_Cursor(uint8_t x, uint8_t y){
	uint8_t a;
	if(x == 1){
	  a = 0x80 + y;
                       
		datosLCD(a);
    }
	else if(x == 2){
	  a = 0xC0 + y;
                       
		datosLCD(a);
    }
}
void Lcd_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	   LCD_CMD(a[i]);
}