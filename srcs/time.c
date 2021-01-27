#include "../inc/ping.h"

long		get_ts_s(void)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_sec);
}

long double	get_ts_ms(void)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	return (((long double)tv.tv_sec * 1000) + ((long double)tv.tv_usec / 1000));
}

void	set_timeout_and_ts(void)
{
	alarm(1);
	env.timeout = 1;
	env.ts_before = get_ts_ms();
	if (env.stats.count == 0)
		env.ts_start = env.ts_before;
}
