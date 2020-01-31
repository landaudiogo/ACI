#ifndef general_h
#define general_h

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <arpa/inet.h>

#define server_ip "127.0.0.1"
#define SERVER_PORT 49950

#define register_to_octet(h_r, l_r, val) {\
    h_r = (0xFF00 & val) >> 8;\
    l_r = 0x00FF & val;\
    }
#define octet_to_register(h_r, l_r, val) {\
    val = h_r << 8;\
    val |= l_r;\
    }

#endif
