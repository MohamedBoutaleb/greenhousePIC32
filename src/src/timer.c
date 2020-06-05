/* 
 * File:   timer.c
 * Author: Alienware 13 R2
 *
 * Created on 26 dicembre 2019, 0.40
 */

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

void initTimer(){
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 0b111 ; // equivale a ?8 ?Prescaler 256
    T2CONbits.T32 = 0; // use 16 - bit timer
    TMR2 = 0;
    PR2 = 39062; // 0 x4E20 // check the formula . If PBCLK is 40 MHz it produce a toggle every 500 ms.
    T2CONbits.ON = 1; 
}

void second(){
    initTimer();
    int i;
    for(i =0;i<4;i++){
        while(TMR2<(PR2-1));
        initTimer();
    }
}

void enableTimer3(){
    T3CONbits.ON = 1; 
    OC1CONbits.ON=1;
}

void disableTimer3(){
    T3CONbits.ON=0;
    OC1CONbits.ON=1;
}

