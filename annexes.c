#include "ft_ping.h"

void display_addr_info(struct addrinfo *addr)
{
    int count;

    count = 0;
    printf("\n=================== ADDR INFOS ===================\n");
    while (addr)
    {
        printf("\n-------------- %d --------------\n", count++);
        printf("ai_flags = %d\n", addr->ai_flags);
        printf("ai_family = %d\n", addr->ai_family);
        printf("ai_socktype = %d\n", addr->ai_socktype);
        printf("ai_protocol = %d\n", addr->ai_protocol);
        printf("ai_addrlen = %u\n", addr->ai_addrlen);
        printf("ai_canonname = %s\n", addr->ai_canonname);
        printf("res->ai_addr->sa_family = %hhu\n", addr->ai_addr->sa_family);
        printf("--------------------------------\n");
        addr = addr->ai_next;
    }
    printf("\n==================================================\n\n");
}

void display_icmp_header_info(t_icmp_header header, char *name)
{
    size_t size;

    size = sizeof(header) - 8;
    printf("\n=================== DISPLAY ICMP %s ===================\n", name);
    printf("type = %d\n", header.type);
    printf("code = %d\n", header.code);
    printf("checksum = %d\n", header.checksum);
    printf("id = %d\n", header.id);
    printf("sequence = %d\n", header.seq);
    printf("--------- DATA ---------\n");
    for (size_t pos = 0; pos < size; pos++)
        write(1, &(header.data[pos]), 1);
    printf("\n------------------------\n");
    for (size_t count = 0; count < (53 + strlen(name)); count++)
        write(1, "=", 1);
    write(1, "\n\n", 2);
}

void display_ip_header_info(t_ip_header header, char *name)
{
    printf("\n=================== DISPLAY IP %s ===================\n", name);
    printf("version = %u\n", header.version);
    printf("header_size = %u\n", header.header_size);
    printf("dscp = %u\n", header.dscp);
    printf("ecn = %u\n", header.ecn);
    printf("packet_size = %u\n", header.packet_size);
    printf("id = %u\n", header.id);
    printf("flags = %u\n", header.flags);
    printf("offset = %u\n", header.offset);
    printf("ttl = %u\n", header.ttl);
    printf("protocol = %u\n", header.protocol);
    printf("checksum = %u\n", header.checksum);
    printf("s_addr = %u\n", header.s_addr);
    printf("d_addr = %u\n", header.d_addr);
    for (size_t count = 0; count < (53 + strlen(name)); count++)
        write(1, "=", 1);
    write(1, "\n\n", 2);
}

long get_ts_s(void)
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    return (tv.tv_sec);
}

long double get_ts_ms(void)
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    return (((long double)tv.tv_sec * 1000) + ((long double)tv.tv_usec / 1000));
}

void set_time()
{
    alarm(1);
    env.timeout = 1;
    env.ts_before = get_ts_ms();
}

float calcul_stddev(float average)
{
    double sum;
    int count;
    float comp;
    float sqrt;

    count = 0;
    sum = 0;
    comp = 0;
    while (count < env.stats.count)
    {
        env.stats.interval_array[count] -= average;
        env.stats.interval_array[count] *= env.stats.interval_array[count];
        sum += (double)env.stats.interval_array[count];
        count++;
    }
    sum /= env.stats.count;
    sqrt = sum / 2;
    while (sqrt != comp)
    {
        comp = sqrt;
        sqrt = (sum/comp + comp) / 2;
    }
    return (sqrt);
}

void clear_ressources(void)
{
    if (env.stats.interval_array)
        free(env.stats.interval_array);
    close(env.sockfd);
    freeaddrinfo(env.addr);
}