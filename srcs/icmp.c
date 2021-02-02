#include "../inc/ping.h"

/*
 * Checksum calculation
 * data is adress of first header byte
 * For every 2 bytes of header 
 * -> Add 2 bytes value to checksum
 * If header length is odd
 * -> Add last byte value to checksum
 * Add most significant byte and least significant byte
 * ones complement of checksum
*/ 
uint16_t	calcul_checksum(void *data, int size)
{
	uint64_t	checksum;
	uint16_t	*addr;

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

/*
 * Prepare icmp request
 * type is 8 (echo request)
 * code is 0 (echo request) -> already set because of main structure bzero() 
 * id is pid
 * set timestamp in data part of icmp request (big endian)
*/ 
void	set_icmp_req(void)
{
	long	ts;
	short	pos_bytes;

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
