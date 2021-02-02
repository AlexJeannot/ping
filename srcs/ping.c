#include "../inc/ping.h"

/*
 * Ping loop
 * Set alarm and start TS
 * send cimp request
 * receive icmp reply and handle error or response
 * Prepare next ping
 * Wait for alarm signal (would use sleep function for 42's subject)
*/
void	ping_loop(void)
{
	int	retsend;
	int	retrecv;

	while (1)
	{
		set_timeout_and_ts();
		if ((retsend = sendto(env.sockfd, &(env.icmp_req), sizeof(env.icmp_req), 0, (env.addr->ai_addr), env.addr->ai_addrlen)) < 0)
		{
			manage_send_failure();
			continue;
		}
		if ((retrecv = recvmsg(env.sockfd, &(env.r_data.msg), MSG_WAITALL)) < 0)
			manage_recv_failure();
		else
			manage_recv_sucess(retrecv);
		set_next_ping();
		while (env.timeout) ;
	}
}

/*
 * Main function
 * Setup everything and call ping loop
*/
int	main(int argc, char** argv)
{
	bzero(&env, sizeof(env));
	parse_args(argc, argv);
	get_addr();
	manage_signal();
	reserve_interval_array(25);
	set_socket();
	set_icmp_req();
	set_reception_struct();
	display_introduction();
	ping_loop();
	return (0);
}
