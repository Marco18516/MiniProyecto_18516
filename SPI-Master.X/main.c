//*****************************************************************************
// Configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "LIB_LCD.h"
//#include "LIB_USART.h"
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
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void OSCILADOR(void);
void Baudios(void);
void config_txsta(void );
void config_rcsta(void );
void Lcd_Init(void );

char data[16];
char volt;
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
    //Se selecciona que se muestre en la primera fila de la LCD
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("S1:    S2:    S3:");
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(PORTB);
       volt = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       //Mostrara en dos decimales el voltaje de 0-5v
        sprintf(data, "%1.2f", volt);
        //Se selecciona que se muestre en la segunda fila de la LCD
        Lcd_Set_Cursor(2, 1); 
        //Muestra un string en la LCD
        Lcd_Write_String(data);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}