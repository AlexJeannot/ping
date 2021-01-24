#include "ft_ping.h"

void error_exit(char *error_msg)
{
    fprintf(stderr, "ft_ping: %s\n", error_msg);
    clear_ressources();
    exit(1);
}