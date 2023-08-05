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
#include "helper.h"
#include "struct.h"
#include "request.h"

/**
 * リクエストメッセージを作成
 * @param request_message リクエストメッセージを格納するバッファへのアドレス
 * @param host ホスト情報が格納された構造体
 * @param request リクエスト情報が格納された構造体
 * @return メッセージのサイズ
 */
int createRequestMessage(char *request_message, Host *host, HttpRequest *request)
{
    char _request[MAX_SIZE];
    char header[MAX_SIZE];

    sprintf(_request, "GET %s HTTP/1.1", request->target);
    sprintf(header, "Host: %s\r\nConnection: close", host->hostname);
    sprintf(request_message, "%s\r\n%s\r\n\r\n", _request, header);

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

int processRequest(int sock, Host *host, HttpRequest *request)
{
    int request_size;
    char request_message[MAX_REQUEST_SIZE];
    // TODO: リクエストメッセージの解析

    // NOTE: リクエストメッセージを作成
    request_size = createRequestMessage(request_message, host, request);
    if (isError(request_size))
    {
        return ERROR_FLAG;
    }

    // NOTE: リクエストメッセージの表示
    printf("\n======Request message======\n\n");
    showMessage(request_message, request_size);

    // NOTE: リクエストメッセージを送信
    if (isError(sendRequestMessage(sock, request_message, request_size)))
    {
        return ERROR_FLAG;
    }
    return SUCCESS_FLAG;
}