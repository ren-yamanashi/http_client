#ifndef _REQUEST_H
#define _REQUEST_H

#include "struct.h"

int createRequestMessage(char *request_message, Host *host, HttpRequest *request);
int sendRequestMessage(int sock, char *request_message, unsigned int message_size);
int processRequest(int sock, Host *host, HttpRequest *request);

#endif