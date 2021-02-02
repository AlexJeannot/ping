#include "../inc/ping.h"

/*
 * Signals handler function
 * SIGALRM is use for timer
 * SIGINT and SIGQUIT display stastitics summary and exit program 
*/
void	signal_handler(int code)
{
	if (code == SIGALRM)
		env.timeout = 0;
	if (code == SIGINT || code == SIGQUIT)
		display_summary();
}

/*
 * Set signal manager
*/
void	manage_signal(void)
{
	signal(SIGALRM, &signal_handler);
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
}
