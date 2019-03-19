#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "ip.h"

short ip_version(char *buff)
{
    for (int i = 0; i < 5; ++i)
    {
        if (buff[i] == '.') { return 4; }
        if (buff[i] == ':') { return 6; }
    }
    return 0;
}

IPv4 str_to_ipv4(char *buff)
{
    char *tail = buff;
    IPv4 ip;
    *((uint8_t*)&ip + 3) = (uint8_t)strtol(tail, &tail, 10);
    for (int i = 2; i >= 0; --i)
    {
        *((uint8_t*)&ip + i) = (uint8_t)strtol(tail + 1, &tail, 10);
    }
    return ip;
}

//position type: 0 - full, 1 - beg, 2 - mid, 3 - end;
void get_ipv6_colon_info(char *s, int *count, int *position_number, int *position_type)
{
    *count = 0;
    *position_number = -1;
    *position_type = -1;
    for (int i = 0; s[i] != 0; ++i)
    {
        if(s[i] == ':')
        {
            (*count)++;
            if(s[i + 1] == ':')
            {
                *position_number = *count;
                if (i == 0) { *position_type = 1; }
                else if (s[i + 2] == 0) { *position_type = 3; }
                else { *position_type = 2; }
            }
        }
    }
    if (*count == 7) { *position_type = 0; }
}

IPv6 str_to_ipv6(char *buff)
{
    int count = 0;
    int position_number = 0;
    int position_type = 0;
    char *tail = buff;
    IPv6 ip;
    uint16_t block[8] = { 0 };
    get_ipv6_colon_info(buff, &count, &position_number, &position_type);
    if (position_type == 1)
    {
        tail++;
        for (int i = 9 - count; i < 8; ++i) { block[i] = strtol(tail + 1, &tail, 16); }
    }
    else if (position_type == 2)
    {
        block[0] = strtol(tail, &tail, 16);
        for (int i = 1; i < position_number; ++i) { block[i] = strtol(tail + 1, &tail, 16); }
        tail++;
        for (int i = 9 - count; i < 8; ++i) { block[i] = strtol(tail + 1, &tail, 16); }
    }
    else if (position_type == 3)
    {
        block[0] = strtol(tail, &tail, 16);
        for (int i = 1; i < position_number; ++i) { block[i] = strtol(tail + 1, &tail, 16); }
    }
    else if (position_type == 0)
    {
        block[0] = strtol(tail, &tail, 16);
        for (int i = 1; i < count; ++i) { block[i] = strtol(tail + 1, &tail, 16); }
    }
    for (int i = 7; i >= 0; --i)
    {
        *((uint16_t*)&ip + i) = block[7 - i];
    }
    return ip;
}

void add_to_ipv4_arr(IPv4_company_node **ipv4, char *buff, unsigned *ipv4_len)
{
    char *company_ID = strchr(buff, ' ') + 1;
    char ip_str[15] = { 0 };
    memcpy(ip_str, buff, strchr(buff, '/') - buff);
    short company_ID_len = strlen(company_ID);
    if (company_ID[company_ID_len - 1] == '\n') { company_ID_len--; }
    (*ipv4)[*ipv4_len].ip = str_to_ipv4(ip_str);
    (*ipv4)[*ipv4_len].mask = strtol(strchr(buff, '/') + 1, NULL, 10);
    (*ipv4)[*ipv4_len].company_ID = (char *)malloc(sizeof(char) * company_ID_len);
    if ((*ipv4)[*ipv4_len].company_ID == NULL) { exit(2); }
    memcpy((*ipv4)[*ipv4_len].company_ID, company_ID, company_ID_len);
    (*ipv4)[*ipv4_len].bytes = 0;
    (*ipv4_len)++;
    if (!(*ipv4_len % 100))
    {
        *ipv4 = (IPv4_company_node *)realloc(*ipv4, sizeof(IPv4_company_node) * ((*ipv4_len) + 100));
        if (*ipv4 == NULL) { exit(2); }
    }
}
void add_to_ipv6_arr(IPv6_company_node **ipv6, char *buff, unsigned *ipv6_len)
{
    char *company_ID = strchr(buff, ' ') + 1;
    char ip_str[39];
    memcpy(ip_str, buff, strchr(buff, '/') - buff);
    short company_ID_len = strlen(company_ID);
    if (company_ID[company_ID_len - 1] == '\n') { company_ID_len--; }
    (*ipv6)[*ipv6_len].ip = str_to_ipv6(ip_str);
    (*ipv6)[*ipv6_len].mask = strtol(strchr(buff, '/') + 1, NULL, 10);
    (*ipv6)[*ipv6_len].company_ID = (char *)malloc(sizeof(char) * company_ID_len);
    if ((*ipv6)[*ipv6_len].company_ID == NULL) { exit(2); }
    memcpy((*ipv6)[*ipv6_len].company_ID, company_ID, company_ID_len);
    (*ipv6)[*ipv6_len].bytes = 0;
    
    (*ipv6_len)++;
    if (!(*ipv6_len % 100))
    {
        *ipv6 = (IPv6_company_node *)realloc(*ipv6, sizeof(IPv6_company_node)* ((*ipv6_len) + 100));
        if (*ipv6 == NULL) { exit(2); }
    }
}

