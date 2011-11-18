/*
 * stty.c
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "stty.h"

///////////////////////////////////////////////////////////////////////////////
// 初始化串口设备并进行原有设置的保存
TTY_INFO *readyTTY(int id)
{
    TTY_INFO *ptty;
    printf("\nOpen Serial......\n");
    ptty = (TTY_INFO *)malloc(sizeof(TTY_INFO));
    if(ptty == NULL)
        return NULL;
    memset(ptty,0,sizeof(TTY_INFO));
    pthread_mutex_init(&ptty->mt,NULL);//初始化互
    sprintf(ptty->name,"/dev/ttyUSB%d",id);
    printf("%s",ptty->name);
    //
    // 打开并且设置串口
    ptty->fd = open(ptty->name, O_RDWR|O_NOCTTY|O_NDELAY);
    if (ptty->fd <0)
    {
        free(ptty);
        //fprintf(stderr, "Open serial com failure!---time:%s",systemtime);
        return NULL;
    }
    else
    {  printf(" Open Success!\n");}

    return ptty;
}

///////////////////////////////////////////////////////////////////////////////
// 清理串口设备资源
int cleanTTY(TTY_INFO *ptty)
{
     // 关闭打开的串口设备
    if(ptty->fd>0)
    {
        tcsetattr(ptty->fd,TCSANOW,&ptty->otm);
        close(ptty->fd);
        ptty->fd = -1;
        free(ptty);
        ptty = NULL;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// 设置串口通信速率
// ptty 参数类型(TTY_INFO *),已经初始化的串口设备信息结构指针
// speed 参数类型(int),用来设置串口的波特率
// return 返回值类型(int),函数执行成功返回零值，否则返回大于零的值
///////////////////////////////////////////////////////////////////////////////
int setTTYSpeed(TTY_INFO *ptty, int speed)
{
    //
    // 进行新的串口设置,数据位为8位
    bzero(&ptty->ntm, sizeof(ptty->ntm));
    tcgetattr(ptty->fd,&ptty->ntm);
    ptty->ntm.c_cflag = /*CS8 |*/ CLOCAL | CREAD;

    switch(speed)
    {
    case 300:
        ptty->ntm.c_cflag |= B300;
        break;
    case 1200:
        ptty->ntm.c_cflag |= B1200;
        break;
    case 2400:
        ptty->ntm.c_cflag |= B2400;
        break;
    case 4800:
        ptty->ntm.c_cflag |= B4800;
        break;
    case 9600:
        ptty->ntm.c_cflag |= B9600;
        break;
    case 19200:
        ptty->ntm.c_cflag |= B19200;
        break;
    case 38400:
        ptty->ntm.c_cflag |= B38400;
        break;
    case 115200:
        ptty->ntm.c_cflag |= B115200;
        break;
    }
    ptty->ntm.c_iflag = IGNPAR;
    ptty->ntm.c_oflag = 0;
    //
    //
    tcflush(ptty->fd, TCIFLUSH);
    tcsetattr(ptty->fd,TCSANOW,&ptty->ntm);
    //
    //
    return 0;
}
///////////////////////////////////////////////////////////////////////////////
// 设置串口数据位，停止位和效验位
// ptty 参数类型(TTY_INFO *),已经初始化的串口设备信息结构指针
// databits 参数类型(int), 数据位,取值为7或者8
// stopbits 参数类型(int),停止位,取值为1或者2
// parity 参数类型(int),效验类型 取值为N,E,O,,S
// return 返回值类型(int),函数执行成功返回零值，否则返回大于零的值
///////////////////////////////////////////////////////////////////////////////
int setTTYParity(TTY_INFO *ptty,int databits,int parity,int stopbits)
{
    //
    // 取得串口设置
    if( tcgetattr(ptty->fd,&ptty->ntm) != 0)
    {
        printf("SetupSerial [%s]\n",ptty->name);
        return 1;
    }

    bzero(&ptty->ntm, sizeof(ptty->ntm));
    ptty->ntm.c_cflag = CS8 | CLOCAL | CREAD;
    ptty->ntm.c_iflag = IGNPAR;
    ptty->ntm.c_oflag = 0;
    //
    // 设置串口的各种参数
    ptty->ntm.c_cflag &= ~CSIZE;
    switch (databits)
    {  //设置数据位数
		case 7:
			ptty->ntm.c_cflag |= CS7;
			break;
		case 8:
			ptty->ntm.c_cflag |= CS8;
			break;
		default:
			printf("Unsupported data size\n");
			return 5;
    }
    //
    //
    switch (parity)
    { // 设置奇偶校验位数
		case 'n':
		case 'N':
			ptty->ntm.c_cflag &= ~PARENB; /* Clear parity enable */
			ptty->ntm.c_iflag &= ~INPCK; /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			ptty->ntm.c_cflag |= (PARODD|PARENB); /* 设置为奇效验*/
			ptty->ntm.c_iflag |= INPCK; /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			ptty->ntm.c_cflag |= PARENB; /* Enable parity */
			ptty->ntm.c_cflag &= ~PARODD; /* 转换为偶效验*/
			ptty->ntm.c_iflag |= INPCK; /* Disnable parity checking */
			break;
		case 'S':
		case 's': /*as no parity*/
			ptty->ntm.c_cflag &= ~PARENB;
			ptty->ntm.c_cflag &= ~CSTOPB;
			break;
		default:
			printf("Unsupported parity\n");
			return 2;
    }
    //
    // 设置停止位
    switch (stopbits)
    {
		case 1:
			ptty->ntm.c_cflag &= ~CSTOPB;
			break;
		case 2:
			ptty->ntm.c_cflag |= CSTOPB;
			break;
		default:
			printf("Unsupported stop bits\n");
			return 3;
    }
    //
    //
    ptty->ntm.c_lflag = 0;
    ptty->ntm.c_cc[VTIME] = 0;  // inter-character timer unused
    ptty->ntm.c_cc[VMIN] = 1;  // blocking read until 1 chars received
    tcflush(ptty->fd, TCIFLUSH);
    if (tcsetattr(ptty->fd,TCSANOW,&ptty->ntm) != 0)
    {
        printf("SetupSerial \n");
        return 4;
    }
    return 0;
}

