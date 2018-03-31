#include "tcp_echo.h"

int
main(int argc, char *argv[]) {
    int                   listenfd, connfd;
	struct sockaddr_in    servaddr, cliaddr;
	socklen_t             clilen;
	pid_t                 pid;

    // Create a socket.
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    err_sys("socket error");
	
    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	    err_sys("bind error");
	
    listen(listenfd, LISTENQ);

	while (1) {
	    clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (SA *) &cliaddr, 
		                     &clilen)) < 0)
		    err_sys("accept error");
		if ((pid = fork()) < 0) {
		    err_sys("fork error");
		} else if (pid == 0) {
		    close(listenfd);
			strecho(connfd);
			close(connfd);
			exit(0);
		}
        close(connfd);
	}
	close(listenfd);
	exit(0);
}

void
strecho(int sockfd) {
    char buff[BUFF_SIZE];
	int n;

again:
	while ((n = read(sockfd, buff, BUFF_SIZE)) > 0)
	    write(sockfd, buff, n);
	
	if (n < 0 && errno == EINTR)
	    goto again;
	else if (n < 0)
	    err_sys("echo error");
}
