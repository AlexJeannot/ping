#include "../inc/ping.h"

/*
 * Retrieve first 32 bits word from ip header
 * IP version
 * Header size
 * Differentiated Services Code Point
 * Explicit Congestion Notification
 * Packet size
*/
int retrieve_first_word(const char* data, t_ip_header *res, int offset)
{
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
    return (offset);
}

/*
 * Retrieve second 32 bits word from ip header
 * Identification
 * Flags
 * Fragment Offset
*/
int retrieve_second_word(const char* data, t_ip_header *res, int offset)
{
    res->id = ((uint8_t)data[offset++]);
    res->id = (res->id << 8);
    res->id += ((uint8_t)data[offset++]);
    res->flags = ((uint8_t)data[offset] & 0xe0);
    res->offset = ((uint8_t)data[offset++] & 0x07);
    res->offset = (res->offset << 8);
    res->offset += ((uint8_t)data[offset++]);

    return (offset);
}

/*
 * Retrieve third 32 bits word from ip header
 * Time To Live
 * Protocol
 * Checksum
*/
int retrieve_third_word(const char* data, t_ip_header *res, int offset)
{
    res->ttl = ((uint8_t)data[offset++]);
    res->protocol = ((uint8_t)data[offset++]);
    res->checksum = ((uint8_t)data[offset++]);
    res->checksum = (res->checksum << 8);
    res->checksum += ((uint8_t)data[offset++]);

    return (offset);
}

/*
 * Retrieve fourth and fifth 32 bits word from ip header
 * Source IP adress
 * Destination IP Adress
*/
void retrieve_ip_addr(const char* data, t_ip_header *res, int offset)
{
    int count;

    count = 0;
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

/*
 * main fonction for IP informations retrieving
*/ 
void retrieve_ip_info(const char* data, t_ip_header *res)
{
    int offset;

    offset = 0;
    offset = retrieve_first_word(data, res, offset);
    offset = retrieve_second_word(data, res, offset);
    offset = retrieve_third_word(data, res, offset);
    retrieve_ip_addr(data, res, offset);
}