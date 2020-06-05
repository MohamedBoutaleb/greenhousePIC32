/* 
 * File:   main.c
 * Author: Alienware 13 R2
 *
 * Created on 20 settembre 2019, 10.00
 */

#include "uart.h"

static void initTimer(){
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 0b111 ; // equivale a ?8 ?Prescaler 256
    T2CONbits.T32 = 0; // use 16 - bit timer
    TMR2 = 0;
    PR2 = 39062; // 0 x4E20 // check the formula . If PBCLK is 40 MHz it produce a toggle every 500 ms.
    T2CONbits.ON = 1; 
}

static int putU4(int c){
    while(U4STAbits.UTXBF == 1);
    U4TXREG=c;
}

void putU4String(char szData[]){
    char *pData = szData;
    while (*pData)
        putU4((*(pData++)));
}

static char getU4(){
    while (!U4STAbits.URXDA) ; // wait for a new char to arrive
    return U4RXREG ; // readchar from receive buffer
}
//used to configure UART4 TX and RX
static void UARTConfigurePin(void){
    TRISFbits.TRISF12 = 0 ; //TX digital output
    RPF12R = 0b0010 ; // 0010 U4TX ? Mapping U4TX to RPF12 ;
    TRISFbits.TRISF13 = 1 ; //RX digital input
    U4RXR = 9 ;
}

static void UARTConfigurePins(int baud){
    U4MODEbits.ON=0;
    U4MODEbits.SIDL=0;
    U4MODEbits.IREN=0;
    U4MODEbits.RTSMD=0;
    U4MODEbits.UEN0=0;
    U4MODEbits.UEN1=0;
    U4MODEbits.WAKE=0;
    U4MODEbits.LPBACK=0;
    U4MODEbits.ABAUD=0;
    U4MODEbits.RXINV=0;
    U4MODEbits.PDSEL1=0;
    U4MODEbits.PDSEL0=0;
    U4MODEbits.STSEL=0;
    U4MODEbits.BRGH=0;
    UartBrg = (int)(((float) PbusClock/( 16*baud)-1)+0.5) ; // add 0.5 to round
    U4BRG=UartBrg;
    U4STAbits.UTXEN = 1 ;
    U4STAbits.URXEN = 1 ;
    U4MODEbits.ON = 1 ;
}

void readString(char input[], int length){
    int i=0, c;
    while(i!=length){
        c=getU4();
        if(c=='\r')
            break;
        input[i]=c;
        i++;
    }
}
void initUart(){
    UARTConfigurePin();    
    UARTConfigurePins(115200);
}

