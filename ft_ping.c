#include "ft_ping.h"

uint16_t calcul_checksum(void *data, int size) 
{
    uint64_t checksum;
    uint16_t *addr;

    checksum = 0;
    addr = data;
    while (size > 1)
    {
        checksum += *addr;
        addr++;
        size -= (int)sizeof(uint16_t);
    }
    if (size == 1)
        checksum += *(uint8_t*)addr;
    checksum = (checksum >> 16) + (checksum & 0xFFFF);
    checksum += (checksum >> 16);
    checksum = ~checksum;

    return ((uint16_t)checksum);
}

void retrieve_icmp_info(char* data, icmp_h *response, int size)
{
    int offset;

    offset = 0;
    bzero(response, sizeof(*response));
    response->type = (uint8_t)data[offset++];
    response->code = (uint8_t)data[offset++];

    response->checksum = (uint8_t)data[offset++];
    response->checksum = (response->checksum << 8);
    response->checksum += (uint8_t)data[offset++];

    response->id = (uint8_t)data[offset++];
    response->id = (response->id << 8);
    response->id += (uint8_t)data[offset++];

    response->sequence = (uint8_t)data[offset++];
    response->sequence = (response->sequence << 8);
    response->sequence += (uint8_t)data[offset++];

    while (offset < size)
        response->data[count++] = (char)data[offset++];
}

int main()
{
    struct addrinfo *res;
    int ret;

    ret = getaddrinfo("google.com", NULL, NULL, &res);
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

        int ttl_val = 44;

        setsockopt(sockfd, 0, IP_TTL, &ttl_val, sizeof(ttl_val));
    

        // struct timeval tv_out;
        // tv_out.tv_sec = 1;
        // tv_out.tv_usec = 0;
        // int ret_to = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

        // printf("ret_to = %d\n", ret_to);

        icmp_h icmp;

        bzero(&icmp, sizeof(icmp));

        icmp.type = 8;
        icmp.code = 0;

        printf("getpid() = %d\n", getpid());
        icmp.id = (uint16_t)getpid();
        icmp.sequence = 0;



        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);

        // icmp.ts = tv.tv_sec;
        // printf(">>>>>> icmp.ts = %llu\n", icmp.ts);
        // icmp.ts *= 1000;
        // icmp.ts +=( tv.tv_usec / 1000);

        printf("sizeof(time_t) = %lu\n", sizeof(tv.tv_sec));

        // void* ptr = &tv.tv_sec;

        printf("tv.tv_sec = %ld\n", tv.tv_sec);
        icmp.data[0] = (tv.tv_sec >> (0 * 8) & 0xFF);
        icmp.data[1] = (tv.tv_sec >> (1 * 8) & 0xFF);
        icmp.data[2] = (tv.tv_sec >> (2 * 8) & 0xFF);
        icmp.data[3] = (tv.tv_sec >> (3 * 8) & 0xFF);
        icmp.data[4] = (tv.tv_sec >> (4 * 8) & 0xFF);
        icmp.data[5] = (tv.tv_sec >> (5 * 8) & 0xFF);
        icmp.data[6] = (tv.tv_sec >> (6 * 8) & 0xFF);
        icmp.data[7] = (tv.tv_sec >> (7 * 8) & 0xFF);

        icmp.checksum = calcul_checksum(&icmp, sizeof(icmp));

        printf("sizeof(icmp) = %lu\n", sizeof(icmp));
        printf("sizeof(res->ai_addr) = %lu\n", sizeof(res->ai_addr));

        struct msghdr msg;
        struct iovec iov[1];
        char databuf[1000];
        char datacontrol[1000];
        bzero(&databuf, sizeof(databuf));
        msg.msg_name = res->ai_addr;
        msg.msg_namelen = res->ai_addrlen;
        iov[0].iov_base = &databuf;
        iov[0].iov_len = sizeof(databuf);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
        msg.msg_control = &datacontrol;
        msg.msg_controllen = sizeof(datacontrol);
        msg.msg_flags = 0;





        struct timeval tv_bef;
        struct timezone tz_bef;
        gettimeofday(&tv_bef, &tz_bef);
    
        int retsend = sendto(sockfd, &icmp, sizeof(icmp), 0, (res->ai_addr), sizeof(res->ai_addr));

        printf("retsend = %d\n", retsend);

        int retrecv = recvmsg(sockfd, &msg, MSG_WAITALL);

        struct timeval tv_af;
        struct timezone tz_af;
        gettimeofday(&tv_af, &tz_af);

        long double delay = ( ( ((long double)tv_af.tv_sec * 1000) + ((long double)tv_af.tv_usec / 1000) ) - ( ((long double)tv_bef.tv_sec * 1000) + ((long double)tv_bef.tv_usec / 1000)  ));

        printf(">>>>>>>>>>> delay = %Lf\n", delay);







        printf("retrecv = %d\n", retrecv);
        for (int i = 0; i < retrecv; i++)
            write(1, &(iov[0].iov_base[i]), 1);
        printf("\n");
        for (int i = 0; i < retrecv; i++)
            write(1, &(databuf[i]), 1);
        printf("\n");
        // databuf += 20;

        icmp_h response;

        // response.type = databuf[20];
        // databuf += 1;
        // response.code = databuf[21];
        // databuf += 1;

        // response.checksum = (uint8_t)databuf[22];
        // response.checksum = (response.checksum << 8);
        // printf("aftet bitwise checksum = %d\n", response.checksum);
        // printf("databuf[23] = %d\n", (uint8_t)databuf[23]);
        // response.checksum += (uint8_t)databuf[23];

        retrieve_icmp_info(&databuf[20], &response, retrecv);

        // databuf += 2;
        // response.id = databuf[25];
        // databuf += 2;
        // response.sequence = databuf[27];
        // databuf += 2;
        // response.ts = databuf[29];
        // databuf += 8;
        //response.data = databuf[35];

        printf("response.type = %d\n", response.type);
        printf("response.code = %d\n", response.code);
        printf("response.checksum = %d\n", response.checksum);
        printf("response.id = %d\n", response.id);
        printf("response.sequence = %d\n", response.sequence);
        // printf("response.ts = %llu\n", response.ts);


    }

    return (0);


}

/*     uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
    uint64_t ts; */