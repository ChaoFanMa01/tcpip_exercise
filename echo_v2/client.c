#include "tcp_echo.h"

static void str_cli(int, int);
static int readline(int fd, char *buf, int num);

int
main(int argc, char *argv[]) {
    int                 sockfd;
	struct sockaddr_in  servaddr;

    if (argc != 2)
	    err_sys("parameter error");
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 1)
	    err_sys("inet_pton error or invalid address");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    err_sys("socket error");
	
	// when writing to a socket that has received
	// an RST, a SIGPIPE signal is generated. Here,
	// we ignore this signal.
	signal(SIGPIPE, SIG_IGN);
	
	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli(STDIN_FILENO, sockfd);

	exit(0);
}

static void
str_cli(int in, int sockfd) {
    char     send[BUFF_SIZE], recv[BUFF_SIZE];
	int      n;

	while ((n = readline(in, send, BUFF_SIZE)) > 0) {
	    write(sockfd, send, n);
		if ((n = readline(sockfd, recv, BUFF_SIZE)) <= 0)
		    break;
		write(STDOUT_FILENO, recv, n);
	}
}

static int 
readline(int fd, char *buf, int num) {
    char c;
	int n = 0, stat;

	while (1) {
	    // read one more character correctly.
	    if ((stat = read(fd, &c, 1)) == 1) {
		    *(buf + n++) = c;
			if (c == '\n')
			    return n;
		// read EOF.
		} else if (stat == 0) {
		    break;
		// read error.
		} else if (stat < 0) {
		    // read operation is interrupted by system calls.
		    if (errno == EINTR)
			    continue;
			else
			    err_sys("read error.");
		}
	}
	*(buf + n) = '\n';
	return n;
}
