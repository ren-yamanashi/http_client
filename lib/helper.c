#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include "constance.h"

void copyStringSafely(char *destination, char *source, size_t destination_size)
{
    strncpy(destination, source, destination_size - 1);
    destination[destination_size - 1] = '\0';
}

int isError(int target)
{
    if (target == ERROR_FLAG)
    {
        return 1;
    }
    return 0;
}

int isMatchStr(const char *str1, const char *str2)
{
    if (strcmp(str1, str2) == 0)
    {
        return 1;
    }
    return 0;
}

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