int ipv4_cmp(IPv4 a, IPv4 b)
{
    if (a == b) { return 0; }
    else if (a > b) { return 1; }
    else { return -1; }
}

int ipv4_company_node_cmp_by_ip(const void *a, const void *b)
{
    IPv4 ip_a = ((IPv4_company_node*)a)->ip;
    IPv4 ip_b = ((IPv4_company_node*)b)->ip;
    int status  = ipv4_cmp(ip_a, ip_b);
    if (status == 0)
    {
        return (((IPv4_company_node*)a)->mask > ((IPv4_company_node*)b)->mask)? 1: -1;
    }
    else { return status; }
}

int ipv4_company_node_cmp_by_company(const void *a, const void *b)
{
    char *company_a = ((IPv4_company_node*)a)->company_ID;
    char *company_b = ((IPv4_company_node*)b)->company_ID;
    int cmp = strcmp(company_a, company_b);
    return cmp;
}

int ipv4_subnet_included(IPv4_company_node ipv4, IPv4 ip)
{
    uint32_t zero = 0;
    uint32_t mask = ~zero ^ ((~zero) >> ipv4.mask);
    IPv4 mask_ip = mask & ip;
    return ipv4_cmp(mask_ip, ipv4.ip);  
}

int ipv6_cmp(IPv6 a, IPv6 b)
{
    if(*((uint64_t*)&a + 1) == *((uint64_t*)&b + 1))
    {
        if(*((uint64_t*)&a) == *((uint64_t*)&b)) { return 0; }
        else if(*((uint64_t*)&a) > *((uint64_t*)&b)) { return 1; }
        else { return -1; }
    }
    else if(*((uint64_t*)&a + 1) > *((uint64_t*)&b + 1)) { return 1; }
    else { return -1; }
}

int ipv6_company_node_cmp_by_ip(const void *a, const void *b)
{
    IPv6 ip_a = ((IPv6_company_node*)a)->ip;
    IPv6 ip_b = ((IPv6_company_node*)b)->ip;
    int status = ipv6_cmp(ip_a, ip_b);
    if (status == 0) { return (((IPv6_company_node*)a)->mask > ((IPv6_company_node*)b)->mask)? 1: -1; }
    else { return status; }
}

int ipv6_company_node_cmp_by_company(const void *a, const void *b)
{
    char *company_a = ((IPv6_company_node*)a)->company_ID;
    char *company_b = ((IPv6_company_node*)b)->company_ID;
    return strcmp(company_a, company_b);
}

