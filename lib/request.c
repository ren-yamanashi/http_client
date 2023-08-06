#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    char request_line[MAX_SIZE];
    char header[MAX_SIZE];

    sprintf(request_line, "%s %s %s", request->method, request->target, request->version);
    if (request->content_length > 0)
    {
        sprintf(header, "Host: %s\r\nConnection: close\r\nContent-Type: %s\r\nContent-Length: %d",
                host->hostname,
                request->content_type,
                request->content_length);
        sprintf(request_message, "%s\r\n%s\r\n\r\n%s\r\n", request_line, header, request->body);
    }
    else
    {
        sprintf(header, "Host: %s\r\nConnection: close", host->hostname);
        sprintf(request_message, "%s\r\n%s\r\n\r\n", request_line, header);
    }

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