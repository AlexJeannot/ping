#include "ft_ping.h"

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
                ft_strlcpy(&(env.args.error_msg[0]), "invalid TTL: '", 14);
                ft_strlcat(&(env.args.error_msg[0]), &(arg[count]), ft_strlen(&(arg[count])));
                ft_strlcat(&(env.args.error_msg[0]), "'", 1);
                error_exit(&(env.args.error_msg[0]));
            }
            count++;
        }
        env.ttl = atoi(arg);
    }
    env.args.ttl = 1;
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
        else if (option[count] == 't')
        {
            count = parse_ttl(&(option[count + 1]), next_arg);
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
    int count;

    pos = 1;
    count = 0;
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