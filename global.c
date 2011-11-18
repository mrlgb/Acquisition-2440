/*
 * global.c
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

void getSystemTime(unsigned char* timeString)
{
	 time_t tval;
	//xmlChar* time1=(xmlChar*)calloc(size,sizeof(xmlChar));
	//printf("shixingtime_now1\n");
	struct tm* now;
	//printf("shixingtime_now2\n");
	 tval=time(NULL);
	 now=localtime(&tval);
	//printf("shixingtime_now3\n");
	//printf("Now:%4d   %d: %02d %d:%02d %02d\n",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	int year=(now->tm_year+1900);
	timeString[0]=(year/1000)+48;year=year%1000;
	timeString[1]=(year/100)+48;year=year%100;timeString[2]=(year/10)+48;timeString[3]=(year%10)+48;
	int mon=now->tm_mon+1;timeString[4]='-';timeString[5]=(mon/10)+48;timeString[6]=(mon%10)+48;
	timeString[7]='-';int day=now->tm_mday;timeString[8]=(day/10)+48;timeString[9]=(day%10)+48;
	timeString[10]=' ';int hour=now->tm_hour;timeString[11]=hour/10+48;timeString[12]=(hour%10)+48;
	timeString[13]=':';int min=now->tm_min;timeString[14]=(min/10)+48;timeString[15]=(min%10)+48;
	timeString[16]=':';int sec=now->tm_sec;timeString[17]=(sec/10)+48;timeString[18]=(sec%10)+48;
	timeString[19]='\0';
}




