#include "../inc/ping.h"

/*
 * Get time is second
*/
long		get_ts_s(void)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_sec);
}

/*
 * Get time is millisecond
*/
long double	get_ts_ms(void)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	return (((long double)tv.tv_sec * 1000) + ((long double)tv.tv_usec / 1000));
}

/*
 * Set alarm signal before each ping
 * env.timeout is set to 0 and alarm hit
 * Set before ping TS
 * if first ping then set start TS as well
*/
void	set_timeout_and_ts(void)
{
	alarm(1);
	env.timeout = 1;
	env.ts_before = get_ts_ms();
	if (env.stats.count == 0)
		env.ts_start = env.ts_before;
}
