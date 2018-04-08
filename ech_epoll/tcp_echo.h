#ifndef TCP_ECHO_H
#define TCP_ECHO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

#define SERV_PORT    9877
#define SERV_PORT_STR  "9877"
#define LISTENQ        1024
#define BUFF_SIZE      1024
#define SA             struct sockaddr

#define err_sys(str, var...) ({\
    printf(str "\n", ## var);\
	exit(-1);\
})

#define print_msg(str, var...) ({\
    printf(str "\n", ## var);\
})

int add_event(int epfd, int fd, int act);
int del_event(int epfd, int fd);
int mod_event(int epfd, int fd, int act);
#endif
