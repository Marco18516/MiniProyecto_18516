/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __ADC_H_
#define	__ADC_H_

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
unsigned ADC_CHANNEL(unsigned short canal) 
{ //FOSC/8

    switch (canal) {
        case 0:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0; //CANAL 00

            break;
            
        default:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0; //CANAL 0
            break;
    }

}

void ADC_INIT(void) {
    ADCON1 = 0b00000000;//Justificado a la izquierda
}

#endif	/* __ADC_H_ */

