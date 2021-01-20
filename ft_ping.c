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

void display_help(int exit_code)
{
    printf("HELP TO DO");
    exit(exit_code);
}

void display_wrong_option(char option)
{
    printf("Ping: invalid option -- '%c'\n", option);
    display_help(1);
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
        env.pause = 0;
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
        env.ts_before = get_ts_ms();

        int retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), sizeof(env.addr->ai_addr));

        printf("retsend = %d\n", retsend);

        alarm(1);
        env.pause = 1;
        int retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL);

        printf("retrecv = %d\n", retrecv);

        env.ts_after = get_ts_ms();


        retrieve_ip_info(&(env.r_data.databuf[0]), &(env.ip_res));
        // display_ip_header_info(env.ip_res, "REPONSE");

        retrieve_icmp_info(&(env.r_data.databuf[20]), &(env.icmp_res), retrecv);
        // display_icmp_header_info(env.icmp_res, "RESPONSE");

        env.interval = (env.ts_after - env.ts_before);
        printf("ping interval = %Lf\n", env.interval);
        env.icmp_req.seq++;
        bzero(&(env.icmp_req.checksum), sizeof(env.icmp_req.checksum));
        env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
        bzero(&(env.r_data), sizeof(env.r_data));
        while (env.pause) ;
    }

    // struct cmsghdr *cmhdr;
    // unsigned char tos;
    // cmhdr = CMSG_FIRSTHDR(&env.r_data.msg);
    // while (cmhdr) {
    //     if (cmhdr->cmsg_level == IPPROTO_IP && cmhdr->cmsg_type == IP_TOS) {
    //         // read the TOS byte in the IP header
    //         tos = ((unsigned char *)CMSG_DATA(cmhdr))[0];
    //     }
    //     cmhdr = CMSG_NXTHDR(&env.r_data.msg, cmhdr);
    // }
    // printf("tos  = %c\n", tos);

    // t_ip_header test;
    // retrieve_ip_info(&(env.icmp_res.data[0]), &(test));
    // display_ip_header_info(test, "ERROR");



    // verify_icmp_response(env.icmp_res);




    return (0);


}

/*     uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
    uint64_t ts; */