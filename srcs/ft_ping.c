#include "../inc/ft_ping.h"

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

void retrieve_info(void)
{
    struct sock_extended_err *pkt;
    int *ttl;
    struct cmsghdr *cmsg;

    bzero(&(cmsg), sizeof(cmsg));
    ttl = NULL;
    cmsg = CMSG_FIRSTHDR(&(env.r_data.msg));
    pkt = NULL;

    if (getnameinfo(env.r_data.msg.msg_name, env.r_data.msg.msg_namelen, &env.r_data.s_host[0], sizeof(env.r_data.s_host), NULL, 0, 0) != 0)
        ft_strlcpy(&(env.r_data.s_host[0]), "N/A", 4);

    inet_ntop(AF_INET, &(env.r_data.s_addr_in.sin_addr), env.r_data.s_addr, INET_ADDRSTRLEN);


    printf("cmsg->cmsg_level = %d ----- cmsg->cmsg_type = %d\n", cmsg->cmsg_level, cmsg->cmsg_type);
    while (cmsg)
    {
        printf("cmsg->cmsg_level = %d ----- cmsg->cmsg_type = %d\n", cmsg->cmsg_level, cmsg->cmsg_type);
        if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVTTL)
        {
            ttl = ((int *)CMSG_DATA(cmsg));
        }
        //TODO ON LINUX
        if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVERR)
        {
            pkt = ((struct sock_extended_err *)CMSG_DATA(cmsg));
        }
        cmsg = CMSG_NXTHDR(&(env.r_data.msg), cmsg);
    }
    if (ttl)
        env.r_ttl = *ttl;
    //TODO ON LINUX
    if (pkt)
    {
        printf("------------\n");
        printf ("ee_errno = %d\n", pkt->ee_errno);
        printf ("ee_origin = %d\n", pkt->ee_origin);
        printf ("ee_type = %d\n", pkt->ee_type);
        printf ("ee_code = %d\n", pkt->ee_code);
        printf ("ee_pad = %d\n", pkt->ee_pad);
        printf ("ee_info = %d\n", pkt->ee_info);
        printf ("ee_data = %d\n", pkt->ee_data);
        printf("------------\n");
        if (pkt->ee_origin == 2)
        {
            printf("ERROR FROM ICMP\n");

        }
    }
}

void set_timeout_and_ts(void)
{
    alarm(1);
    env.timeout = 1;
    env.ts_before = get_ts_ms();
    if (env.stats.count == 0)
        env.ts_start = env.ts_before;
}

void set_next_ping(void)
{
    bzero(&env.ts_before, sizeof(env.ts_before));
    bzero(&env.ts_after, sizeof(env.ts_after));
    bzero(&(env.icmp_req.checksum), sizeof(env.icmp_req.checksum));
    env.icmp_req.seq++;
    env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
    set_reception_struct();
}

void reserve_interval_array(int nb)
{
    float *new_array;
    int count;

    count = 0;
    if (!(new_array = (float*)malloc(sizeof(float) * nb)))
        error_exit("Interval array memory allocation");
    if (env.stats.interval_array)
    {
        while (count < env.stats.alloc_interval)
        {
            new_array[count] = env.stats.interval_array[count];
            count++;
        }
        free(env.stats.interval_array);
    }
    env.stats.interval_array = new_array;
    env.stats.alloc_interval += nb;
}

void set_stats(void)
{
    env.interval = (float)(env.ts_after - env.ts_before);
    env.stats.count++;
    if (env.interval < env.stats.min || env.stats.min == 0)
        env.stats.min = env.interval;
    if (env.interval > env.stats.max || env.stats.min == 0)
        env.stats.max = env.interval;
    env.stats.sum += env.interval;
    if (env.stats.count  > env.stats.alloc_interval)
        reserve_interval_array(25);
    env.stats.interval_array[env.stats.count - 1] = env.interval;
}

void manage_send_failure(void)
{
    printf("ICMP echo request icmp_seq=%d ttl=%d to %s failed\n", env.icmp_req.seq, env.ttl, env.args.hostname);
    env.stats.count++;
    env.stats.error++;    
    set_next_ping();
}

void ping_loop(void)
{
    int retsend;
    int retrecv;

    while (1)
    {
        set_timeout_and_ts();
        printf("BEFORE SEND\n");
        if ((retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), env.addr->ai_addrlen)) < 0)
        {
            printf("SNED FAILED\n");
            manage_send_failure();
            continue;
        }
        printf("AFTER SEND\n");
        if ((retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL)) < 0)  //MSG_WAITALL MSG_ERRQUEUE
        {
            if (errno == EAGAIN)
                printf("EAGAIN\n");
            perror("Error recv: ");
        }
        printf("env.r_data.msg.msg_flags = %d\n", env.r_data.msg.msg_flags);

        printf("retrecv = %d\n", retrecv);
        struct icmphdr *icmp_response;
        bzero(&(icmp_response), sizeof(icmp_response));
        icmp_response = (struct icmphdr *)&env.r_data.m_data[20];

        printf("icmp_response->type = %d\n", icmp_response->type);
        env.ts_after = get_ts_ms();

        // retrieve_ip_info(&(env.r_data.m_data[0]), &(env.ip_res));
        // retrieve_icmp_info(&(env.r_data.m_data[20]), &(env.icmp_res), (retrecv - (env.ip_res.header_size * 4)));

        retrieve_info();
        set_stats();
        display_ping((retrecv - 20)); //(env.ip_res.header_size * 4
        set_next_ping();
        while (env.timeout) ;
    }
}

void signal_handler(int code)
{
    if (code == SIGALRM)
    {
        printf("SIGALARM\n");
        env.timeout = 0;
    }
    if (code == SIGINT || code == SIGQUIT)
        display_summary();
}

void manage_signal(void)
{
    signal(SIGALRM, &signal_handler);
    // signal(SIGINT, &signal_handler);
    // signal(SIGQUIT, &signal_handler);
}

int main(int argc, char** argv)
{
    bzero(&env, sizeof(env));
    parse_args(argc, argv);
    get_addr();
    display_addr_info(env.addr);
    manage_signal();
    reserve_interval_array(25);
    set_socket();
    set_icmp_req();
    set_reception_struct();
    display_introduction();
    ping_loop();
    return (0);
}