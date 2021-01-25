#include "../inc/ft_ping.h"

void error_exit(char *error_msg)
{
    fprintf(stderr, "ft_ping: %s\n", error_msg);
    clear_ressources();
    exit(1);
}

void getaddr_error(char *target, int error_code)
{
    char *error;

    error = NULL;
    if (error_code == EAI_NONAME)
        error = "NAME or SERVICE is unknown";
    else if (error_code == EAI_AGAIN)
        error = "Temporary failure in name resolution";
    else if (error_code == EAI_FAIL)
        error = "Non-recoverable failure in name res";
    else if (error_code == EAI_MEMORY)
        error = "Memory allocation failure";
    else if (error_code == EAI_SYSTEM)
        error = "System error";
    else
        error = "Error in adress resolution";
    sprintf(&env.args.error_msg[0], "%s: %s", target, error);
    error_exit(&env.args.error_msg[0]);
}