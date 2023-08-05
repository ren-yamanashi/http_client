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
#include "response.h"

/**
 * レスポンスメッセージの受信
 * @param sock 接続済みのソケット
 * @param response_size レスポンスメッセージを格納するバッファのアドレス
 * @param buffer_size response_messageのバッファのサイズ
 * @return 受信したデータサイズ(バイト長)
 */
int recvResponseMessage(int sock, char *response_message, unsigned int buffer_size)
{
    // NOTE: レスポンスを受信
    int recv_size = recv(sock, response_message, buffer_size, RECV_FLAG);
    if (isError(recv_size))
    {
        printf("Error: Failed to receive response message\n");
        return ERROR_FLAG;
    }
    // NOTE: バッファの現在の終端をNULL文字で終了
    response_message[recv_size] = '\0';
    return recv_size;
}