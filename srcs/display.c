#include "../inc/ft_ping.h"

void display_help(int exit_code)
{
    printf("Usage: ft_ping [-hv] [-c count] [-t ttl]\n");
    clear_ressources();
    exit(exit_code);
}

void display_wrong_option(char option)
{
    printf("Ping: invalid option -- '%c'\n", option);
    display_help(1);
}

void display_error()
{
    t_ip_header ip_header;
    char *dest_error_code[16] = {"Destination network unreachable", "Destination host unreachable" \
                                "Destination protocol unreachable", "Destination port unreachable" \
                                "Fragmentation required", "Source route failed", "Destination network unknown" \
                                "Destination host unknown", "Source host isolated", "Network administratively prohibited" \
                                "Host administratively prohibited", "Network unreachable for ToS", \
                                "Host unreachable for ToS", "Communication administratively prohibited", \
                                "Host Precedence Violation", "Precedence cutoff in effect"};
    char *ttl_error_code[2] = {"Time To Live exceeded", "Fragment reassembly time exceeded"};
    char *error_msg;

    retrieve_ip_info(&(env.icmp_res.data[0]), &(ip_header));
    display_ip_header_info(ip_header, "ERROR");

    error_msg = (env.icmp_res.type == 3) ? dest_error_code[env.icmp_res.code] : ttl_error_code[env.icmp_res.code];
    printf("From %s icmp_seq=%d %s\n", "TEST", env.icmp_res.seq, error_msg);
}

void display_introduction(void)
{
    char ip_addr[INET_ADDRSTRLEN];

    bzero(&(ip_addr[0]), sizeof(ip_addr));
    inet_ntop(AF_INET, &(((struct sockaddr_in*)env.addr->ai_addr)->sin_addr), &(ip_addr[0]), INET_ADDRSTRLEN);
    printf("PING %s (%s) 56(84) bytes of data.", env.args.hostname, ip_addr);
}

void display_ping(int bytes)
{
    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n", bytes, env.r_data.s_host, env.r_data.s_addr, env.icmp_req.seq, env.r_ttl, env.interval);
    if (env.args.counter && env.stats.count == env.args.counter)
        display_summary();
}

void display_summary(void)
{
    float average;
    int percentage;
    int received;
    int duration;

    average = (float)(env.stats.sum / env.stats.count);
    percentage = env.stats.error / env.stats.count;
    received = env.stats.count - env.stats.error;
    duration = (int)(get_ts_ms() - env.ts_start);
    printf("\n--- %s ping statistics ---\n", env.args.hostname);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %dms\n", env.stats.count, received, percentage, duration);
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", env.stats.min, average, env.stats.max, calcul_stddev(average));
    clear_ressources();
    exit(0);
}