//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include "ADC_LIB.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
char volt;
uint8_t ADC = 0;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint8_t ADC_1(void);//
void INTER(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
    //Si la bandera de interrupcion es igual a 1
   if(SSPIF == 1){
        //Escribira los datos en SSPBUF
        spiWrite(ADC);
        //Limpiamos la bandera
        SSPIF = 0;
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    INTER();
    ADC_INIT();
    
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       ADC_1(); //Llamamos a la función
       ADC = ADC_1();//Guardamos en una variable el valor de la función
       PORTD = ADC;//La mostramos en el puerte D
        __delay_ms(1);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
//Configuración de Puertos
void setup(void){
    ANSEL = 1;
    ANSELH = 0;
    TRISA = 1;
    TRISB = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
   //Configuracion necesaria para slave
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
//Funcion para conversion
uint8_t ADC_1(void) {
    ADC_CHANNEL(0); //canal 0
    //Cinfiguracion bits ADCON0
    ADCON0bits.ADCS0 = 1;//Clock ADC conversion
    ADCON0bits.ADCS1 = 0;//Fosc/2
    ADCON0bits.ADON = 1;//Se habilita el ADC
    __delay_ms(0.25);//Para conversion
    ADCON0bits.GO = 1;  //Inicia la conversión
    while (ADCON0bits.GO == 1) {
        //volt = ADRESH; //Prueba desplegar ADRESH en volt
        //PORTD = volt;
        return  ADRESH;     
    }
}

//Configuracion interrupcion
void INTER(void){
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    
}
