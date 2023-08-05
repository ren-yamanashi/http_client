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

/**
 * ホスト名からIPアドレスを取得する
 * @param hostname ホスト名
 * @return IPアドレスが格納されたバッファへのアドレス(失敗時はNULL)
 */
char *getIpAddress(char *hostname)
{
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
int getHostnameAndPathAndPort(char *hostname, int *port, char *path, char *url)
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
        strcpy(hostname, hostname_path);
        strcpy(path, '/');
    }
    else
    {
        // NOTE: `/` が hostname_path に含まれていた場合: `/` の直前を hostname, `/` 以降を path とする
        strncpy(hostname, hostname_path, i);
        hostname[i] = '\0';
        strcpy(path, &hostname_path[i]);
    }

    // NOTE: 最初の `:` までの文字数をカウント
    for (j = 0; j < strlen(hostname); j++)
    {
        if (hostname[j] == ':')
        {
            break;
        }
    }
    // NOTE: `:` が hostname に含まれていた場合: `:` 以降を port とする
    if (j != strlen(hostname))
    {
        char portString[MAX_SIZE];
        strcpy(portString, &hostname[j + 1]);
        *port = atoi(portString);
        hostname[j] = '\0';
    }
    // NOTE: `:` が hostname に含まれていない場合、80 を port とする
    else
    {
        *port = DEFAULT_PORT;
    }
    return 0;
}