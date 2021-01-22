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


void verify_args(int argc, char **argv, t_env *env)
{
    int pos;
    int count;

    pos = 1;
    count = 0;
    if (argc < 2)
        display_help(1);
    while (pos < argc)
    {
        if (argv[pos][count] == '-' && argv[pos][count + 1])
        {
            while (argv[pos][++count])
            {
                if (argv[pos][count] == 'v')
                    env->args.verbose = 1;
                else if (argv[pos][count] == 'h')
                    display_help(0);
                else
                    display_wrong_option(argv[pos][count]);
            }
        }
        else
        {
            if (env->args.hostname)
                display_help(1);
            env->args.hostname = argv[pos];
        }
        pos++;
    }
}

void signal_handler(int code)
{
    if (code == SIGALRM)
        env.timeout = 0;
}

void retrieve_info()
{
    // struct sock_extended_err *pkt;
    int *ttl;
    struct cmsghdr *cmsg;

    bzero(&(cmsg), sizeof(cmsg));
    ttl = NULL;
    cmsg = CMSG_FIRSTHDR(&(env.r_data.msg));

    getnameinfo(env.r_data.msg.msg_name, env.r_data.msg.msg_namelen, &env.r_data.s_host[0], sizeof(env.r_data.s_host), NULL, 0, 0);

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
        // if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVERR)
        // {
        //     pkt = ((struct sock_extended_err *)CMSG_DATA(cmsg));
        // }
        cmsg = CMSG_NXTHDR(&(env.r_data.msg), cmsg);
    }
    if (ttl)
        env.r_ttl = *ttl;
    //TODO ON LINUX
    // if (pkt)
    // {
    //     printf ("ee_errno = %d", pkt->ee_errno);
    //     printf ("ee_origin = %d", pkt->ee_origin);
    //     printf ("ee_type = %d", pkt->ee_type);
    //     printf ("ee_code = %d", pkt->ee_code);
    //     printf ("ee_pad = %d", pkt->ee_pad);
    //     printf ("ee_info = %d", pkt->ee_info);
    //     printf ("ee_data = %d", pkt->ee_data);
    // }
}


int main(int argc, char** argv)
{
    bzero(&env, sizeof(env));
    env.ttl = 64;

    verify_args(argc, argv, &env);

    get_addr(&env);

    display_addr_info(env.addr);
    signal(SIGALRM, &signal_handler);

    setup_socket(&env);

    setup_icmp_req(&env);

    set_reception_struct(&env);

    while (1)
    {
        alarm(1);
        env.timeout = 1;
        env.ts_before = get_ts_ms();

        int retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), env.addr->ai_addrlen);

        printf("retsend = %d\n", retsend);

        int retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL);

        // printf("\n\n==================================\n\n");
        // int offset;

        // offset = 0;
        // for (int c = 0; c < retrecv; c++)
        // {
        //     for (int i = 7; i >= 0; i--)
        //         printf("%d", ((env.r_data.m_data[offset] >> i) & 1));
        //     printf("  ");
        //     if ((c + 1) % 8 == 0)
        //         printf("\n");
        //     offset++;
        // }
        // printf("\n\n==================================\n\n");

        printf("retrecv = %d\n", retrecv);
        if (retrecv == -1)
            perror("Error recv: ");

        env.ts_after = get_ts_ms();



        retrieve_ip_info(&(env.r_data.m_data[0]), &(env.ip_res));
        // display_ip_header_info(env.ip_res, "REPONSE");

        retrieve_icmp_info(&(env.r_data.m_data[env.ip_res.header_size * 4]), &(env.icmp_res), (retrecv - (env.ip_res.header_size * 4)));

        retrieve_info();
        // printf("ping interval = %Lf\n", env.interval);
        display_ping((retrecv - (env.ip_res.header_size * 4)));

        env.icmp_req.seq++;
        // bezro ts
        // bzero(&env.ts_before, sizeof(env.ts_before));
        // bzero(&env.ts_after, sizeof(env.ts_after));
        bzero(&(env.icmp_req.checksum), sizeof(env.icmp_req.checksum));
        env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
        set_reception_struct(&env);
        while (env.timeout) ;
    }




    return (0);


}