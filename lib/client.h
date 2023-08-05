#ifndef _CLIENT_H
#define _CLIENT_H

#include "constance.h"
#include "struct.h"

void showMessage(char *message, unsigned int size);
int recvResponseMessage(int sock, char *response_message, unsigned int buffer_size);
int saveBody(const char *file_path, char *response_message, unsigned int response_size);
int connection(int sock, Host *host, HttpRequest *request);
int httpRequestWithConnection(char *url);

#endif
