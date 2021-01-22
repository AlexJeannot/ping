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

// int error_control()
// {
//     if (env.icmp_res.type == 3 || env.icmp_res.type == 11)
//         return (1);
//     return (0);
// }

void error_control()
{
    struct cmsghdr *control;
    struct sock_extended_err *pkt;
    int *ttl;

    ttl = NULL;
    control = CMSG_FIRSTHDR(&(env.r_data.msg));
    printf("AVANT WHILE\n");
    printf("control->cmsg_level = %d ----- control->cmsg_type = %d\n", control->cmsg_level, control->cmsg_type);
    while (control)
    {
        printf("WHILE\n");
        if (control->cmsg_level == IPPROTO_IP && control->cmsg_type == IP_RECVTTL)
        {
            printf("PASSE IF\n");
            ttl = ((int *)CMSG_DATA(control));
        }
        if (control->cmsg_level == IPPROTO_IP && control->cmsg_type == IP_RECVERR)
        {
            printf("PASSE IF\n");
            pkt = ((struct sock_extended_err *)CMSG_DATA(control));
        }
        control = CMSG_NXTHDR(&(env.r_data.msg), control);
    }
    if (ttl)
        printf("TTL = %d\n", *ttl);
    if (pkt)
    {
        printf ("ee_errno = %d", pkt->ee_errno);
        printf ("ee_origin = %d", pkt->ee_origin);
        printf ("ee_type = %d", pkt->ee_type);
        printf ("ee_code = %d", pkt->ee_code);
        printf ("ee_pad = %d", pkt->ee_pad);
        printf ("ee_info = %d", pkt->ee_info);
        printf ("ee_data = %d", pkt->ee_data);
    }
}

void display_error()
{
    t_ip_header ip_header;
    char *dest_error_code[16] = {"Destination network unreachable", "Destination host unreachable" \
                                "Destination protocol unreachable", "Destination port unreachable" \
                                "Fragmentation required", "Source route failed", "Destination network unknown" \
                                "Destination host unknown", "Source host isolated", "Network administratively prohibited" \
                                "Host administratively prohibited", "Network unreachable for ToS", \
                                "Host unreachable for ToS", "Communication administratively prohibited", \
                                "Host Precedence Violation", "Precedence cutoff in effect"};
    char *ttl_error_code[2] = {"Time To Live exceeded", "Fragment reassembly time exceeded"};
    char *error_msg;

    retrieve_ip_info(&(env.icmp_res.data[0]), &(ip_header));
    display_ip_header_info(ip_header, "ERROR");

    char src[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(env.r_data.r_addr.sin_addr), src, INET_ADDRSTRLEN);
    printf(">>>>>>>>>>>>>>>>>>> SRC ADRESS = %s\n", src);

    error_msg = (env.icmp_res.type == 3) ? dest_error_code[env.icmp_res.code] : ttl_error_code[env.icmp_res.code];
    printf("From %s icmp_seq=%d %s\n", "TEST", env.icmp_res.seq, error_msg);
}


int main(int argc, char** argv)
{
    bzero(&env, sizeof(env));
    env.ttl = 0;

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

        int retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), env.addr->ai_addrlen);

        printf("retsend = %d\n", retsend);

        alarm(1);
        env.pause = 1;
        int retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL);

        // char src[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(env.r_data.r_addr.sin_addr), src, INET_ADDRSTRLEN);
        // printf("STP = %s\n", src);

        printf("retrecv = %d\n", retrecv);
        if (retrecv == -1)
            perror("Error recv: ");

        env.ts_after = get_ts_ms();

        // char src[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(env.r_data.r_addr.sin_addr), src, INET_ADDRSTRLEN);
        // printf(">>>>>>>>>>>>>>>>>>> SRC ADRESS = %s\n", src);

        retrieve_ip_info(&(env.r_data.databuf[0]), &(env.ip_res));
        display_ip_header_info(env.ip_res, "REPONSE");

        // char src[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(env.r_data.r_addr.sin_addr), src, INET_ADDRSTRLEN);
        // printf(">>>>>>>>>>>>>>>>>>> SRC ADRESS = %s\n", src);

        printf("offset = %d\n", (env.ip_res.header_size * 4));
        retrieve_icmp_info(&(env.r_data.databuf[env.ip_res.header_size * 4]), &(env.icmp_res), (retrecv - (env.ip_res.header_size * 4)));
        // display_icmp_header_info(env.icmp_res, "RESPONSE");

        char src[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(env.r_data.r_addr.sin_addr), src, INET_ADDRSTRLEN);
        printf(">>>>>>>>>>>>>>>>>>> SRC ADRESS = %s\n", src);

        printf("env.r_data.msg.msg_name = %p\n", env.r_data.msg.msg_name);

        // if (error_control())
        //     display_error();
        error_control();
        env.interval = (env.ts_after - env.ts_before);
        printf("ping interval = %Lf\n", env.interval);
        env.icmp_req.seq++;
        bzero(&(env.icmp_req.checksum), sizeof(env.icmp_req.checksum));
        env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
        set_reception_struct(&env);
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