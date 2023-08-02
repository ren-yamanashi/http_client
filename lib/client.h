#ifndef _CLIENT_H
#define _CLIENT_H

#define CONNECT_PORT 80
#define MAX_RESPONSE_SIZE (1024 * 1024 * 5)
#define MAX_REQUEST_SIZE (1024)
#define MAX_HOSTNAME_SIZE 1024
#define MAX_PATH_SIZE 1024
#define MAX_SIZE 128
#define MAX_URL 256

char *getIpAddress(char *hostname);
int getHostnameAndPath(char *hostname, char *path, char *url);
int createRequestMessage(char *request_message, char *path, char *hostname);
int sendRequestMessage(int sock, char *request_message, unsigned int message_size);
int recvResponseMessage(int sock, char *request_message, unsigned int buffer_size);
int saveBody(const char *file_path, char *response_message, unsigned int response_size);
int httpClient(int sock, char *hostname, char *path);
int main(int argc, char *argv[]);

#endif
