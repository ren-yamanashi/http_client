#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "client.h"

/**
 * ホスト名からIPアドレスを取得する
 * @param hostname ホスト名
 * @return IPアドレスが格納されたバッファへのアドレス(失敗時はNULL)
 */
char *getIpAddress(char *hostname)
{
    printf("hostname: %s\n", hostname);
    struct hostent *host;

    // NOTE: ホスト名がIPv4アドレスかチェック
    if (inet_addr(hostname) != INADDR_NONE)
    {
        // NOTE: ホスト名が既にIPv4アドレスであれば、そのまま返す
        return hostname;
    }

    // NOTE: ホスト名からホスト情報を取得
    host = gethostbyname(hostname);
    if (host == NULL)
    {
        printf("Error: Failed to get host infomation\n");
        return NULL;
    }

    // NOTE: IPv4以外はエラー
    if (host->h_length != 4)
    {
        printf("Error: Internet protocol is not IPv4");
        return NULL;
    }

    // NOTE: ホスト情報のアドレス群の一つ目をIPアドレスとする
    return host->h_addr_list[0];
}

/**
 * URLからホスト名とパスを取得する
 * @param hostname ホスト名と格納するバッファへのアドレス
 * @param path パスを格納するバッファへのアドレス
 * @param URLが格納するバッファへのアドレス
 * @return 0
 */
int getHostnameAndPath(char *hostname, char *path, char *url)
{
    unsigned int i;
    char hostname_path[MAX_HOSTNAME_SIZE + MAX_PATH_SIZE];

    // URLの最初が `http://` の場合は `http://` を取り除く
    if (strncmp(url, "http://", strlen("http://")) == 0)
    {
        sscanf(url, "http://%s", hostname_path);
    }
    else
    {
        strcpy(hostname_path, url);
    }

    // NOTE: 最初の `/` までの文字数をカウント
    for (i = 0; i < strlen(hostname_path); i++)
    {
        if (hostname_path[i] == '/')
        {
            break;
        }
    }

    // NOTE: `/` が hostname_path に含まれていなかった場合: hostname_path 全体を hostname, path を `/` とする
    if (i == strlen(hostname_path))
    {
        strcpy(hostname, hostname_path);
        strcpy(path, "/");
    }
    else
    {
        // NOTE: `/` が hostname_path に含まれていた場合: `/` の直前を hostname, `/` 以降を path とする
        strncpy(hostname, hostname_path, i);
        hostname[i] = '\0';
        strcpy(path, &hostname_path[i]);
    }
    return 0;
}

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

/**
 * レスポンスメッセージの受信
 * @param sock 接続済みのソケット
 * @param response_size レスポンスメッセージを格納するバッファのアドレス
 * @param buffer_size response_messageのバッファのサイズ
 * @return 受信したデータサイズ(バイト長)
 */
int recvResponseMessage(int sock, char *request_message, unsigned int buffer_size)
{
    int total_recv_seize = 0;
    int i;
    while (1)
    {
        int recv_size = recv(sock, &request_message[total_recv_seize], buffer_size, 0);
        if (recv_size == -1)
        {
            printf("Error: Failed to receive response");
            return -1;
        }
        if (recv_size == 0)
        {
            printf("connection ended\n");
            break;
        }

        for (i = 0; i < recv_size; i++)
        {
            putc(request_message[total_recv_seize + i], stdout);
        }
        total_recv_seize += recv_size;
    }
    return total_recv_seize;
}

/**
 * ボディをファイル保存する
 * @param file_path 保存先のファイルパス
 * @param response_message レスポンスメッセージを格納するバッファへのアドレス
 * @param response_size レスポンスメッセージのサイズ
 * @return 成功時 0 失敗時 -1
 */
int saveBody(const char *file_path, char *response_message, unsigned int response_size)
{
    FILE *fo;
    char *tmp;
    unsigned int skip_size = 0;

    // NOTE: レスポンスメッセージを空行まで読み飛ばし
    tmp = strtok(response_message, "\n");
    while (tmp != NULL && (strcmp(tmp, "\r") != 0 && strcmp(tmp, "") != 0))
    {
        skip_size += strlen(tmp) + strlen("\n");
        tmp = strtok(NULL, "\n");
        printf("%s\n", tmp);
    }

    if (tmp == NULL)
    {
        printf("Error: body is not found\n");
        return -1;
    }

    // NOTE: 空行の次の行からをボディと見做してファイル保存
    skip_size += strlen(tmp) + strlen("\n");
    fo = fopen(file_path, "wb");
    if (fo == NULL)
    {
        printf("Error: failed open file (%s)\n", file_path);
        return -1;
    }

    fwrite(&response_message[skip_size], 1, response_size - skip_size, fo);
    fclose(fo);
    return 0;
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
    if (request_message == -1)
    {
        printf("Error: failed create request message\n");
        return -1;
    }

    // NOTE: リクエストメッセージを送信
    if (sendRequestMessage(sock, request_message, request_size) == -1)
    {
        printf("Error: failed send request message\n");
        return -1;
    }

    // NOTE: レスポンスメッセージを受信
    response_size = recvResponseMessage(sock, response_message, MAX_RESPONSE_SIZE);
    if (response_size == -1)
    {
        printf("Error: failed recv response message\n");
        return -1;
    }

    // NOTE: レスポンスメッセージに応じた処理
    if (saveBody("test.html", response_message, response_size) == -1)
    {
        printf("Error failed save body\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int sock = -1;
    struct sockaddr_in addr;
    char url[MAX_URL];
    char *ip_address;
    char hostname[MAX_HOSTNAME_SIZE];
    char path[MAX_PATH_SIZE];

    if (argc == 2)
    {
        strcpy(url, argv[1]);
    }
    else
    {
        printf("Error: URL does not exist. Please set request URL\n");
        return -1;
    }

    // NOTE: ホスト名とパスを取得
    getHostnameAndPath(hostname, path, url);

    // NOTE: IPアドレスを取得
    ip_address = getIpAddress(hostname);
    if (ip_address == NULL)
    {
        printf("Error: failed get IP address\n");
        return -1;
    }

    // NOTE: ソケットを作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("socket error\n");
        return -1;
    }

    // NOTE: 構造体を全て０にセット
    memset(&addr, 0, sizeof(struct sockaddr_in));

    // NOTE: 接続先の情報を設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons(CONNECT_PORT);

    // NOTE: ip_addressは数値の配列なのでそのままコピー (inet_addrは不要)
    memcpy(&(addr.sin_addr.s_addr), ip_address, 4);

    // NOTE: サーバーに接続
    printf("Connect to %u.%u.%u.%u\n",
           (unsigned char)ip_address[0],
           (unsigned char)ip_address[1],
           (unsigned char)ip_address[2],
           (unsigned char)ip_address[3]);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("Error: failed connect server\n");
        close(sock);
        return -1;
    }

    // NOTE: HTTP通信でデータのやり取り
    httpClient(sock, hostname, path);

    // NOTE: ソケット通信をクローズ
    if (close(sock) == -1)
    {
        printf("Error: failed close socket\n");
        return -1;
    }
    return 0;
}