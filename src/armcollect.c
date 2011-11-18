/*
 * arm-collect.c
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>
//#include <math.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <sys/ioctl.h>
//#include <fcntl.h>
//#include <libxml/parser.h>
//#include <libxml/tree.h>
#include <syslog.h>  
#include <curl.h>
#include "armcollect.h"

#define CONFIG_XML "/conf/server_conf.xml"
#define COLLECT_XML "/conf/collect_conf.xml"
#define TEMP_XML "myxml.xml"

struct  data
{
  char trace_ascii; /* 1 or 0 */
};

static
void dump(const char *text,
          FILE *stream, unsigned char *ptr, size_t size,
          char nohex)
{
  size_t i;
  size_t c;

  unsigned int width=0x10;

  if(nohex)
    /* without the hex output, we can fit more on screen */
    width = 0x40;

  fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
          text, (long)size, (long)size);

  for(i=0; i<size; i+= width) {

    fprintf(stream, "%4.4lx: ", (long)i);

    if(!nohex) {
      /* hex not disabled, show it */
      for(c = 0; c < width; c++)
        if(i+c < size)
          fprintf(stream, "%02x ", ptr[i+c]);
        else
          fputs("   ", stream);
    }

    for(c = 0; (c < width) && (i+c < size); c++) {
      /* check for 0D0A; if found, skip past and start a new line of output */
      if (nohex && (i+c+1 < size) && ptr[i+c]==0x0D && ptr[i+c+1]==0x0A) {
        i+=(c+2-width);
        break;
      }
      fprintf(stream, "%c",
              (ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.');
      /* check again for 0D0A, to avoid an extra \n if it's at width */
      if (nohex && (i+c+2 < size) && ptr[i+c+1]==0x0D && ptr[i+c+2]==0x0A) {
        i+=(c+3-width);
        break;
      }
    }
    fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

extern
int parseXML(char *docname,char *rootstring,char *valuestring);

static
int my_trace(CURL *handle, curl_infotype type,char *data, size_t size,void *userp)
{
  struct data *config = (struct data *)userp;
  const char *text;
  (void)handle; /* prevent compiler warning */

  switch (type) {
  case CURLINFO_TEXT:
	getSystemTime(systemtime);
    fprintf(stderr, "== Info: %s----------time:%s", data,systemtime);

  default: /* in case a new one is introduced to shock us */
    return 0;

  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "=> Send SSL data";
    break;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    break;
  }

  dump(text, stderr, (unsigned char *)data, size, config->trace_ascii);
  return 0;
}

long getXmlFileLength(FILE * pFile)
{
	long size;

	if (pFile==NULL)
	{
		fprintf(stderr, "Open XML File error\n");
		//exit (1);
	}

	//* 获取文件大小
	fseek (pFile , 0 , SEEK_END);
	size = ftell (pFile);
	return size;
}

void prepareXmlData(FILE * pFile,long lSize ,char *buffer)
{
	//* 分配内存存储整个文件 */
	if (buffer == NULL)
	{
		fprintf(stderr, "Malloc Memory error\n");
		//exit (2);
	}

	//* 将文件拷贝到buffer中
	size_t result;
	rewind (pFile);
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize)
	{
		fprintf(stderr,"Reading XML file error\n");
		//exit (3);
	}
}

int main(int argc,char **argv)
{
	printf("----------------------------------------\n");
	int ConfigReturn=0;
	char *docname=CONFIG_XML;
	ConfigReturn=parseXML(docname,"Server","value");
	//记录到系统日志-
	if(ConfigReturn<0)
	{
	    openlog("armcollet-syslog", LOG_CONS | LOG_PID, 0);   
	    syslog(LOG_USER | LOG_INFO, "Open config xml file failue! ,(%d)",(int)ConfigReturn);   
	    closelog(); 
	}
	//记录到系统日志-
	docname=COLLECT_XML;
	ConfigReturn=parseXML(docname,"MeterOrigValue","value");
	if(ConfigReturn<0)
	{
	    openlog("armcollet-syslog", LOG_CONS | LOG_PID, 0);   
	    syslog(LOG_USER | LOG_INFO, "Open collect xml file failue! ,(%d)",(int)ConfigReturn);   
	    closelog(); 
	}
	

//	printf("%s:%s--%s\n",config.serverip,config.port,config.rate);
	getMeterAddress(adress);//获取表号
	
	//long sn=0;
	int sn=0;
	while(1)
	{
	    
		if (freopen("err.log", "w", stderr)==NULL)
			fprintf(stderr, "error redirecting stderr\n");

		TTY_INFO *ptty;
		int idx=0;
		ptty = readyTTY(idx);//打开串口
		// if(hh1>2)
		// break;

		if(ptty == NULL)
		{
			printf("readyTTY(0) error\n");
			fprintf(stderr, "Open serial com failure!---time:%s",systemtime);
			return 1;
		}

		lockTTY(ptty);//给设备加锁
		//设置串口参数
		if(setTTYSpeed(ptty,1200)>0)
		{
			printf("setTTYSpeed() error\n");
			return -1;
		}
		if(setTTYParity(ptty,8,'E',1)>0)
		{
			printf("setTTYParity() error\n");
			return -1;
		}

		//read Meter Power number.
//		printf("Begin Read Meter Power ......");
		unsigned char pow[RECV_DATA_LEN];
		readMeterPower(adress,pow,ptty);
		//int h;
//		printf("\n+++++++++++++\n");
//		for(h=0;h<RECV_DATA_LEN;h++)
//		{
//			if(h==9){printf("\n");}
//			if(h<9)
//				printf("[%d]%x",h,pow[h]);
//			else
//
//				printf("[%d]%02x",h,pow[h]);
//
//		}
//
//		printf("\n+++++++++++++\n");
//		printf("End Reading !\n");


		xmlDocPtr pdoc=NULL;
		pdoc=xmlNewDoc(BAD_CAST("1.0"));
		xmlNode* proot_node=NULL;
		proot_node=xmlNewNode(NULL,BAD_CAST("MeterOrigValue"));//建立根节点
		xmlNewProp(proot_node,NULL,NULL);
		xmlDocSetRootElement(pdoc,proot_node);//把根节点和文本联系到一起

		xmlNode*root_element=NULL;
		root_element=proot_node;

		//save xml file
		//xmlNodePtr first;

		//unsigned char adress1[6];
		int i=0;
		for(i=0;i<1;i++)
		{
			//xmlNodePtr node;
//			printf("\n[%s]-\n[%s]-\n[%s]",METER.BuildID,METER.MeterID,METER.MeterParamID);
//			340100A0010001
			xmlNewChild(root_element,NULL,BAD_CAST("BuildID"),BAD_CAST(METER.BuildID));//增加孩子节点
			xmlNodePtr valueNode=xmlNewNode(NULL,BAD_CAST("Value"));
			xmlAddChild(root_element,valueNode);
//			340100A0010001
			xmlNewChild(valueNode,NULL,BAD_CAST("MeterID"),BAD_CAST(METER.MeterID));
//			340100A001000101
			xmlNewChild(valueNode,NULL,BAD_CAST("MeterParamID"),BAD_CAST(METER.MeterParamID));
			//自定义的增加孩子节点，节点的内容为pow

			getPowerString(pow,METER.PowerStr);
//			printf("[PowerStr]:%s",METER.PowerStr);
			xmlNewChild(valueNode,NULL,BAD_CAST("OrigValue"),BAD_CAST(METER.PowerStr));

			getSystemTime(systemtime);

			xmlNewChild(valueNode,NULL,BAD_CAST("CollectTime"),BAD_CAST(systemtime));
		}
		xmlSaveFormatFileEnc("myxml.xml",pdoc,"UTF-8",1);
		//xml clean task
		xmlFreeDoc (pdoc);
		xmlCleanupParser ();
		xmlMemoryDump ();

		//open xml file to prepare send!
		FILE * pFile;
		//* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开
		pFile = fopen ("myxml.xml", "rb" );
		long lSize= getXmlFileLength(pFile);
		char *buffer = (char*) malloc (sizeof(char)*lSize);
		if(buffer == NULL)
		{
			printf("malloc buffer failure!");
		}	prepareXmlData(pFile,lSize,buffer);
		/* 关闭文件并释放内存 */
		fclose (pFile);

		const char prefix[] = "type=store&data=";
		char *dataprefix=(char*) malloc( (sizeof(char)*lSize + strlen(prefix)));
		if(dataprefix == NULL)
		{
			printf("malloc dataprefix failure!");
		}
		sprintf(dataprefix,"%s%s",prefix,buffer);
		
		//close stty
		cleanTTY(ptty);//Close stty
		//free buffer for xml file
		free (buffer);
		buffer= NULL;
		
		CURL *curl;
		CURLcode res;
		struct data curl_config;
		curl_config.trace_ascii = 1; /* enable ascii tracing */ 
		curl = curl_easy_init();
		if(curl)
		{
		    printf("\n-------Start  send to server-----\n");
		    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
		    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &curl_config);
	 
		    // the DEBUGFUNCTION has no effect until we enable VERBOSE 
		    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		    curl_easy_setopt(curl, CURLOPT_URL,"http://192.168.1.101:8888/bec/MeterOrigValueRec");
		    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataprefix);
		

		    // if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself 
		    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(dataprefix));

		    res = curl_easy_perform(curl);
		    curl_easy_cleanup(curl);
		    printf("\n");
		    printf("End of %d",sn++);
		    //记录到系统日志
		    if(res>0)
		    {
			openlog("armcollet-syslog", LOG_CONS | LOG_PID, 0);   
			syslog(LOG_USER | LOG_INFO, "curl submit ,(%d)",(int)res);   
			closelog(); 
		    }
		      
		}
		
		fclose(stderr);//close stderr
		
		//free buffer for post toserver
		free (dataprefix);
		dataprefix =NULL;
		
		//if(config.rate !=NULL)
		{
		//	sleep(atoi(config.rate));//转换
		}
		//else
		{
			sleep(5);//default 900s
		}

	}

}


