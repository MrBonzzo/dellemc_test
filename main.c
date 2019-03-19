#include <stdio.h>
#include <stdint.h>
#include "ip.h"
#include "files.h"

int main()
{
    char cst_file[] = "data/customers.txt";
    char log_file[] = "data/log.txt";
    char dst_file[] = "data/report.txt";
    IPv4_company_node *ipv4_company_array;
    IPv6_company_node *ipv6_company_array;
    unsigned ipv4_len = 0;
    unsigned ipv6_len = 0;
    read_customers_file(cst_file, &ipv4_company_array, &ipv6_company_array, &ipv4_len, &ipv6_len);
    uint64_t unknown_bytes = 0;
    unknown_bytes = read_log_file(log_file, ipv4_company_array, ipv6_company_array, ipv4_len, ipv6_len);
    create_report(dst_file, ipv4_company_array, ipv6_company_array, ipv4_len, ipv6_len, unknown_bytes);
    return 0;
}