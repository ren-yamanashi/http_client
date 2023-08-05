#ifndef _PARSE_URL_H
#define _PARSE_URL_H

#include "constance.h"
#include "struct.h"

char *getIpAddress(char *hostname); 
int getHostnameAndPath(Host *host, char *path, char *url);
void getPortNumber(Host *host);
#endif