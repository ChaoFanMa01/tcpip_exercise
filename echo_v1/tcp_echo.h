#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERV_PORT         9877
#define SERV_PORT_STR    "9877"

#define LISTENQ           1024

#define BUFF_SIZE         1024

#define SA    struct sockaddr

#define err_sys(str, var...) ({\
    printf(str "\n", ## var);\
	exit(-1);\
})

#define print_msg(str, var...) ({\
    printf(str "\n", ## var);\
})

void strecho(int sockfd);

