#include "udp_echo.h"

void echo(int sockfd);

int
main(void) {
    int                   sockfd;
    struct sockaddr_in    servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        sys_err("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        sys_err("bind error");

    echo(sockfd);

    exit(0);
}

void
echo(int sockfd) {
    struct sockaddr_in    cliaddr;
    socklen_t             clilen;
    char                  recvbuf[BUFF_SIZE];
    int                   n;

    clilen = sizeof(cliaddr);

    while (1) {
        if ((n = recvfrom(sockfd, recvbuf, BUFF_SIZE, 
                          0, (SA *) &cliaddr, &clilen)) < 0) {
            print_msg("recvfrom error");
            continue;
        }
        write(STDOUT_FILENO, recvbuf, n);
        sendto(sockfd, recvbuf, n, 0, (SA *) &cliaddr, clilen);
    }
}
