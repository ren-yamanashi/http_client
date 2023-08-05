#ifndef _CLIENT_H
#define _CLIENT_H

char *getIpAddress(char *hostname);
int getHostnameAndPathAndPort(char *hostname, int *port, char *path, char *url);
int createRequestMessage(char *request_message, char *path, char *hostname);
int sendRequestMessage(int sock, char *request_message, unsigned int message_size);
int recvResponseMessage(int sock, char *request_message, unsigned int buffer_size);
int saveBody(const char *file_path, char *response_message, unsigned int response_size);
int httpClient(int sock, char *hostname, char *path);
int main(int argc, char *argv[]);

#endif
