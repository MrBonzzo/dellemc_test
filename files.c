#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip.h"
#include "files.h"

void read_customers_file(char *name, IPv4_company_node **ipv4, IPv6_company_node **ipv6, unsigned *ipv4_len, unsigned *ipv6_len)
{
    FILE *fp = fopen(name, "r");
    if (fp == NULL) { exit(1); }
    *ipv4 = (IPv4_company_node*)malloc(sizeof(IPv4_company_node)*100);
    *ipv6 = (IPv6_company_node*)malloc(sizeof(IPv6_company_node)*100);
    if ((*ipv4 == NULL) || (*ipv6 == NULL))
    {
        fclose(fp);
        exit(2);
    }
    char buff[145];
    while (1)
    {
        char *c = fgets(buff, sizeof(buff), fp);
        if ((c == NULL) && (feof(fp) != 0)) { break; }
        short ip_vers = ip_version(buff);
        if (ip_vers == 4)
        { 
        	add_to_ipv4_arr(ipv4, buff, ipv4_len);
        }
        else if (ip_vers == 6) { add_to_ipv6_arr(ipv6, buff, ipv6_len); }
    }
    fclose(fp);
}

uint64_t read_log_file(char *name, IPv4_company_node *ipv4, IPv6_company_node *ipv6, unsigned ipv4_len, unsigned ipv6_len)
{
    FILE *fp = fopen(name, "r");
    if (fp == NULL) { exit(1); }
    sort_ipv4_by_ip(ipv4, ipv4_len);
    sort_ipv6_by_ip(ipv6, ipv6_len);
    char buff[145];
    uint64_t unknown_bytes = 0;
    while (1)
    {
    	char *c = fgets(buff, sizeof(buff), fp);
        if ((c == NULL) && (feof(fp) != 0)) { break; }
        short ip_vers = ip_version(buff);
        if (ip_vers == 4) { recount_ipv4_node(ipv4, buff, ipv4_len, &unknown_bytes); }
        else if (ip_vers == 6) { recount_ipv6_node(ipv6, buff, ipv6_len, &unknown_bytes); }
    }
    fclose(fp);
    return unknown_bytes;
}

void write_to_file(FILE *fp, char *company_ID, uint64_t count)
{
    fprintf(fp, "%s %ju\n", company_ID, count);
}

void create_report(char *name, IPv4_company_node *ipv4, IPv6_company_node *ipv6, unsigned ipv4_len, unsigned ipv6_len, uint64_t unknown_bytes)
{
    FILE *fp = fopen(name, "w");
    if (fp == NULL)
    {
        exit(1);
    }
    sort_ipv4_by_company_id(ipv4, ipv4_len);
    sort_ipv6_by_company_id(ipv6, ipv6_len);
    for(unsigned i_ipv4 = 0, i_ipv6 = 0; (i_ipv4 < ipv4_len) || (i_ipv6 < ipv6_len);)
    {
        if (i_ipv4 >= ipv4_len)
        {
            uint64_t count = ipv6[i_ipv6].bytes;
            while((i_ipv6 + 1 < ipv6_len) && (strcmp(ipv6[i_ipv6].company_ID, ipv6[i_ipv6 + 1].company_ID)) == 0)
            {
                i_ipv6++;
                count += ipv6[i_ipv6].bytes;
            }
            write_to_file(fp, ipv6[i_ipv6].company_ID, count);
            i_ipv6++;
        }
        else if (i_ipv6 >= ipv6_len)
        {
            uint64_t count = ipv4[i_ipv4].bytes;
            while((i_ipv4 + 1 < ipv4_len) && (strcmp(ipv4[i_ipv4].company_ID, ipv4[i_ipv4 + 1].company_ID)) == 0)
            {
                i_ipv4++;
                count += ipv4[i_ipv4].bytes;
            }
            write_to_file(fp, ipv4[i_ipv4].company_ID, count);
            i_ipv4++;
        }
        else
        {
            int cmp = strcmp(ipv4[i_ipv4].company_ID, ipv6[i_ipv6].company_ID);
            if (cmp < 0)
            {
                uint64_t count = ipv4[i_ipv4].bytes;
                while((i_ipv4 + 1 < ipv4_len) && (strcmp(ipv4[i_ipv4].company_ID, ipv4[i_ipv4 + 1].company_ID)) == 0)
                {
                    i_ipv4++;
                    count += ipv4[i_ipv4].bytes;
                }
                write_to_file(fp, ipv4[i_ipv4].company_ID, count);
                i_ipv4++;
            }
            else if (cmp > 0)
            {
                uint64_t count = ipv6[i_ipv6].bytes;
                while((i_ipv6 + 1 < ipv6_len) && (strcmp(ipv6[i_ipv6].company_ID, ipv6[i_ipv6 + 1].company_ID)) == 0)
                {
                    i_ipv6++;
                    count += ipv6[i_ipv6].bytes;
                }
                write_to_file(fp, ipv6[i_ipv6].company_ID, count);
                i_ipv6++;
            }
            else
            {
                uint64_t count = ipv4[i_ipv4].bytes + ipv6[i_ipv6].bytes;
                while((i_ipv4 + 1 < ipv4_len) && (strcmp(ipv4[i_ipv4].company_ID, ipv4[i_ipv4 + 1].company_ID)) == 0)
                {
                    i_ipv4++;
                    count += ipv4[i_ipv4].bytes;
                }
                while((i_ipv6 + 1 < ipv6_len) && (strcmp(ipv6[i_ipv6].company_ID, ipv6[i_ipv6 + 1].company_ID)) == 0)
                {
                    i_ipv6++;
                    count += ipv6[i_ipv6].bytes;
                }
                write_to_file(fp, ipv4[i_ipv4].company_ID, count);
                i_ipv4++;
                i_ipv6++;
            }
        }
    }
    write_to_file(fp, "UNKNOWN", unknown_bytes);
    fclose(fp);
    free(ipv4);
    free(ipv6);
}