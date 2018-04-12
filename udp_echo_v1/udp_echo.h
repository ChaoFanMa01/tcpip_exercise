#ifndef UDP_ECHO_H
#define UDP_ECHO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_PORT    9877
#define SERV_PORT_STR  "9877"
#define BUFF_SIZE      1024
#define SA             struct sockaddr

#define sys_err(str, var...) ({\
    printf(str "\n", ## var);\
	exit(-1);\
})

#define print_msg(str, var...) ({\
    printf(str "\n", ## var);\
})

#endif 
