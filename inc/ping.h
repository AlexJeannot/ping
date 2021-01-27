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
#include "../libft/libft.h"

typedef struct	s_args
{
	char	error_msg[1024];
	int	verbose;
	int	ttl;
	int	counter;
	char	*hostname;
}		t_args;

typedef struct	s_stats
{
	int	count;
	int	error;
	float	min;
	float	max;
	double	sum;
	float	*interval_array;
	int	alloc_interval;
}		t_stats;

typedef struct	s_ip_header
{
	uint8_t		version;
	uint8_t		header_size;
	uint8_t		dscp;
	uint8_t		ecn;
	uint16_t	packet_size;
	uint16_t	id;
	uint8_t		flags;
	uint16_t	offset;
	uint8_t		ttl;
	uint8_t		protocol;
	uint16_t	checksum;
	uint32_t	s_addr;
	uint32_t	d_addr;

}		t_ip_header;

typedef struct	s_icmp_header
{
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
	uint16_t	id;
	uint16_t	seq;
	char		data[56];
}		t_icmp_header;

typedef struct	s_reception
{
	struct msghdr		msg;
	struct iovec		iov;
	char			m_data[1024]; // main data
	char			a_data[1024]; // ancillary data
	struct			sockaddr_in s_addr_in;
	char			s_addr[INET_ADDRSTRLEN];
	char			s_host[1024];
}		t_reception;

typedef struct	s_env
{
	int		sockfd;
	int		ttl;
	int		timeout;
	t_args		args;
	struct addrinfo	*addr;
	t_ip_header	ip_res;
	t_icmp_header	icmp_req;
	t_icmp_header	*icmp_res;
	t_reception	r_data;
	long double	ts_before;
	long double	ts_after;
	long double	ts_start;
	float		interval;
	int		r_ttl;
	t_stats		stats;

}		t_env;

t_env	env;

void		set_socket(void);
void		error_exit(char *error_msg);
long		get_ts_s(void);
long double	get_ts_ms(void);
void		set_icmp_req(void);
uint16_t	calcul_checksum(void *data, int size);
void		retrieve_icmp_info(const char* data, t_icmp_header *response, int size);
void		display_addr_error(int error_code);
void		get_addr(void);
void		set_reception_struct(void);
void		retrieve_ip_info(const char* data, t_ip_header *res);
void		parse_args(int argc, char **argv);
void		display_help(int exit_code);
void		display_wrong_option(char option);
void		display_error(int ttl);
void		display_ping(int bytes);
void		display_summary(void);
float		calcul_stddev(float average);
void		clear_ressources(void);
void		getaddr_error(char *target, int error_code);
void		display_introduction(void);
uint16_t	calcul_checksum(void *data, int size);
void		retrieve_info(void);
void		set_stats(void);
void		reserve_interval_array(int nb);
void		signal_handler(int code);
void		manage_signal(void);
void		manage_recv_sucess(int retrecv);
void 		manage_recv_failure(void);
void		manage_send_failure(void);
void		set_next_ping(void);
void		set_timeout_and_ts(void);

#endif
