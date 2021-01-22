#include "ft_ping.h"

void display_help(int exit_code)
{
    printf("HELP TO DO");
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

void display_ping(int bytes)
{
    long double interval;

    interval = (env.ts_after - env.ts_before);
    printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3Lf ms\n", bytes, env.r_data.s_host, env.r_data.s_addr, env.icmp_req.seq, env.r_ttl, interval);
}

void display_summary(void)
{
    printf("\n--- %s ping statistics ---\n", env.args.hostname);
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", 1.1, 2.2, 3.3, 4.4);
    exit(0);
}