int recvnTTY(TTY_INFO *ptty,unsigned char *pbuf,int size)
{
    int ret,left,bytes;
    clock_t s=clock();
    clock_t e=s;
    left = size;
    double r=0;
    while(left>0)
    {
        ret = 0;
        bytes = 0;

        pthread_mutex_lock(&ptty->mt);
        ioctl(ptty->fd, FIONREAD, &bytes);//返回有多少字节要读取，结果放在BYTES中
        if(bytes>0)
        {
            ret = read(ptty->fd,pbuf,left);
            s=clock();
        }
        else
       {
        	e=clock();
			r=((double)(e-s)/CLOCKS_PER_SEC);
			if(r>0.5)return 0;
       }
        pthread_mutex_unlock(&ptty->mt);
        if(ret >0)
        {
            left -= ret;
            pbuf += ret;//pbuf指向下次要放数据的位置,应该被改变了，变大了;
        }
        //usleep(100);//高精度的SLEEP
    }

    return size - left;//返回读取字节数
}

int sendnTTY(TTY_INFO *ptty,unsigned char *pbuf,int size)
{
    int ret,nleft;
    unsigned char *ptmp;

    ret = 0;
    nleft = size;
    ptmp = pbuf;

    while(nleft>0)
    {
        pthread_mutex_lock(&ptty->mt);
        ret = write(ptty->fd,ptmp,nleft);
        pthread_mutex_unlock(&ptty->mt);

        if(ret >0)
        {
            nleft -= ret;
            ptmp += ret;
        }
        //usleep(100);
    }

    return size - nleft;//返回写了的字节数
}

int lockTTY(TTY_INFO *ptty)
{
    if(ptty->fd < 0)
    {
        return 1;
    }

    return flock(ptty->fd,LOCK_EX);
}
int unlockTTY(TTY_INFO *ptty)
{
    if(ptty->fd < 0)
    {
        return 1;
    }

    return flock(ptty->fd,LOCK_UN);
}


