#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>

unsigned short calcul_checksum(void *data, int size) 
{
    unsigned long checksum;
    unsigned short *addr;

    checksum = 0;
    addr = data;
    while (size > 1)
    {
        printf(">> checksum = %lu\n", checksum);
        checksum += *addr;
        addr++;
        size -= (int)sizeof(unsigned short);
    }

    if (size == 1)
        checksum += *(unsigned char*)addr;

    // printf("checksum = %lu\n", checksum);
    // printf("checksum >> 16 = %lu\n", (checksum >> 16));
    // printf("checksum & 0xFFFF = %lu\n", (checksum & 0xFFFF));

    checksum = (checksum >> 16) + (checksum & 0xFFFF);

    // printf("-------------\n");
    // printf("checksum = %lu\n", checksum);
    // printf("checksum >> 16 = %lu\n", (checksum >> 16));

    checksum += (checksum >> 16);
    checksum = ~checksum;

    return ((unsigned short)checksum);

}

typedef struct icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
    uint64_t ts;
    char data[48];
} imcp_h;

int main()
{
    struct addrinfo *res;
    int ret;

    ret = getaddrinfo("8.8.8.8", NULL, NULL, &res);
    printf("----------- ret = %d ---------------\n", ret);
    if (ret == 0)
    {
        int count = 0;
        struct addrinfo *browse;

        browse = res;

        while (res)
        {
            printf("=============== %d ===============\n", count);
            printf("ai_flags = %d\n", res->ai_flags);
            printf("ai_family = %d\n", res->ai_family);
            printf("ai_socktype = %d\n", res->ai_socktype);
            printf("ai_protocol = %d\n", res->ai_protocol);
            printf("ai_addrlen = %u\n", res->ai_addrlen);
            printf("ai_canonname = %s\n", res->ai_canonname);

            printf("---------\n");
            printf("res->ai_addr->sa_family = %hhu\n", res->ai_addr->sa_family);
            for (size_t i = 0; i < res->ai_addrlen; i++)
                printf("%c", res->ai_addr->sa_data[i]);
            printf("\n");

            printf("---------\n");
            printf("==============================\n");


            res = res->ai_next;
            count++;
        }

        res = browse;

        int sockfd = 0;
        // int x= 1;
        sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

        if (sockfd < 0)
        {
            perror("socket: ");
            printf("errno: %d\n", errno);
        }


        imcp_h icmp;

        bzero(&icmp, sizeof(icmp));

        icmp.type = 8;
        icmp.code = 0;

        icmp.id = (uint16_t)65000;
        icmp.sequence = 65000;



        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);

        icmp.ts = tv.tv_sec;

        printf("sizeof(time_t) = %lu\n", sizeof(time_t));


        icmp.checksum = calcul_checksum(&icmp, sizeof(icmp));

        printf("sizeof(icmp) = %lu\n", sizeof(icmp));
        printf("sizeof(res->ai_addr) = %lu\n", sizeof(res->ai_addr));

        int retsend = sendto(sockfd, &icmp, sizeof(icmp), 0, (res->ai_addr), sizeof(res->ai_addr));

        printf("retsend = %d\n", retsend);

        struct msghdr msg;

        int retrecv = recvmsg(sockfd, &msg, 0);

        printf("retrecv = %d\n", retrecv);






    }

    return (0);


}