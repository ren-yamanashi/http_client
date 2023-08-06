#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "constance.h"
#include "helper.h"
#include "parseURL.h"

/**
 * ホスト名からIPアドレスを取得する
 * @param host ホスト情報が格納された構造体
 * @return 0
 */
int *getIpAddress(Host *host)
{
    // NOTE: ホスト名がIPv4アドレスかチェック
    if (inet_addr(host->hostname) != INADDR_NONE)
    {
        // NOTE: ホスト名が既にIPv4アドレスであれば、ホスト名をIPアドレスとする
        host->ip_address = host->hostname;
        return 0;
    }

    // NOTE: ホスト名からホスト情報を取得
    struct hostent *host_info = gethostbyname(host->hostname);
    if (host_info == NULL)
    {
        printf("Error: Failed to get host infomation\n");
        host->ip_address = NULL;
        return 0;
    }

    // NOTE: IPv4以外はエラー
    if (host_info->h_length != 4)
    {
        printf("Error: Internet protocol is not IPv4");
        host->ip_address = NULL;
        return 0;
    }

    // NOTE: ホスト情報のアドレス群の一つ目をIPアドレスとする
    host->ip_address = host_info->h_addr_list[0];
    return 0;
}

/**
 * URLからホスト名とパスを取得する
 * @param host ホスト情報が格納された構造体
 * @param request リクエスト情報が格納された構造体
 * @param URL
 * @return 0
 */
int getHostnameAndPath(Host *host, HttpRequest *request)
{
    unsigned int i, j;
    char hostname_path[MAX_HOSTNAME_SIZE + MAX_PATH_SIZE];

    // URLの最初が `http://` の場合は `http://` を取り除く
    if (strncmp(request->origin, "http://", strlen("http://")) == 0)
    {
        sscanf(request->origin, "http://%s", hostname_path);
    }
    else
    {
        strcpy(hostname_path, request->origin);
    }

    // NOTE: 最初の `/` までの文字数をカウント
    for (i = 0; i < strlen(hostname_path); i++)
    {
        if (hostname_path[i] == '/')
        {
            break;
        }
    }
    // NOTE: `/` が hostname_path に含まれていなかった場合: hostname_path 全体を hostname とする
    if (i == strlen(hostname_path))
    {
        copyStringSafely(host->hostname, hostname_path, sizeof(host->hostname));
    }
    else
    {
        // NOTE: `/` が hostname_path に含まれていた場合: `/` の直前を hostname とする
        copyStringSafely(host->hostname, hostname_path, i + 1);
    }
    return 0;
}

/**
 * hostnameからポート番号を取得する
 * @param host ホスト情報が格納された構造体
 */
void getPortNumber(Host *host)
{
    unsigned int j;
    // NOTE: 最初の `:` までの文字数をカウント
    for (j = 0; j < strlen(host->hostname); j++)
    {
        if (host->hostname[j] == ':')
        {
            break;
        }
    }
    // NOTE: `:` が hostname に含まれていた場合: `:` 以降を port とする
    if (j != strlen(host->hostname))
    {
        char portString[MAX_SIZE];
        copyStringSafely(portString, &host->hostname[j + 1], MAX_SIZE);
        host->port = atoi(portString);
        host->hostname[j] = '\0';
    }
    // NOTE: `:` が hostname に含まれていない場合、80 を port とする
    else
    {
        host->port = DEFAULT_PORT;
    }
}

void parseURL(Host *host, HttpRequest *request)
{
    getHostnameAndPath(host, request);
    getPortNumber(host);
    getIpAddress(host);
}