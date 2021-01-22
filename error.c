#include "ft_ping.h"

void error_exit(char *error_msg)
{
    fprintf(stderr, "ft_ping: %s\n", error_msg);
    exit(1);
}