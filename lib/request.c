#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "constance.h"
#include "request.h"

/**
 * リクエストメッセージを作成
 * @param request_message リクエストメッセージを格納するバッファへのアドレス
 * @param target リクエストターゲット(リクエストするファイル)
 * @param host リクエスト先のホスト名
 * @return メッセージのサイズ
 */
int createRequestMessage(char *request_message, char *path, char *hostname)
{
    char request[MAX_SIZE];
    char header[MAX_SIZE];

    sprintf(request, "GET %s HTTP/1.1", path);
    sprintf(header, "Host: %s\r\nConnection: close", hostname);
    sprintf(request_message, "%s\r\n%s\r\n\r\n", request, header);

    return strlen(request_message);
}

/**
 * リクエストメッセージを送信
 * @param sock 接続済みのソケット
 * @param request_message 送信するリクエストメッセージへのアドレス
 * @param message_size 送信するメッセージのサイズ
 * @return 送信したデータサイズ(バイト長)
 */
int sendRequestMessage(int sock, char *request_message, unsigned int message_size)
{
    int send_size = send(sock, request_message, message_size, 0);
    return send_size;
}