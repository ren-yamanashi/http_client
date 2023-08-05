#ifndef _CLIENT_H
#define _CLIENT_H

void showMessage(char *message, unsigned int size);
int recvResponseMessage(int sock, char *response_message, unsigned int buffer_size);
int saveBody(const char *file_path, char *response_message, unsigned int response_size);
int httpClient(int sock, char *hostname, char *path);
int main(int argc, char *argv[]);

#endif
