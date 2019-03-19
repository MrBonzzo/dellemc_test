#ifndef IP_H
#define IP_H
#include <stdint.h>

typedef uint32_t IPv4;
typedef struct IPv6 { uint64_t chunk[2]; } IPv6;

typedef struct IPv4_company_node
{
    IPv4 ip;
    uint8_t mask;
    char *company_ID;
    uint64_t bytes;
} IPv4_company_node;

typedef struct IPv6_company_node
{
    IPv6 ip;
    uint8_t mask;
    char *company_ID;
    uint64_t bytes;
} IPv6_company_node;

short ip_version(char *buff);

//position type: 0 - full, 1 - beg, 2 - mid, 3 - end;
void get_ipv6_colon_info(char *s, int *count, int *position_number, int *position_type);
IPv4 str_to_ipv4(char *buff);
IPv6 str_to_ipv6(char *buff);
void add_to_ipv4_arr(IPv4_company_node **ipv4, char *buff, unsigned *ipv4_len);
void add_to_ipv6_arr(IPv6_company_node **ipv6, char *buff, unsigned *ipv6_len);

int ipv4_cmp(IPv4 a, IPv4 b);
int ipv4_company_node_cmp_by_ip(const void *a, const void *b);
int ipv4_company_node_cmp_by_company(const void *a, const void *b);
int ipv4_subnet_included(IPv4_company_node ipv4, IPv4 ip);

int ipv6_cmp(IPv6 a, IPv6 b);
int ipv6_company_node_cmp_by_ip(const void *a, const void *b);
int ipv6_company_node_cmp_by_company(const void *a, const void *b);
int ipv6_subnet_included(IPv6_company_node ipv6, IPv6 ip);

long long find_ipv4_subnet(IPv4_company_node *ipv4, IPv4 ip, unsigned len);
long long find_ipv6_subnet(IPv6_company_node *ipv6, IPv6 ip, unsigned len);

uint64_t str_count_to_uint(char *buff);
void recount_ipv4_node(IPv4_company_node *ipv4, char *buff, unsigned len, uint64_t *unknown_bytes);
void recount_ipv6_node(IPv6_company_node *ipv6, char *buff, unsigned len, uint64_t *unknown_bytes);

void sort_ipv4_by_ip(IPv4_company_node *ipv4, unsigned len);
void sort_ipv6_by_ip(IPv6_company_node *ipv6, unsigned len);

void sort_ipv4_by_company_id(IPv4_company_node *ipv4, unsigned len);
void sort_ipv6_by_company_id(IPv6_company_node *ipv6, unsigned len);

#endif