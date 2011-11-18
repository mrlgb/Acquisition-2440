/*
 * meter.h
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */

#ifndef METER_H_
#define METER_H_
#include <stdio.h>
#include <stdlib.h>
#include "stty.h"

#define METER_ADD_LEN 6
#define RECV_DATA_LEN 19

//电量数据偏移
#define POWER_OFFSET 2
//电量数据偏移
#define POWER_DATA_LEN 4

//整数起始偏移bytes
#define IN_DECIMAL_OFFSET 6

//整数数据长度bytes
#define OUT_INTEGER_LEN 6
//整数起始偏移bytes
#define OUT_POINT_OFFSET 6

//小数起始偏移bytes
#define OUT_DECIMAL_OFFSET 7

//小数数据长度bytes
#define OUT_DECIMAL_LEN 2

#define DefaultID_LEN 13
#define BuildID_LEN 11
#define MeterID_LEN 15
#define MeterParamID_LEN 17
#define  POWER_CHAR_LEN 10

 typedef struct Meter_t{
  char DefaultID[DefaultID_LEN];
  char BuildID[BuildID_LEN];
  char MeterID[MeterID_LEN];
  char MeterParamID[MeterParamID_LEN];
  unsigned char PowerStr[POWER_CHAR_LEN];//
}METER_INFO;

METER_INFO METER;

unsigned char adress[METER_ADD_LEN];//存放地址的数组，每行存一个

void readMeterPower(unsigned char*ad,unsigned char* pow,TTY_INFO *ptty1);

unsigned char CheckSum(unsigned char* a,int size );

void getMeterAddress(unsigned char address[METER_ADD_LEN]);

void getPowerString(unsigned char* content,unsigned char *power_string);

#endif /* METER_H_ */
