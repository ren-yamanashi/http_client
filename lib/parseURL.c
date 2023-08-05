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
#include "parseURL.h"

/**
 * ホスト名からIPアドレスを取得する
 * @param host ホスト情報が格納された構造体
 * @return 0
 */
int *getIpAddress(Host *host)
{
    struct hostent *host_info;

    // NOTE: ホスト名がIPv4アドレスかチェック
    if (inet_addr(host->hostname) != INADDR_NONE)
    {
        // NOTE: ホスト名が既にIPv4アドレスであれば、ホスト名をIPアドレスとする
        host->ip_address = host->hostname;
        return 0;
    }

    // NOTE: ホスト名からホスト情報を取得
    host_info = gethostbyname(host->hostname);
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
 * @param path パスを格納するバッファへのアドレス
 * @param URLが格納するバッファへのアドレス
 * @return 0
 */
int getHostnameAndPath(Host *host, char *path, char *url)
{
    unsigned int i, j;
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
        copyStringSafely(host->hostname, hostname_path, sizeof(host->hostname));
        strcpy(path, '/');
    }
    else
    {
        // NOTE: `/` が hostname_path に含まれていた場合: `/` の直前を hostname, `/` 以降を path とする
        copyStringSafely(host->hostname, hostname_path, i + 1);
        strcpy(path, &hostname_path[i]);
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