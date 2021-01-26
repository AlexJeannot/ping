#include "../inc/ft_ping.h"

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
    int on;

    on = 1;
    if ((env.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        error_exit("socket creation");

    if (!(env.args.ttl))
        env.ttl = 64;
    if ((setsockopt(env.sockfd, 0, IP_TTL, (char*)&(env.ttl), sizeof(env.ttl))) < 0)
        error_exit("socket TTL setup");

    // TODO ON LINUX
    // if ((setsockopt(env.sockfd, 0, IP_RECVERR, (char*)&(on), sizeof(on))) < 0)
    //     error_exit("socket TTL setup");

    on = 1;
    if ((setsockopt(env.sockfd, IPPROTO_IP, IP_RECVTTL, (char*)&(on), sizeof(on))) < 0)
        error_exit("socket TTL setup");


    struct timeval send_to;
    send_to.tv_sec = 1;
    send_to.tv_usec = 0;

    struct timeval recv_to;
    recv_to.tv_sec = 1;
    recv_to.tv_usec = 0;

    if ((setsockopt(env.sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&(send_to), sizeof(send_to))) < 0)
        error_exit("socket TTL setup");

    if ((setsockopt(env.sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&(recv_to), sizeof(recv_to))) < 0)
        error_exit("socket TTL setup");

    // on = 1;
    // if ((setsockopt(env.sockfd, 0, IP_HDRINCL, &(on), sizeof(on))) < 0)
    //     error_exit("socket TTL setup");
}

