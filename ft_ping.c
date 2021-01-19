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


int main()
{
    t_env env;
    bzero(&env, sizeof(env));
    env.ttl = 10;


    get_addr(&env);


    display_addr_info(env.addr);

    setup_socket(&env);


    setup_icmp_req(&env);


    set_reception_struct(&env);

    env.ts_before = get_ts_ms();

    int retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), sizeof(env.addr->ai_addr));

    printf("retsend = %d\n", retsend);

    int retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL);

    printf("retrecv = %d\n", retrecv);

    env.ts_after = get_ts_ms();


    retrieve_ip_info(&(env.r_data.databuf[0]), &(env.ip_res));
    display_ip_header_info(env.ip_res, "REPONSE");

    retrieve_icmp_info(&(env.r_data.databuf[20]), &(env.icmp_res), retrecv);
    display_icmp_header_info(env.icmp_res, "RESPONSE");



    // verify_icmp_response(env.icmp_res);

    env.interval = (env.ts_after - env.ts_before);



    return (0);


}

/*     uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
    uint64_t ts; */