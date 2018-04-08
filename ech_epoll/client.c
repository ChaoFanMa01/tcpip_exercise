#include "tcp_echo.h"

void str_cli(int fd, int sockfd);
int read_line(int fd, char *buf, int num);

int 
main(int argc, char *argv[]) {
    struct sockaddr_in    servaddr;
	int    sockfd;

	if (argc != 2)
	    err_sys("parameter error");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    err_sys("socket error");
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	str_cli(STDIN_FILENO, sockfd);

	exit(0);
}

void
str_cli(int fd, int sockfd) {
    int   n;
	char  recv_buf[BUFF_SIZE], send_buf[BUFF_SIZE];

	while ((n = read_line(fd, send_buf, BUFF_SIZE)) > 0) {
	    write(sockfd, send_buf, n);
		if ((n = read_line(sockfd, recv_buf, BUFF_SIZE)) > 0)
		    write(STDOUT_FILENO, recv_buf, n);
	}
}

int
read_line(int fd, char *buf, int num) {
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
