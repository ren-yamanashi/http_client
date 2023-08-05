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
#include "parseURL.h"
#include "request.h"
#include "helper.h"
#include "struct.h"
#include "response.h"
#include "client.h"

/**
 * 受信した文字列を表示
 * @param message メッセージを格納するバッファへのアドレス
 * @param size メッセージのバイト数
 */
void showMessage(char *message, unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++)
    {
        putchar(message[i]);
    }
    printf("\r\n");
}

/**
 * HTTP通信でサーバーとデータのやり取りを行う
 * @param sock サーバーと接続積みのソケット
 * @param hostname ホスト名が格納されたバッファへのアドレス
 * @param path パスが格納されたバッファへのアドレス
 * @return 成功時 0 失敗時 -1
 */
int httpClient(int sock, char *hostname, char *path)
{
    int request_size, response_size;
    char request_message[MAX_REQUEST_SIZE];
    char response_message[MAX_RESPONSE_SIZE];

    // NOTE: リクエストメッセージを作成
    request_size = createRequestMessage(request_message, path, hostname);
    if (isError(request_size))
    {
        printf("Error: failed create request message\n");
        return ERROR_FLAG;
    }

    // NOTE: リクエストメッセージの表示
    printf("\n======Request message======\n\n");
    showMessage(request_message, request_size);

    // NOTE: リクエストメッセージを送信
    if (isError(sendRequestMessage(sock, request_message, request_size)))
    {
        printf("Error: failed send request message\n");
        return ERROR_FLAG;
    }

    // NOTE: レスポンスメッセージを受信
    response_size = recvResponseMessage(sock, response_message, MAX_RESPONSE_SIZE);
    if (isError(response_size))
    {
        printf("Error: failed recv response message\n");
        return ERROR_FLAG;
    }
    if (response_size == 0)
    {
        // NOTE: 受信サイズが0の場合は相手が接続を閉じていると判断
        printf("Info: Connection ended\n");
        return ERROR_FLAG;
    }

    // NOTE: レスポンスメッセージの表示
    printf("\n======Response message======\n\n");
    showMessage(response_message, response_size);

    // TODO: レスポンスメッセージに応じた処理

    return 0;
}

int main(int argc, char *argv[])
{
    int sock = ERROR_FLAG;
    char url[MAX_URL];
    char path[MAX_PATH_SIZE];
    struct sockaddr_in sock_addr_info;
    Host host;

    if (argc == 2)
    {
        strcpy(url, argv[1]);
    }
    else
    {
        printf("Error: URL does not exist. Please set request URL\n");
        return ERROR_FLAG;
    }

    // NOTE: urlからホスト名・パス・ポート番号を取得
    getHostnameAndPath(&host, path, url);
    getPortNumber(&host);

    // NOTE: ホスト名からIPアドレスを取得
    getIpAddress(&host);
    if (host.ip_address == NULL)
    {
        printf("Error: failed get IP address\n");
        return ERROR_FLAG;
    }

    // NOTE: ソケットを作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (isError(sock))
    {
        printf("socket error\n");
        return ERROR_FLAG;
    }

    // NOTE: 構造体を全て0にセット
    memset(&sock_addr_info, 0, sizeof(struct sockaddr_in));

    // NOTE:サーバーのアドレスファミリー・ボート番号・IPアドレスの情報を設定
    sock_addr_info.sin_family = AF_INET;
    sock_addr_info.sin_port = htons((unsigned short)host.port);
    memcpy(&(sock_addr_info.sin_addr.s_addr), host.ip_address, 4);

    // NOTE: サーバーに接続
    if (isError(connect(sock, (struct sockaddr *)&sock_addr_info, sizeof(struct sockaddr_in))))
    {
        printf("Error: failed connect server\n");
        close(sock);
        return ERROR_FLAG;
    }

    // NOTE: HTTP通信でデータのやり取り
    httpClient(sock, host.hostname, path);

    // NOTE: ソケット通信をクローズ
    if (isError(close(sock)))
    {
        printf("Error: failed close socket\n");
        return ERROR_FLAG;
    }
    return 0;
}