#ifndef _PARSE_URL_H
#define _PARSE_URL_H

char *getIpAddress(char *hostname);
int getHostnameAndPathAndPort(char *hostname, int *port, char *path, char *url);

#endif