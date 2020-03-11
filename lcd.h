/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    lcd.h

  @Summary
 This file contains the define variables and function that are implemented in the lcd.c file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H
/* 
 * File:   main.c
 * Author: Alienware 13 R2
 *
 * Created on 20 settembre 2019, 10.00
 */
#include <p32xxxx.h>

#define LCDDATA 1 // RS = 1 ; access data register
#define LCDCMD 0 // RS = 0 ; access command register
#define PMDATA PMDIN // PMP data buffer
#define putLCD(d) writeLCD( LCDDATA , ( d ) )
#define cmdLCD(c) writeLCD( LCDCMD , ( c ) )
#define homeLCD() writeLCD( LCDCMD , 2)
#define clrLCD() writeLCD( LCDCMD , 1)
#define busyLCD() readLCD(LCDCMD) & 0x80

void putsLCD(char *s);
void putssLCD(char *s);
void writeLCD(int addr , char c);
char readLCD(int addr);
void newLine();
void initLCD();
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


#endif /* _EXAMPLE_FILE_NAME_H */
