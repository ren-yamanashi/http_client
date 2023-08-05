#ifndef _PARSE_URL_H
#define _PARSE_URL_H

#include "constance.h"
#include "struct.h"

int *getIpAddress(Host *host);
int getHostnameAndPath(Host *host, HttpRequest *request);
void getPortNumber(Host *host);
void parseURL(Host *host, HttpRequest *request);

#endif