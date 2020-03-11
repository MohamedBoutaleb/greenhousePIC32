#include "audio.h"
#define TMR_FREQ_SINE   48000 // 48 kHz

int idxAudioBuf;
unsigned short rgSinSamples[] = {
256,320,379,431,472,499,511,507,488,453,
406,350,288,224,162,106, 59, 24,  5,  1,
 13, 40, 81,133,192
};
int rgsin_size= (sizeof(rgSinSamples) / sizeof(rgSinSamples[0]));

void initAudio(){
    TRISBbits.TRISB14=0; //audio output
    RPB14R=0xc;
    ANSELBbits.ANSB14=0;
    PR3 = (int)((float)((float)40000000/TMR_FREQ_SINE) + 0.5);
    idxAudioBuf = 0;
    OC1RS = rgSinSamples[0];
    TMR3 = 0;
    T3CONbits.TCKPS = 0;     //1:1 prescale value
    T3CONbits.TGATE = 0;     //not gated input (the default)
    T3CONbits.TCS = 0;       //PCBLK input (the default)
 
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
         // Start the OC1 module  

    IPC3bits.T3IP = 2;      // interrupt priority
    IPC3bits.T3IS = 3;      // interrupt subpriority
    IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}

void __ISR(_TIMER_3_VECTOR, ipl2) 
Timer3IntHandler(void) 
{  
    OC1RS = 4*rgSinSamples[(++idxAudioBuf) % rgsin_size];   
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
}
