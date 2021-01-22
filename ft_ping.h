#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>

typedef struct s_args
{
    int verbose;
    char *hostname;
} t_args;

typedef struct s_ip_header
{
    uint8_t version;
    uint8_t header_size;
    uint8_t dscp;
    uint8_t ecn;
    uint16_t packet_size;
    uint16_t id;
    uint8_t flags;
    uint16_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t s_addr;
    uint32_t d_addr;

} t_ip_header;



typedef struct s_icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
    char data[56];
} t_icmp_header;

typedef struct s_reception
{
    struct sockaddr_in r_addr;
    struct msghdr msg;
    struct iovec iov;
    char databuf[1024];
    char datacontrol[1024];
} t_reception;

typedef struct s_env
{
    int sockfd;
    int ttl;
    int pause;
    t_args args;
    struct addrinfo *addr;
    t_ip_header ip_res;
    t_icmp_header icmp_req;
    t_icmp_header icmp_res;
    t_reception r_data;
    long double ts_before;
    long double ts_after;
    long double interval;
} t_env;

typedef struct s_error_code_dest
{

} t_s_error_code_dest;

t_env env;

void setup_socket(t_env *env);
void error_exit(char *error_msg);
long get_ts_s(void);
long double get_ts_ms(void);
void setup_icmp_req(t_env *env);
uint16_t calcul_checksum(void *data, int size);
void retrieve_icmp_info(const char* data, t_icmp_header *response, int size);
void display_addr_error(int error_code);
void get_addr(t_env *env);
void display_addr_info(struct addrinfo *addr);
void set_reception_struct(t_env *env);
void display_icmp_header_info(t_icmp_header header, char *name);
void retrieve_ip_info(const char* data, t_ip_header *res);
void display_ip_header_info(t_ip_header header, char *name);

#endif