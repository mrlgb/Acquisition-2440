/*
 * global.h
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SYSTIMELENGTH 19
//4*2
//#define  POWER_CHAR_LEN 10
#define  POWER_BYTE_LEN 4
unsigned char systemtime[SYSTIMELENGTH];
//unsigned char power_string[POWER_CHAR_LEN];//

#define IP_LEN 20
#define PORT_LEN 5
#define RATE_LEN 5
 typedef struct CONFIG{
   char serverip[20];
   char port[5];
   char rate[5];
 }CONFIG_INFO;

CONFIG_INFO config;


extern void getSystemTime(unsigned char* timeString);

#endif /* GLOBAL_H_ */
