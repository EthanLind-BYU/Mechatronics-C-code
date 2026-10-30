 // This program reads from an A/D channel in manual mode.

#include <xc.h>

#define THRESHOLD 1024
#define QRDL ADC1BUF0
#define QRDR ADC1BUF1
#define QRDM ADC1BUF4
//#pragma config ICS = PGx3 // For debugging
//#pragma FNOSC = FRCDIV // 8 MHz w/postscaler

void config_ad(void)
{
    
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified
                  // in AD1CSSx registers
    _SMPI = 3;	  // AD1CON2<6:2> -- number of channels sampled -1)
                  // to buffer (if sampling 10 channels)
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only
    
    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D (Auto-Sample Time Select bits)
                  // period TAD
    _ADCS = 0b00000011;    // AD1CON3<7:0> -- TAD needs to be at least 600 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock. 0b00000011 is slower than we could go, but still fast enough for our needs.

    AD1CSSL = 0b1000000010011; //chooses with A2D channels you are going to scan (here we chose 12 for ball and 1,2,4 for line following)
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D

}

int main()
{    
    // Configure analog pin 
    // Configure analog pin 2 (AN0 is shared with RA0)
    _TRISA0 = 1;    // Set as input (for left QRD)
    _ANSA0 = 1;     // Enable pin 2 for analog input (recall setting this 0 enables digital input)
    // Configure analog pin 3 (AN1 is shared with RA1)
    _TRISA1 = 1;    // Set as input (for  right QRD)
    _ANSA1 = 1;     // Enable pin 3 for analog input (recall setting this 0 enables digital input).    
    // Configure analog pin 4 (AN2 is shared with RA4)
    _TRISB0 = 1;    // Set as input (for middle QRD)
    _ANSB0 = 1;     // Enable pin 4 for analog input (recall setting this 0 enables digital input).    
    // Configure analog pin 15 (AN12 is shared with RA)
    _TRISB12 = 1;    // Set as input (for the ping pong ball collor)
    _ANSB12 = 1;     // Enable pin 15 for analog input (recall setting this 0 enables digital input).    

    config_ad();    // Configure A/D


    return 0;
}
