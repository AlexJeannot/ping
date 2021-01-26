#include "../inc/ping.h"

void get_addr()
{
    struct addrinfo hints;
    int ret;

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    if ((ret = getaddrinfo(env.args.hostname, NULL, &hints, &(env.addr))) != 0)
        getaddr_error(env.args.hostname, ret);
}

void set_socket()
{
    struct timeval send_to;
    struct timeval recv_to;
    int on;

    send_to.tv_sec = 1;
    send_to.tv_usec = 0;
    recv_to.tv_sec = 1;
    recv_to.tv_usec = 0;

    on = 1;
    if ((env.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        error_exit("socket creation");

    on = 1;
    if ((setsockopt(env.sockfd, IPPROTO_IP, IP_RECVTTL, (char*)&(on), sizeof(on))) < 0)
        error_exit("socket TTL reception");

    if ((setsockopt(env.sockfd, 0, IP_TTL, (char*)&(env.ttl), sizeof(env.ttl))) < 0)
        error_exit("socket TTL setup");

    if ((setsockopt(env.sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&(send_to), sizeof(send_to))) < 0)
        error_exit("socket send TO");

    if ((setsockopt(env.sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&(recv_to), sizeof(recv_to))) < 0)
        error_exit("socket reception TO");
}

