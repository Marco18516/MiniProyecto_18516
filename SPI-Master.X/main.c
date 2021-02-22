//*****************************************************************************
// Configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = XT    // Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "LIB_LCD.h"
#include "LIB_USART.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//*****************************************************************************
// Variables
//*****************************************************************************
uint8_t cont = 0;
uint8_t ADC1 = 0;
uint8_t ADC2 = 0;
char data[20];
float volt,temp;
//*****************************************************************************
// Definición de funciones 
//*****************************************************************************
void setup(void);
void OSCILADOR(void);
void leer_ADC(void);
void Conteo(void);
float temperatura(void);
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    OSCILADOR();
    Baudios();
    config_txsta();
    config_rcsta();
    Lcd_Init();
    LCD_clean();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        Conteo();
        leer_ADC();
        LCD_clean();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1   S2   S3");
       
       //volt = ADC1*0.0196;
        temp = temperatura();
        sprintf(data, "%1.0f   %d   %3.0f" ,volt,cont,temp);
        
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(data);
        
        Write_USART_String("S1   S2   S3");
        Write_USART(13);
        Write_USART(10);
        
        Write_USART_String(data);
        Write_USART(13);
        Write_USART(10);
        __delay_ms(500);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISE = 0;
    TRISD = 0;
    //Steo los puertos
    PORTE = 0;
    PORTD = 0;
    PORTB = 0;
    
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC7 = 1;

    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, 
        SPI_IDLE_2_ACTIVE);
}

void OSCILADOR(void) {
    OSCCON = 0b01110001; //8MHz
}

void leer_ADC(void){
    PORTCbits.RC0 = 0;       //Slave Select
    __delay_ms(1);
       
    spiWrite(1);
    volt = spiRead();
       
    __delay_ms(1);
    PORTCbits.RC0 = 1;       //Slave Deselect 
    __delay_ms(1);
} 

void Conteo(void){
    PORTCbits.RC1 = 0;
    __delay_ms(1);
       
    spiWrite(1);
    cont = spiRead();
       
    __delay_ms(1);
    PORTCbits.RC1 = 1; 
    __delay_ms(1);
}   

float temperatura(void){
    PORTCbits.RC2 = 0;
    __delay_ms(1);
       
    spiWrite(1);
    temp = spiRead();
       
    __delay_ms(1);
    PORTCbits.RC2 = 1; 
    __delay_ms(1);
    return temp;
}