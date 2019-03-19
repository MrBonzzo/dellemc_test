#ifndef FILES_H
#define FILES_H

void read_customers_file(char *name, IPv4_company_node **ipv4, IPv6_company_node **ipv6, unsigned *ipv4_len, unsigned *ipv6_len);
uint64_t read_log_file(char *name, IPv4_company_node *ipv4, IPv6_company_node *ipv6, unsigned ipv4_len, unsigned ipv6_len);
void write_to_file(FILE *fp, char *company_ID, uint64_t count);
void create_report(char *name, IPv4_company_node *ipv4, IPv6_company_node *ipv6, unsigned ipv4_len, unsigned ipv6_len, uint64_t unknown_bytes);

#endif