#include "tcp_echo.h"

static void echo(int sockfd);
void sig_chld(int signo);

int
main(void) {
    int                 listenfd, connfd;
	struct sockaddr_in  servaddr, cliaddr;
	socklen_t           clilen;
	pid_t               child;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    err_sys("socket error");
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	    err_sys("bind error");
	
	if (listen(listenfd, LISTENQ) < 0)
	    err_sys("listen error");

	signal(SIGCHLD, sig_chld);

	while (1) {
	    clilen = sizeof(cliaddr);
	    if ((connfd = accept(listenfd, (SA *) &cliaddr,
		                     &clilen)) < 0)
		    if (errno == EINTR)
			    continue;
			else
			    err_sys("accept error");
		if ((child = fork()) < 0) {
		    err_sys("fork error");
		} else {
		    // child process.
		    if (child == 0) {
			    close(listenfd);
				echo(connfd);
				exit(0);
			}
			close(connfd);
		}
	}
	close(listenfd);
	exit(0);
}

static void
echo(int sockfd) {
    char     recv[BUFF_SIZE];
	int      n;

again:
	while ((n = read(sockfd, recv, BUFF_SIZE)) > 0) {
	    write(STDOUT_FILENO, recv, n);
		write(sockfd, recv, n);
	}

	if (n < 0 && errno == EINTR)
	    goto again;
	else if (n < 0)
	    err_sys("echo error");
}

void
sig_chld(int signo) {
    pid_t    child;
	int      stat;
    if (signo != SIGCHLD)
	    return;
	
	while ((child = waitpid(-1, &stat, WNOHANG)) > 0)
	    print_msg("pid %d terminated", child);
}
