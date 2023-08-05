#ifndef _REQUEST_H
#define _REQUEST_H

int createRequestMessage(char *request_message, char *path, char *hostname);
int sendRequestMessage(int sock, char *request_message, unsigned int message_size);

#endif