#include "tcp_echo.h"

int 
add_event(int epfd, int fd, int act) {
    struct epoll_event event;

	event.data.fd = fd;
	event.events = act;
	return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

int 
del_event(int epfd, int fd) {
    return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

int 
mod_event(int epfd, int fd, int act) {
    struct epoll_event event;

	event.data.fd = fd;
	event.events = act;
	return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
}
