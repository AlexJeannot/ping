#include "ft_ping.h"

void set_icmp_req(void)
{
    long ts;
    short pos_bytes;

    env.icmp_req.type = 8;
    env.icmp_req.id = (uint16_t)getpid();

    pos_bytes = 7;
    ts = get_ts_s();
    while (pos_bytes >= 0)
    {
        env.icmp_req.data[pos_bytes] = (ts >> (pos_bytes * 8) & 0xFF);
        pos_bytes--;
    }
    env.icmp_req.checksum = calcul_checksum(&(env.icmp_req), sizeof(env.icmp_req));
}

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

void retrieve_ip_info(const char* data, t_ip_header *res)
{
    int offset;
    int count;

    offset = 0;
    count = 0;
    res->version = ((uint8_t)data[offset]);
    res->version = (res->version >> 4);
    res->header_size = ((uint8_t)data[offset++]);
    res->header_size = (res->header_size & 0x0F);

    res->dscp = ((uint8_t)data[offset]);
    res->dscp = (res->dscp >> 2);
    res->ecn = ((uint8_t)data[offset++]);
    res->ecn = (res->ecn & 0x03);

    res->packet_size = 0;
    res->packet_size = ((uint8_t)data[++offset]);
    res->packet_size = (res->packet_size << 8);
    res->packet_size += ((uint8_t)data[--offset]);

    offset += 2;

    res->id = ((uint8_t)data[offset++]);
    res->id = (res->id << 8);
    res->id += ((uint8_t)data[offset++]);

    res->flags = ((uint8_t)data[offset] & 0xe0);
    res->offset = ((uint8_t)data[offset++] & 0x07);
    res->offset = (res->offset << 8);
    res->offset += ((uint8_t)data[offset++]);


    res->ttl = ((uint8_t)data[offset++]);

    res->protocol = ((uint8_t)data[offset++]);

    res->checksum = ((uint8_t)data[offset++]);
    res->checksum = (res->checksum << 8);
    res->checksum += ((uint8_t)data[offset++]);

    while (count++ < 4)
    {
        res->s_addr += ((uint8_t)data[offset++]);
        if (count < 4)
            res->s_addr = (res->s_addr << 8);
    }
    
    count = 0;
    while (count++ < 4)
    {
        res->d_addr += ((uint8_t)data[offset++]);
        if (count < 4)
            res->d_addr = (res->d_addr << 8);
    }
}

void retrieve_icmp_info(const char* data, t_icmp_header *res, int size)
{
    int offset;
    int count;

    offset = 0;
    count = 0;
    bzero(res, sizeof(*res));
    res->type = (uint8_t)data[offset++];
    res->code = (uint8_t)data[offset++];

    res->checksum = (uint8_t)data[offset++];
    res->checksum = (res->checksum << 8);
    res->checksum += (uint8_t)data[offset++];

    if (size > 24)
    {
        res->id = (uint8_t)data[offset++];
        res->id = (res->id << 8);
        res->id += (uint8_t)data[offset++];

        res->seq = (uint8_t)data[offset++];
        res->seq = (res->seq << 8);
        res->seq += (uint8_t)data[offset++];
        while (offset < size)
            res->data[count++] = (char)data[offset++];

    }
}

void verify_icmp_response(t_icmp_header res)
{
    if (res.type != 0 && res.code != 0)
    {

    }
}
