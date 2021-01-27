#include "../inc/ping.h"

float	calcul_stddev(float average)
{
	double	sum;
	int	count;
	float	comp;
	float	sqrt;

	count = 0;
	sum = 0;
	comp = 0;
	if (env.stats.count == env.stats.error)
		return (0);
	while (count < (env.stats.count - env.stats.error))
	{
		env.stats.interval_array[count] -= average;
		env.stats.interval_array[count] *= env.stats.interval_array[count];
		sum += (double)env.stats.interval_array[count];
		count++;
	}
	sum /= (env.stats.count - env.stats.error);
	sqrt = sum / 2;
	while (sqrt != comp)
	{
		comp = sqrt;
		sqrt = (sum/comp + comp) / 2;
	}
	return (sqrt);
}

void	set_stats(void)
{
	env.interval = (float)(env.ts_after - env.ts_before);
	env.stats.count++;
	if (env.interval < env.stats.min || env.stats.min == 0)
		env.stats.min = env.interval;
	if (env.interval > env.stats.max || env.stats.min == 0)
		env.stats.max = env.interval;
	env.stats.sum += env.interval;
	if (env.stats.count  > env.stats.alloc_interval)
		reserve_interval_array(25);
	env.stats.interval_array[env.stats.count - 1] = env.interval;
}
