#include "../inc/ping.h"

/*
 * If sendto() failed then announce it and prepare for next ping
*/ 
void	manage_send_failure(void)
{
	printf("ICMP echo request icmp_seq=%d ttl=%d to %s failed\n", env.icmp_req.seq, env.ttl, env.args.hostname);
	env.stats.count++;
	env.stats.error++;    
	set_next_ping();
}

/*
 * If recvmsg() failed then announce timeout
*/ 
void	manage_recv_failure(void)
{
	env.stats.error++;
	env.stats.count++;
	if (env.args.verbose)
		printf("Request timeout for icmp_seq=%d\n", env.icmp_req.seq);
}

/*
 * If error code in icmp reply then process and display it
*/ 
void	manage_reply_error(void)
{
	t_ip_header	ip_error;

	retrieve_ip_info(&(env.r_data.m_data[28]), &(ip_error));
	display_error(ip_error.ttl);
	env.stats.error++;
	env.stats.count++;
}

/*
 * If ping succeeded then display information
*/ 
void	manage_recv_sucess(int retrecv)
{
	retrieve_info();
	if (env.icmp_res->type != 0)
		manage_reply_error();
	else
	{
		set_stats();
		display_ping((retrecv - 20));
	}
}
