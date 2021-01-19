#include "ft_ping.h"

void get_addr(t_env *env)
{
    struct addrinfo hints;
    int ret;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    if ((ret = getaddrinfo("google.com", NULL, &hints, &(env->addr))) != 0)
        display_addr_error(ret);
}

void setup_socket(t_env *env)
{
    if ((env->sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        error_exit("socket creation");
    if ((setsockopt(env->sockfd, 0, IP_TTL, &(env->ttl), sizeof(env->ttl))) < 0)
        error_exit("socket TTL setup");
}