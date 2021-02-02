#ifndef FT_PING_H
# define FT_PING_H

/*
 * Needed librairies
*/
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

/*
 * Arguments structure
*/
typedef struct	s_args
{
	char	error_msg[1024];
	int		verbose;
	int		ttl;
	int		counter;
	char	*hostname;
}		t_args;

/*
 * Statistics structure
*/
typedef struct	s_stats
{
	int		count;
	int		error;
	float	min;
	float	max;
	double	sum;
	float	*interval_array;
	int		alloc_interval;
}		t_stats;

/*
 * IP header structure
*/
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

/*
 * ICMP header structure
*/
typedef struct	s_icmp_header
{
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
	uint16_t	id;
	uint16_t	seq;
	char		data[56];
}		t_icmp_header;

/*
 * Reception structure structure
*/
typedef struct	s_reception
{
	struct msghdr	msg;
	struct iovec	iov;
	char			m_data[1024];				// main data
	char			a_data[1024];				// ancillary data
	struct			sockaddr_in s_addr_in;
	char			s_addr[INET_ADDRSTRLEN];	// Source IP adress buffer
	char			s_host[1024];				// Source hostname buffer (reverse DNS)
}		t_reception;

/*
 * Main structure structure
*/
typedef struct	s_env
{
	int				sockfd;		// Socket
	int				ttl;		// Time To live (64 by default -> can by specified with -t option)
	int				timeout;	// Bool variable for timeout
	t_args			args;
	struct addrinfo	*addr;
	t_ip_header		ip_res;
	t_icmp_header	icmp_req;
	t_icmp_header	*icmp_res;
	t_reception		r_data;
	long double		ts_before;
	long double		ts_after;
	long double		ts_start;
	float			interval;
	int				r_ttl;		// Reception Time To Live
	t_stats		stats;

}		t_env;


t_env	env; // Global variable used along the program


/*
 * annexes.c
*/
void		reserve_interval_array(int nb);
void		clear_ressources(void);

/*
 * args.c
*/
void		parse_args(int argc, char **argv);

/*
 * data.c
*/
void		set_reception_struct(void);
void		retrieve_info(void);
void		set_next_ping(void);

/*
 * display.c
*/
void		display_help(int exit_code);
void		display_wrong_option(char option);
void		display_error(int ttl);
void		display_introduction(void);
void		display_ping(int bytes);
void		display_summary(void);

/*
 * error.c
*/
void		error_exit(char *error_msg);
void		getaddr_error(char *target, int error_code);

/*
 * icmp.c
*/
uint16_t	calcul_checksum(void *data, int size);
void		set_icmp_req(void);

/*
 * io_return.c
*/
void		manage_send_failure(void);
void 		manage_recv_failure(void);
void		manage_recv_sucess(int retrecv);

/*
 * ip.c
*/
void		retrieve_ip_info(const char* data, t_ip_header *res);

/*
 * network.c
*/
void		get_addr(void);
void		set_socket(void);

/*
 * signal.c
*/
void		signal_handler(int code);
void		manage_signal(void);

/*
 * stats.c
*/
float		calcul_stddev(float average);
void		set_stats(void);

/*
 * time.c
*/
long		get_ts_s(void);
long double	get_ts_ms(void);
void		set_timeout_and_ts(void);

#endif
