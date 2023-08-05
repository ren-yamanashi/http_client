#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "constance.h"
#include "client.h"

int main()
{
    HttpRequest request;
    createRequest(&request, HTTP_METHOD_GET, "/user/23", "http://localhost:8080");
    httpRequestWithConnection(&request);
}