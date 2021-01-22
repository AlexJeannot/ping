#include "ft_ping.h"

void get_addr(t_env *env)
{
    struct addrinfo hints;
    int ret;
    char test[1024];

    bzero(&test, sizeof(test));

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_CANONNAME;
    // env->addr->ai_canonname = &test[0];

    if ((ret = getaddrinfo(env->args.hostname, NULL, &hints, &(env->addr))) != 0)
        error_exit("getaddrinfo");
    printf("env->addr->ai_canonname = %s\n", env->addr->ai_canonname);
}

void setup_socket(t_env *env)
{
    int yes;

    yes = 1;
    if ((env->sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        error_exit("socket creation");
    if ((setsockopt(env->sockfd, 0, IP_TTL, &(env->ttl), sizeof(env->ttl))) < 0)
        error_exit("socket TTL setup");

    // TODO ON LINUX
    // if ((setsockopt(env->sockfd, 0, IP_RECVERR, &(yes), sizeof(yes))) < 0)
    //     error_exit("socket TTL setup");

    int oui = 60;
    if ((setsockopt(env->sockfd, 0, IP_RECVTTL, &(oui), sizeof(oui))) < 0)
        error_exit("socket TTL setup");

}

