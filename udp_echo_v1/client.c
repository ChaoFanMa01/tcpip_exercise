#include "udp_echo.h"

static int readline(int fd, char *buf, int n);

int
main(int argc, char *argv[]) {
    struct sockaddr_in       servaddr, recvaddr;
    int                      sockfd;
    socklen_t                recvlen;
    char                     recvbuf[BUFF_SIZE], sendbuf[BUFF_SIZE];
    int                      n;

    if (argc != 2)
        sys_err("parameter error");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        sys_err("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
        sys_err("invalid ip address");
    
    recvlen = sizeof(recvaddr);

    while ((n = readline(STDIN_FILENO, sendbuf, BUFF_SIZE)) > 0) {
        sendto(sockfd, sendbuf, n, 0, (SA *) &servaddr, recvlen);
        n = recvfrom(sockfd, recvbuf, BUFF_SIZE, 0, 
                     (SA *) &recvaddr, &recvlen);
        write(STDOUT_FILENO, recvbuf, n);
    }
}

static int
readline(int fd, char *buf, int num) {
    int n = 0, status;

    while (1) {
	    if ((status = read(fd, buf + n, 1)) != 1 ||
		    buf[n++] == '\n' || n >= num)
		    break;
	}
   
    if (status < 0)
	    return -1;
	else
	    return n;
}
