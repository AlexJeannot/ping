#include "../inc/ping.h"

void set_reception_struct(void)
{
    bzero(&(env.r_data), sizeof(env.r_data));
    env.r_data.msg.msg_name = &(env.r_data.s_addr_in);
    env.r_data.msg.msg_namelen = sizeof(env.r_data.s_addr_in);
    env.r_data.iov.iov_base = &(env.r_data.m_data[0]);
    env.r_data.iov.iov_len = sizeof(env.r_data.m_data);
    env.r_data.msg.msg_iov = &(env.r_data.iov);
    env.r_data.msg.msg_iovlen = 1;
    env.r_data.msg.msg_control = &(env.r_data.a_data[0]);
    env.r_data.msg.msg_controllen = sizeof(env.r_data.a_data);
    env.r_data.msg.msg_flags = 0;
}

void retrieve_info(void)
{
    int *ttl;
    struct cmsghdr *cmsg;

    bzero(&(cmsg), sizeof(cmsg));
    ttl = NULL;
    cmsg = CMSG_FIRSTHDR(&(env.r_data.msg));

    env.ts_after = get_ts_ms();
    if (getnameinfo(env.r_data.msg.msg_name, env.r_data.msg.msg_namelen, &env.r_data.s_host[0], sizeof(env.r_data.s_host), NULL, 0, 0) != 0)
        ft_strlcpy(&(env.r_data.s_host[0]), "N/A", 4);
    inet_ntop(AF_INET, &(env.r_data.s_addr_in.sin_addr), env.r_data.s_addr, INET_ADDRSTRLEN);

    while (cmsg)
    {
        if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
            ttl = ((int *)CMSG_DATA(cmsg));
        cmsg = CMSG_NXTHDR(&(env.r_data.msg), cmsg);
    }
    if (ttl)
        env.r_ttl = *ttl;

    env.icmp_res = (t_icmp_header *)&env.r_data.m_data[20];
}

void set_next_ping(void)
{
    if (env.args.counter && env.stats.count == env.args.counter)
        display_summary();
    bzero(&env.ts_before, sizeof(env.ts_before));
    bzero(&env.ts_after, sizeof(env.ts_after));
    bzero(&(env.icmp_req.checksum), sizeof(env.icmp_req.checksum));
    env.icmp_req.seq++;
    env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
    set_reception_struct();
}