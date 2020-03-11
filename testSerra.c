/* 
 * File:   testSerra.c
 * Author: Alienware 13 R2
 *
 * Created on 24 dicembre 2019, 14.00
 */

#include <proc/p32mx370f512l.h>


#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include <ctype.h>
#include "acl.h"
#include <string.h>
#include "i2c.h"
#include "uart.h"
#include "adc.h"
#include <plib.h>
#include <p32xxxx.h>
#include "timer.h"
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
/* Device Config Bits in DEVCFG1 : */
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_2
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
//PBCLK=8/FPLLIDIV*FPLLMUL/FPLLODIV/FPBDIV
float axisValues[3];
int irrigationStarted=0;
int durationIrrigation=0;
int stopIrrigation=0;

void rgbLED(char color){
    LATDbits.LATD2=0;
    LATDbits.LATD3=0;
    LATDbits.LATD12=0;
    switch(color){
        case 'R':
            TRISDbits.TRISD2=0;
            ANSELDbits.ANSD2=1;
            LATDbits.LATD2=1;
            break;
        case 'B':
            TRISDbits.TRISD3=0;
            ANSELDbits.ANSD3=1;
            LATDbits.LATD3=1;
            break;
        case 'G':
            TRISDbits.TRISD12=0;
            LATDbits.LATD12=1;
            break;
    }
}

void delay(){
    unsigned int counter=10000000;
    while(counter--);
}

void welcomeMessage(){
    putsLCD("SERRA AUTOMATICA");
    newLine();
    putsLCD("Mohamed Boutaleb");
}
int checkTime(char input[]){
    return input[0]<='5'&&input[3]<='5';
}
int check(char input[]){
    if(isdigit(input[0]) && isdigit(input[1])&& input[2]==':' && isdigit(input[3]) && isdigit(input[4]))
        return checkTime(input);
    return 0;
}
void readCurrTime(char *input){
    while(1){
        putU4String("\n\rPut current time (mm:ss):");
        readString(input, 5);
        if(check(input))
            break;
        putU4String("\n\rInvalid format.");
    }
}
void shiftLC(int times){
    int i;
    for(i=0; i<times;i++)
        cmdLCD(0x13);
}
void shiftRC(int times){
    int i;
    for(i=0; i<times;i++)
        cmdLCD(0x17);
}
void setInputOutput(){
    TRISBbits.TRISB0=1;  
    TRISBbits.TRISB1=1;
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    TRISBbits.TRISB8=1;
    ANSELBbits.ANSB8=0;
    TRISFbits.TRISF0=1;
}
void plusOne(char *c){
    int n=*c-'0';
    n++;
    *c=n+'0';
}
void checkBoardStatus(){
    ACL_ReadGValues(axisValues);
    if(axisValues[2]<0.0){
        putU4String("\n\rEMERGENCY!! THE BOARD IS FLIPPED.");
        if(irrigationStarted){
            disableTimer3();
            stopIrrigation=1;
        }
        rgbLED('R');
        while(!PORTBbits.RB1);
        rgbLED('G');
        putU4String("\n\rBOARD RESTARTED.");
    }
}

void minute(){
    for(durationIrrigation =0;durationIrrigation<60;durationIrrigation++){
        putU4String(" ");
        checkBoardStatus();
        if(stopIrrigation)
            break;
        second();
    }
}
void startIrrigation(char *progTime){
    char messaggio[20];
    char messaggioFinale[50];
    sprintf(messaggio, "\n\rStart irrigazione alle %s.", progTime);
    putU4String(messaggio);
    initAudio();
    irrigationStarted=1;
    enableTimer3();
    minute();
    disableTimer3();
    sprintf(messaggioFinale, "\n\rStop irrigazione, durata di 00:%02d", durationIrrigation);
    putU4String(messaggioFinale);
}

void startComparing(char *currTime, char *progTime){
    while(strcmp(currTime, progTime)){
        currTime[5]='\0';
        progTime[5]='\0';   
        checkBoardStatus();
        putssLCD(currTime);
        newLine();
        putsLCD(progTime);
        second();
        if(currTime[4]== '9'){
            if(currTime[3]=='5'){
                if(currTime[1]=='9'){
                    if(currTime[0]=='5')
                        currTime[0]=currTime[1]=currTime[3]=currTime[4]='0';
                    else{
                        plusOne(&currTime[0]);
                        currTime[1]=currTime[3]=currTime[4]='0';
                    }
                }else{
                    plusOne(&currTime[1]);
                    currTime[3]=currTime[4]='0';
                }
            }else{
                plusOne(&currTime[3]);
                currTime[4]='0';
            }
        }else
            plusOne(&currTime[4]);
    }
    startIrrigation(progTime);
}

void main(){
    INTEnableSystemMultiVectoredInt();
    setInputOutput();
    char currTime[6], progTime[6];
    progTime[2]=':';
    initUart();
    initLCD();
    welcomeMessage();
    readCurrTime(currTime);
    currTime[5]='\0';
    putssLCD(currTime);
    rgbLED('G');
    initPotentiometer();
    initACL();
    while(1){
        checkBoardStatus();
        if(PORTFbits.RF0){
            rgbLED('B');
            newLine();
            putsLCD("PROG 00:00");
            while(1){
                checkBoardStatus();
                if(PORTBbits.RB0){
                    shiftLC(5);
                    while(!PORTAbits.RA15 && !PORTBbits.RB8){
                        checkBoardStatus();
                        char value[10];
                        sprintf(value, "%02d",timeValue());
                        putLCD(value[0]);
                        putLCD(value[1]);
                        progTime[0]=value[0];
                        progTime[1]=value[1];
                        shiftLC(2);
                    }
                    shiftRC(5);
                }
                if(PORTBbits.RB8){
                    shiftLC(2);
                    while(!PORTAbits.RA15 && !PORTBbits.RB0){
                        checkBoardStatus();
                        char value[10];
                        sprintf(value, "%02d",timeValue());
                        putLCD(value[0]);
                        putLCD(value[1]);
                        progTime[3]=value[0];
                        progTime[4]=value[1];
                        shiftLC(2);
                    }
                    shiftRC(2);
                }
                if(PORTAbits.RA15){
                    rgbLED('G');
                    break;
                }
            }
            startComparing(currTime, progTime);
        }
    }
    
}

