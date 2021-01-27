#include "../inc/ping.h"

void	signal_handler(int code)
{
	if (code == SIGALRM)
		env.timeout = 0;
	if (code == SIGINT || code == SIGQUIT)
		display_summary();
}

void	manage_signal(void)
{
	signal(SIGALRM, &signal_handler);
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
}
