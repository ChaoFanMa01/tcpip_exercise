#include "tcp_echo.h"

static int init_socket(void);
static void handle_events(int epfd, struct epoll_event *events, int num, int listenfd, char buf[], int *cnt);
static void do_accept(int epfd, int listenfd);
static void do_read(int epfd, int sockfd, char buf[], int *n);
static void do_write(int epfd, int sockfd, char buf[], int *n);

const int FDSIZE     = 1024;
const int EVENTS_NUM = 20;

int
main(void) {
    int                listenfd, epfd;
    struct epoll_event events[EVENTS_NUM];
	int                n;
	char               recv[BUFF_SIZE];
	int                cnt;

    listenfd = init_socket();
	if (listen(listenfd, LISTENQ) < 0)
	    err_sys("listen error");
	
	if ((epfd = epoll_create(FDSIZE)) < 0)
	    err_sys("epoll create error");
    add_event(epfd, listenfd, EPOLLIN);

	while (1) {
	    n = epoll_wait(epfd, events, EVENTS_NUM, -1);
		print_msg("wait over.");
		handle_events(epfd, events, n, listenfd, recv, &cnt);
		print_msg("handle over.");
	}

    close(listenfd);
	close(epfd);
}

static void
handle_events(int epfd, struct epoll_event *events, 
              int num, int listenfd, char buf[], int *cnt) {
    for (int i = 0; i < num; i++)
	    if (events[i].data.fd == listenfd &&
		    events[i].events & EPOLLIN) {
		    do_accept(epfd, listenfd);
		} else if (events[i].events & EPOLLIN) {
		    do_read(epfd, events[i].data.fd, buf, cnt);
		} else if (events[i].events & EPOLLOUT) {
		    do_write(epfd, events[i].data.fd, buf, cnt);
		}
}

static void
do_accept(int epfd, int listenfd) {
    int                  connfd;
	struct sockaddr_in   cliaddr;
	socklen_t            clilen;
	char                 ip[INET_ADDRSTRLEN];

	print_msg("do accept");
 
    clilen = sizeof(cliaddr);
	if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0)
	    err_sys("accept error");
	
	inet_ntop(AF_INET, &cliaddr.sin_addr, ip, sizeof(ip));
	print_msg("client %s has connected", ip);
	add_event(epfd, connfd, EPOLLIN);
	print_msg("accept over");
}

static void
do_read(int epfd, int sockfd, char recv[], int *n) {
    print_msg("do read");
again:
    // read error.
    if ((*n = read(sockfd, recv, BUFF_SIZE)) < 0) {
	    if (errno == EINTR) {
		    goto again;
		} else {
		    close(sockfd);
			del_event(epfd, sockfd);
		    err_sys("read error");
		}
	// read EOF.
	} else if (*n == 0) {
	    del_event(epfd, sockfd);
	    close(sockfd);
	// read data.
	} else {
	    // prepare for echoing by changing its direction.
	    mod_event(epfd, sockfd, EPOLLOUT);
	}
}

static void
do_write(int epfd, int sockfd, char buf[], int *n) {
    int    i = 0, cnt;
	print_msg("do write");

again:
    if ((cnt = write(sockfd, buf, *n)) < 0) {
	    // interrupted by signal.
	    if (errno == EINTR) {
		    goto again;
		// write error.
		} else {
		    close(sockfd);
			del_event(epfd, sockfd);
			err_sys("write error");
		}
	}
	mod_event(epfd, sockfd, EPOLLIN);
	bzero(buf, BUFF_SIZE);
}

static int
init_socket(void) {
    int                 listenfd;
    struct sockaddr_in  servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	    err_sys("bind error");
	return listenfd;
}
