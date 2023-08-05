#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "constance.h"
#include "client.h"

int main(int argc, char *argv[])
{
    char url[MAX_URL];
    if (argc == 2)
    {
        strcpy(url, argv[1]);
    }
    else
    {
        printf("Error: URL does not exist. Please set request URL\n");
        return ERROR_FLAG;
    }

    httpRequestWithConnection(url);
}