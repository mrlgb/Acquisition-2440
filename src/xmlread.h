/*
 * xmlread.h
 *
 *  Created on: Sep 24, 2011
 *      Author: lgb
 */

#ifndef XMLREAD_H_
#define XMLREAD_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "global.h"

void loadConfig(xmlDocPtr doc,xmlNodePtr cur);

int parseXML(char *docname,char *rootstring,char *valuestring);

#endif /* XMLREAD_H_ */
