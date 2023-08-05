#ifndef _PARSE_URL_H
#define _PARSE_URL_H

#include "constance.h"
#include "struct.h"

int *getIpAddress(Host *host);
int getHostnameAndPath(Host *host, HttpRequest *request, char *url);
void getPortNumber(Host *host);
#endif