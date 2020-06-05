/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <p32xxxx.h>
unsigned int PbusClock = 40000000;
extern unsigned int UartBrg=0 ;
void readString(char string[], int length);
void putU4String();
void initUart();
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#endif /* _EXAMPLE_FILE_NAME_H */