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
    if ((setsockopt(env.sockfd, 0, IP_TTL, &(env.ttl), sizeof(env.ttl))) < 0)
        error_exit("socket TTL setup");

    // TODO ON LINUX
    if ((setsockopt(env.sockfd, 0, IP_RECVERR, &(on), sizeof(on))) < 0)
        error_exit("socket TTL setup");

    on = 1;
    if ((setsockopt(env.sockfd, 0, IP_RECVTTL, &(on), sizeof(on))) < 0)
        error_exit("socket TTL setup");

}

