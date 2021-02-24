//******************************************************************************
//Configuración
//******************************************************************************
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Importación de librerías
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "LIB_ADC.h"
#include "SPI.h"

//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 8000000
uint8_t ADC = 0;
float T;
uint8_t volt, volt2;//variable para los voltajes en los pots
//******************************************************************************
//Funciones
//******************************************************************************
void setup(void);
void Rango(uint8_t T);
void INTER(void);
uint8_t ADC_2(void);
//******************************************************************************
// Interrupcion
//******************************************************************************
void __interrupt() isr(void){
    //Si la bandera de interrupcion es igual a 1
    if(SSPIF == 1){
        //Escribira los datos en SSPBUF
        spiWrite(T);
        //Limpiamos la bandera
        SSPIF = 0;
    }
}
//******************************************************************************
// Programación
//******************************************************************************
void main(void){
    setup();
    INTER();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
        ADC_2();//Llamamos a la funcion
        ADC = ADC_2();//Guardamos en una variable el valor de la función
        T = (ADC*1.95);//Convertimos al rango deceado
        Rango(T);//Realizamos la funcion con el valor de la variable T 
    }        
}
void Rango(uint8_t T){
    //Led verde se enciende si es menor a 25
    if (T < 25){
        PORTD = 0b00100000;}
    //Led amarillo se enciende si es mayor a 25 y menor a 36
    else if (T > 25 && T < 36){
        PORTD = 0b01000000;}
    //Led rojo se enciende si es mayor a 36
    else if (T > 36){
        PORTD = 0b10000000;}
}
//******************************************************************************
// Configuración
//******************************************************************************
//Configuracion de puertos
void setup(void){
    ANSEL = 1;
    ANSELH = 0;  
    //TRISB = 0;
    TRISA = 0;
    TRISD = 0;
    //Steo el puerto
    PORTA = 0;
    PORTD = 0;
    PORTB = 0;
   
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, 
            SPI_IDLE_2_ACTIVE);

}

void INTER(void){
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
}

uint8_t ADC_2(void){
    ADC_CHANNEL(0);//canal 0
    //Configuracion bits ADCON0
    ADCON0bits.ADCS0 = 1;//Clock ADC conversion 
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADON = 1;//Habilitamos el ADC
    __delay_ms(0.25);//Para la conversion
    ADCON0bits.GO = 1;//Inicia la conversion
    while (ADCON0bits.GO == 1){
    }
    return ADRESH;
}
