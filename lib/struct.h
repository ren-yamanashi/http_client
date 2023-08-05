#ifndef _STRUCT_H
#define _STRUCT_H

#include "constance.h"
typedef struct 
{
    int port;
    char *ip_address;
    char hostname[MAX_HOSTNAME_SIZE];
} Host;

#endif