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

void createRequest(
    HttpRequest *request,
    char *method,
    char *target,
    char *endpoint,
    char *body,
    char *content_type)
{
    copyStringSafely(request->method, method, sizeof(request->method));
    // TODO: targetの先頭に`/`がない場合、`/`をつける
    copyStringSafely(request->target, target, sizeof(request->target));
    copyStringSafely(request->endpoint, endpoint, sizeof(request->endpoint));
    copyStringSafely(request->version, HTTP_VERSION, sizeof(request->version));
    if (body != NULL && strlen(body) - 1 > 0)
    {
        copyStringSafely(request->body, body, sizeof(request->body));
        request->content_length = strlen(request->body) - 1;
    }
    if (content_type != NULL && strlen(content_type) - 1 > 0)
    {
        copyStringSafely(request->content_type, content_type, sizeof(request->content_type));
    }
}

/**
 * HTTP通信でサーバーとデータのやり取りを行う
 * @param sock サーバーと接続積みのソケット
 * @param hostname ホスト名が格納されたバッファへのアドレス
 * @param path パスが格納されたバッファへのアドレス
 * @return 成功時 0 失敗時 -1
 */
int connection(int sock, Host *host, HttpRequest *request)
{
    // NOTE: リクエストを処理
    if (isError(processRequest(sock, host, request)))
    {
        printf("Error: Failed send request");
        return ERROR_FLAG;
    }
    // NOTE: レスポンスメッセージを処理
    if (isError(processResponse(sock)))
    {
        printf("Error: Failed recv response");
        return ERROR_FLAG;
    }
    return 0;
}

int httpRequestWithConnection(HttpRequest *request)
{
    int sock = ERROR_FLAG;
    struct sockaddr_in sock_addr_info;
    Host host;

    // NOTE: urlからホスト名・パス・ポート番号・IPアドレスを取得
    parseURL(&host, request);
    if (host.ip_address == NULL)
    {
        printf("Error: Failed get IP address\n");
        return ERROR_FLAG;
    }

    // NOTE: ソケットを作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (isError(sock))
    {
        printf("Error: Failed create socket\n");
        return ERROR_FLAG;
    }

    // NOTE:サーバーのアドレスファミリー・ボート番号・IPアドレスの情報を設定
    memset(&sock_addr_info, 0, sizeof(struct sockaddr_in));
    sock_addr_info.sin_family = AF_INET;
    sock_addr_info.sin_port = htons((unsigned short)host.port);
    memcpy(&(sock_addr_info.sin_addr.s_addr), host.ip_address, 4);

    // NOTE: サーバーに接続
    if (isError(connect(sock, (struct sockaddr *)&sock_addr_info, sizeof(struct sockaddr_in))))
    {
        close(sock);
        return ERROR_FLAG;
    }

    // NOTE: HTTP通信でデータのやり取り
    connection(sock, &host, request);

    // NOTE: ソケット通信をクローズ
    if (isError(close(sock)))
    {
        printf("Error: failed close socket\n");
        return ERROR_FLAG;
    }
    return 0;
}