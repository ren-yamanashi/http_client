#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constance.h"
#include "helper.h"
#include "response.h"

/**
 * レスポンスメッセージの受信
 * @param sock 接続済みのソケット
 * @param response_message レスポンスメッセージを格納するバッファのアドレス
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

int processResponse(int sock)
{
    int response_size;
    char response_message[MAX_RESPONSE_SIZE];

    // NOTE: レスポンスメッセージを受信
    response_size = recvResponseMessage(sock, response_message, MAX_RESPONSE_SIZE);
    if (isError(response_size))
    {
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

    return SUCCESS_FLAG;
}