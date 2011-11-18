/*
 * meter.c
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "global.h"
#include "stty.h"
#include "meter.h"

void readMeterPower(unsigned char*ad,unsigned char* pow,TTY_INFO *ptty1)
{

	unsigned char pow_command[14];
	unsigned char pow_buf[20];
	pow_command[0]=0x68;
	int j;
	for(j=1;j<7;j++)
	{
	    pow_command[j]=ad[j-1];
	}
	pow_command[7]=0x68;
	pow_command[8]=0x01;
	pow_command[9]=0x02;
	//发送数据 +033
	pow_command[10]=0x43;//0x10+0x33
	pow_command[11]=0xc3;//0x90+0x33
	pow_command[12]=CheckSum(pow_command,12);
	pow_command[13]=0x16;
	sendnTTY(ptty1,pow_command,14);
//	printf("----send bytes-----\n");
	int in=0;
		for(in=0;in<14;in++)
		{
			//printf("[%2d]%02x ",in,pow_command[in]);
		}
//		printf("----------------\n");
	int size=recvnTTY(ptty1,pow_buf,19);
	//接受数据 -0x33
	if(size!=19)
	fprintf(stderr, "Receive data error%d\n",size);
	//printf("Receive data error%d\n",size);
//	printf("---rece bytes----\n");
	for(j=0;j<19;j++)
	{
		pow[18-j]= pow_buf[j]-0x33; //pow先存高字节---逆序
	}
//	printf("----------------\n");
//	printf("+++++++++++++\n");
	for(j=0;j<19;j++)
		{
//		printf("[%2d]%02x ",j,pow[18-j]);
		}
//	printf("+++++++++++++\n");
	pow[19]='\0';

}

unsigned char CheckSum(unsigned char* a,int size )
{
    unsigned char b[40];
    int i=0;
    unsigned char sum=0x00;
    for(i=0;i<size;i++)b[i]=a[i];
    for(i=0;i<size;i++)
    {
    	sum=sum+b[i];
    }

//    printf("CHECKSUM : %d\n",sum);
    return sum;

}

void getMeterAddress(unsigned char address[METER_ADD_LEN])
{
      //int fd=open("mycode.txt",O_RDONLY);

      //read(fd,METER.DefaultID,sizeof(METER_INFO));
      printf("Meter ID: %s",METER.DefaultID);
      unsigned char AddressDstination[METER_ADD_LEN];
      int j;
      //for(i=0;i<1;i++)
      {
        for(j=0;j<METER_ADD_LEN;j++)
        {
         AddressDstination[j]= (METER.DefaultID[2*j]-48)*16+METER.DefaultID[2*j+1]-48;
         address[METER_ADD_LEN-j-1]=AddressDstination[j];
        }
      }

}

void getPowerString(unsigned char* content,unsigned char *power_string)
{
	unsigned char power_temp[POWER_BYTE_LEN];//copy表电量数据：4BYTE
	{
		memset(power_temp,0,POWER_BYTE_LEN);//
		memcpy(power_temp,content+POWER_OFFSET,POWER_DATA_LEN);//整数
	}

	int i;
	int higher,lower;

//	printf("**********(+++)*************\n");
//	int h;
//	for(h=0;h<POWER_CHAR_LEN;h++)
//	{
//			//printf("[%d]%x-",h,content[h]);
//		printf("[%d]%x ",h,power_temp[h]);
//	}
//	printf("\n***********(+++)************\n");

	unsigned char power_all[POWER_CHAR_LEN];
	for(i=0;i<POWER_BYTE_LEN;i++)
	  {
	    higher=power_temp[i]/16;
	    lower=power_temp[i]%16;
	    if(higher<=9)
	    {
	    	power_all[(i)*2]=(48+higher);
	    }
	    else
	    {
	    	power_all[(i)*2]=(97+higher);
	    }
	    if(lower<=9)
	    {
	    	power_all[(i)*2+1]=(48+lower);
	    }
	    else
	    {
	    	power_all[(i)*2+1]=(97+lower);
	    }

	  }

	memset(power_string,'\0',POWER_CHAR_LEN);//
	memcpy(power_string,power_all,OUT_INTEGER_LEN);//整数
	power_string[OUT_POINT_OFFSET]=46;//'.'
	//printf("[%c",power_string[7]);
	memcpy(power_string+OUT_DECIMAL_OFFSET,power_all+IN_DECIMAL_OFFSET,OUT_DECIMAL_LEN);//小数

//	printf("**********(1)*************\n");
//	//int h;
//	for(h=0;h<POWER_CHAR_LEN;h++)
//	{
//		//printf("[%d]%x-",h,content[h]);
//		printf("[%d]%c ",h,power_string[h]);
//	}
//
//	printf("\n***********(1)************\n");
}

