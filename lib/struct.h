#ifndef _STRUCT_H
#define _STRUCT_H

#include "constance.h"
typedef struct
{
    int port;
    char *ip_address;
    char hostname[MAX_HOSTNAME_SIZE];
} Host;

typedef struct
{
    char method[32];
    char target[1024];
    char version[32];
    char content_type[128];
    char body[1024];
    char origin[1024];
    unsigned int content_length;
} HttpRequest;

#endif