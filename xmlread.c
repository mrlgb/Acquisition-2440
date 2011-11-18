/*
 * xmlread.c
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "global.h"
#include "meter.h"
#include "xmlread.h"

void loadConfig(xmlDocPtr doc,xmlNodePtr cur)
{
 xmlChar *key=NULL;
 cur=cur->xmlChildrenNode;
 while(cur!=NULL)
 {
	//	 server_config_xml
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"ServerIP")) )
	  {
		  key=(xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
		  memcpy(config.serverip,key,20);
	  }
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"Port")) )
	  {
		   key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		   memcpy(config.port,key,5);
	  }
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"Rate")) )
	  {
		   key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		   memcpy(config.rate,key,5);
	  }
	  //	 collect_config_xml
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"DefaultID")) )
	  {
			 key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			 memcpy(METER.DefaultID,key,DefaultID_LEN);
		  }
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"BuildID")) )
	  {
			 key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
//			 printf("\nBuildID: %10s\n", key);
			 memcpy(METER.BuildID,key,BuildID_LEN);
//			 printf("\nBuildID: %10s\n", METER.BuildID);
		  }
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"MeterID")) ){
			 key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
//			 printf("\nMeterID: %10s\n", key);
			 memcpy(METER.MeterID,key,MeterID_LEN);
//			 printf("\nMeterID: %10s\n", METER.MeterID);
		  }
	  if( (!xmlStrcmp(cur->name,(const xmlChar*)"MeterParamID")) ){
			 key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
//			 printf("\nMeterParamID: %10s\n", key);
			 memcpy(METER.MeterParamID,key,MeterParamID_LEN);
//			 printf("\nMeterParamID: %10s\n", METER.MeterParamID);
		  }

//	  printf("\nkeyword: %10s\n", key);
	  xmlFree(key);
	  cur = cur->next;
 }
 return;
}

int parseXML(char *docname,char *rootstring,char *valuestring)
{
 xmlDocPtr doc;
 xmlNodePtr cur;
 xmlKeepBlanksDefault(0);
 //doc=xmlParseFile(docname);
 doc = xmlReadFile(docname, NULL, XML_PARSE_NOBLANKS|XML_PARSE_NODICT);
 if(doc==NULL){
  fprintf(stderr,"Document not parsed successfully. /n");
  return -1;
 }
 cur = xmlDocGetRootElement(doc);
 if(cur==NULL){
//  fprintf(stderr,"empty document/n");
  xmlFreeDoc(doc);
  return -1;
 }
// printf("[Root]:%s",cur->name);
 if (xmlStrcmp(cur->name, (const xmlChar *)rootstring)) {
  fprintf(stderr,"document of the wrong type, root node !=%s",rootstring);
  xmlFreeDoc(doc);
  return -1;
 }
 cur = cur->xmlChildrenNode;
 while (cur != NULL) {
//	 printf("   \n--[child]:%s",cur->name);
  if ((xmlStrcmp(cur->name, (const xmlChar *)valuestring))){
	  loadConfig (doc, cur);
  }
  cur = cur->next;
 }

 xmlFreeDoc(doc);
 xmlCleanupParser ();
 xmlMemoryDump ();
 return 0;
}