int ipv6_subnet_included(IPv6_company_node ipv6, IPv6 ip)
{
    uint64_t zero = 0;
    uint64_t mask[2] = { 0 };
    if (ipv6.mask < 64)
    {
        mask[1] = ~zero ^ (~zero >> ipv6.mask);
    }
    else
    {
        mask[1] = ~zero;
        mask[0] = ~zero ^ (~zero >> ipv6.mask - 64);
    }
    IPv6 mask_ip;
    *((uint64_t*)&mask_ip + 1) = mask[1] & *((uint64_t*)&ip + 1);
    *((uint64_t*)&mask_ip) = mask[0] & *((uint64_t*)&ip);
    return ipv6_cmp(mask_ip, ipv6.ip);
}

long long find_ipv4_subnet(IPv4_company_node *ipv4, IPv4 ip, unsigned len)
{
    long long left = 0;
    long long right = len - 1;
    while (left <= right)
    { 
        long long mid = left + (right - left) / 2;
        int status = ipv4_subnet_included(ipv4[mid], ip); 
        if (status == 0)
        {
            while(mid + 1 < len)
            {
                if (ipv4_subnet_included(ipv4[mid + 1], ip) == 0) 
                {
                    ++mid;
                }
                else
                {
                    break;
                }
            }
            return mid;
        }
        else if (status > 0) { left = mid + 1; }
        else if (status < 0) { right = mid - 1; }
    }
    return -1;
}

long long find_ipv6_subnet(IPv6_company_node *ipv6, IPv6 ip, unsigned len)
{
    long long left = 0;
    long long right = len - 1;
    while (left <= right)
    { 
        long long mid = left + (right - left) / 2;
        int status = ipv6_subnet_included(ipv6[mid], ip); 
        if (status == 0)
        {
            while(mid + 1 < len)
            {
                if (ipv6_subnet_included(ipv6[mid + 1], ip) == 0) 
                {
                    ++mid;
                }
                else
                {
                    break;
                }
            }
            return mid;
        }
        else if (status > 0) { left = mid + 1; }
        else if (status < 0) { right = mid - 1; }
    }
    return -1;
}

uint64_t str_count_to_uint(char *buff)
{
    uint64_t result = 0;
    for (int i = 0; (buff[i] != '\n') && buff[i] != 0; ++i) { result = result * 10 + buff[i] - '0'; }
    return result; 
}

void recount_ipv4_node(IPv4_company_node *ipv4, char *buff, unsigned len, uint64_t *unknown_bytes)
{
    char ip_str[15];
    char *count_str = strchr(buff, ' ') + 1;
    memcpy(ip_str, buff, strchr(buff, ' ') - buff + 1);
    IPv4 ip = str_to_ipv4(ip_str);
    long long index = find_ipv4_subnet(ipv4, ip, len);
    if (index > -1) { ipv4[index].bytes += str_count_to_uint(count_str); }
    else { *unknown_bytes += str_count_to_uint(count_str); }    
}

void recount_ipv6_node(IPv6_company_node *ipv6, char *buff, unsigned len, uint64_t *unknown_bytes)
{
    char ip_str[39];
    char *count_str = strchr(buff, ' ') + 1;
    memcpy(ip_str, buff, strchr(buff, ' ') - buff);
    IPv6 ip = str_to_ipv6(ip_str);
    long long index = find_ipv6_subnet(ipv6, ip, len);
    if (index > -1) { ipv6[index].bytes += str_count_to_uint(count_str); }
    else { *unknown_bytes += str_count_to_uint(count_str); }    
}

void sort_ipv4_by_ip(IPv4_company_node *ipv4, unsigned len)
{
    qsort(ipv4, len, sizeof(IPv4_company_node), ipv4_company_node_cmp_by_ip);
}

void sort_ipv4_by_company_id(IPv4_company_node *ipv4, unsigned len)
{
    qsort(ipv4, len, sizeof(IPv4_company_node), ipv4_company_node_cmp_by_company);
}

void sort_ipv6_by_ip(IPv6_company_node *ipv6, unsigned len)
{
    qsort(ipv6, len, sizeof(IPv6_company_node), ipv6_company_node_cmp_by_ip);
}

void sort_ipv6_by_company_id(IPv6_company_node *ipv6, unsigned len)
{
    qsort(ipv6, len, sizeof(IPv6_company_node), ipv6_company_node_cmp_by_company);
}