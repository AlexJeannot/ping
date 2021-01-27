#include "../inc/ping.h"

void	reserve_interval_array(int nb)
{
	float	*new_array;
	int	count;

	count = 0;
	if (!(new_array = (float*)malloc(sizeof(float) * nb)))
		error_exit("Interval array memory allocation");
	if (env.stats.interval_array)
	{
		while (count < env.stats.alloc_interval)
		{
			new_array[count] = env.stats.interval_array[count];
			count++;
		}
		free(env.stats.interval_array);
	}
	env.stats.interval_array = new_array;
	env.stats.alloc_interval += nb;
}

void	clear_ressources(void)
{
	if (env.stats.interval_array)
		free(env.stats.interval_array);
	close(env.sockfd);
	freeaddrinfo(env.addr);
}
