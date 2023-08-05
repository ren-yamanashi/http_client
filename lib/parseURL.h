#ifndef _PARSE_URL_H
#define _PARSE_URL_H

#include "constance.h"

char *getIpAddress(char *hostname);
int getHostnameAndPathAndPort(char *hostname, int *port, char *path, char *url);

#endif