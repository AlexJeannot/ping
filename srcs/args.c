#include "../inc/ft_ping.h"

int parse_ttl(char *current_arg, char *next_arg)
{
    int count;
    char* arg;

    count = 0;
    arg = (current_arg[count]) ? current_arg : next_arg;
    if (arg[count])
    {
        while (arg[count])
        {
            if (!(ft_isdigit(arg[count])))
            {
                sprintf(&(env.args.error_msg[0]), "invalid TTL: '%s'", &(arg[count]));
                error_exit(&(env.args.error_msg[0]));
            }
            count++;
        }
    }
    else
        error_exit("option requires an argument -- t");
    if ((env.ttl = atoi(arg)) < 1 || (env.ttl = atoi(arg)) > 255)
        error_exit("invalid TTL: invalid value");
    env.args.ttl = 1;
    return (current_arg[0] ? 0 : 1);
}

int parse_counter(char *current_arg, char *next_arg)
{
    int count;
    char* arg;

    count = 0;
    arg = (current_arg[count]) ? current_arg : next_arg;
    if (arg[count])
    {
        while (arg[count])
        {
            if (!(ft_isdigit(arg[count])))
            {
                sprintf(&(env.args.error_msg[0]), "invalid counter: '%s'", &(arg[count]));
                error_exit(&(env.args.error_msg[0]));
            }
            count++;
        }
    }
    else
        error_exit("option requires an argument -- c");
    if ((env.args.counter = atoi(arg)) < 1)
        error_exit("invalid counter: must be > 0");
    return (current_arg[0] ? 0 : 1);
}

int parse_options(char *option, char* next_arg)
{
    int count;

    count = 0;
    while (option[count])
    {
        if (option[count] == 'v')
            env.args.verbose = 1;
        else if (option[count] == 'h')
            display_help(0);
        else if (option[count] == 't' || option[count] == 'c' || option[count] == 'i')
        {
            if (option[count] == 't')
                count += parse_ttl(&(option[count + 1]), next_arg);
            else if (option[count] == 'c')
                count += parse_counter(&(option[count + 1]), next_arg);
            break;
        }
        else
            display_wrong_option(option[count]);
        count++;
    }
    return (count);
}

void parse_args(int argc, char **argv)
{
    int pos;

    pos = 1;
    if (argc < 2)
        display_help(1);
    while (pos < argc)
    {
        if (argv[pos][0] == '-' && argv[pos][1])
            pos += parse_options(&(argv[pos][1]), argv[pos + 1]);
        else
        {
            if (env.args.hostname)
                error_exit("Multiple targets");
            env.args.hostname = argv[pos];
        }
        pos++;
    }
}