#include <stdint.h>
#include <stdio.h>
#include "ip.h"
#include <CUnit/Basic.h>


void test_ip_version()
{
	CU_ASSERT(ip_version("124.1.0.15") == 4);
	CU_ASSERT(ip_version("aaaa:f::10") == 6);
}

void test_str_to_ipv4()
{
	char *str1 = "255.255.255.255";
	IPv4 ip1 = (IPv4)4294967295;
	
	char *str2 = "0.0.0.0";
	IPv4 ip2 = (IPv4)0;
	
	char *str3 = "192.168.0.1";
	IPv4 ip3 = (IPv4)3232235521;
	
	CU_ASSERT(str_to_ipv4(str1) == ip1);
	CU_ASSERT(str_to_ipv4(str2) == ip2);
	CU_ASSERT(str_to_ipv4(str3) == ip3);
}
void test_str_to_ipv6()
{
	char *str1 = "a::";
	IPv6 ip1 = { 0, (uint64_t)2814749767106560};
	IPv6 ip1_t = str_to_ipv6(str1);
	char *str2 = "a::b";
	IPv6 ip2 = { 11, (uint64_t)2814749767106560};
	IPv6 ip2_t = str_to_ipv6(str2);
	char *str3 = "::a";
	IPv6 ip3 = { 10, 0};
	IPv6 ip3_t = str_to_ipv6(str3);
	char *str4 = "affa:3422:2344:90ff:0:4:ffff:a";
	IPv6 ip4 = { (uint64_t)21474770954, (uint64_t)12680505022040281343};
	IPv6 ip4_t = str_to_ipv6(str4);
	
	CU_ASSERT((((uint64_t*)&ip1_t)[0] == ((uint64_t*)&ip1)[0]) && (((uint64_t*)&ip1_t)[1] == ((uint64_t*)&ip1)[1]));
	CU_ASSERT((((uint64_t*)&ip2_t)[0] == ((uint64_t*)&ip2)[0]) && (((uint64_t*)&ip2_t)[1] == ((uint64_t*)&ip2)[1]));
	CU_ASSERT((((uint64_t*)&ip3_t)[0] == ((uint64_t*)&ip3)[0]) && (((uint64_t*)&ip3_t)[1] == ((uint64_t*)&ip3)[1]));
	CU_ASSERT((((uint64_t*)&ip4_t)[0] == ((uint64_t*)&ip4)[0]) && (((uint64_t*)&ip4_t)[1] == ((uint64_t*)&ip4)[1]));
}

void test_ipv4_cmp()
{

	IPv4 ip1 = str_to_ipv4("255.255.255.255");
	IPv4 ip2 = str_to_ipv4("60.252.12.249");

	CU_ASSERT(ipv4_cmp(ip1, ip2) == 1);
	CU_ASSERT(ipv4_cmp(ip2, ip1) == -1);
	CU_ASSERT(ipv4_cmp(ip1, ip1) == 0);
}

void test_ipv4_company_node_cmp_by_ip()
{
	IPv4_company_node ip1 = { str_to_ipv4("192.128.0.0"), 10, "AAAA", 0};
	IPv4_company_node ip2 = { str_to_ipv4("192.128.0.0"), 11, "BBBB", 0};
	IPv4_company_node ip3 = { str_to_ipv4("192.192.0.0"), 10, "CCCCCCCC", 0};

	CU_ASSERT(ipv4_company_node_cmp_by_ip((const void *)&ip1, (const void *)&ip2) == -1);
	CU_ASSERT(ipv4_company_node_cmp_by_ip((const void *)&ip2, (const void *)&ip1) == 1);
	CU_ASSERT(ipv4_company_node_cmp_by_ip((const void *)&ip1, (const void *)&ip3) == -1);
	CU_ASSERT(ipv4_company_node_cmp_by_ip((const void *)&ip3, (const void *)&ip1) == 1);
}

void test_ipv4_company_node_cmp_by_company()
{
	IPv4_company_node ip1 = { str_to_ipv4("192.128.0.0"), 10, "AAAAAAAAA", 0};
	IPv4_company_node ip2 = { str_to_ipv4("192.192.0.0"), 10, "CCCCCCCC", 0};

	CU_ASSERT(ipv4_company_node_cmp_by_company((const void *)&ip1, (const void *)&ip2) < 0);
	CU_ASSERT(ipv4_company_node_cmp_by_company((const void *)&ip2, (const void *)&ip1) > 0);
	CU_ASSERT(ipv4_company_node_cmp_by_company((const void *)&ip1, (const void *)&ip1) == 0);
}

void test_ipv4_subnet_included()
{
	IPv4_company_node ipv4 = { str_to_ipv4("192.128.0.0"), 10, "AAAA", 0};
	IPv4 ip1 = str_to_ipv4("192.128.0.1");
	IPv4 ip2 = str_to_ipv4("192.0.0.1");
	IPv4 ip3 = str_to_ipv4("255.255.255.255");
	CU_ASSERT(ipv4_subnet_included(ipv4, ip1) == 0);
	CU_ASSERT(ipv4_subnet_included(ipv4, ip2) == -1);
	CU_ASSERT(ipv4_subnet_included(ipv4, ip3) == 1);
}

void test_ipv6_cmp()
{
	IPv6 ip1 = str_to_ipv6("a::");
	IPv6 ip2 = str_to_ipv6("::a");

	CU_ASSERT(ipv6_cmp(ip1, ip2) == -1);
	CU_ASSERT(ipv6_cmp(ip2, ip1) == 1);
	CU_ASSERT(ipv6_cmp(ip1, ip1) == 0);
}
void test_ipv6_company_node_cmp_by_ip()
{

	IPv6_company_node ip1 = { {(uint64_t)2814749767106560, 11}, 10, "AAAA", 0};
	IPv6_company_node ip2 = { {(uint64_t)2814749767106560, 11}, 11, "BBBB", 0};
	IPv6_company_node ip3 = { {(uint64_t)12680505022040281343, (uint64_t)21474770954}, 10, "CCCCCCCC", 0};

	CU_ASSERT(ipv6_company_node_cmp_by_ip((const void *)&ip1, (const void *)&ip2) == -1);
	CU_ASSERT(ipv6_company_node_cmp_by_ip((const void *)&ip2, (const void *)&ip1) == 1);
	CU_ASSERT(ipv6_company_node_cmp_by_ip((const void *)&ip1, (const void *)&ip3) == -1);
	CU_ASSERT(ipv6_company_node_cmp_by_ip((const void *)&ip3, (const void *)&ip1) == 1);
}
void test_ipv6_company_node_cmp_by_company()
{
	IPv6_company_node ip1 = { {(uint64_t)2814749767106560, 11}, 10, "AAAA", 0};
	IPv6_company_node ip2 = { {(uint64_t)2814749767106560, 11}, 11, "CCCCCCCC", 0};

	CU_ASSERT(ipv6_company_node_cmp_by_company((const void *)&ip1, (const void *)&ip2) < 0 );
	CU_ASSERT(ipv6_company_node_cmp_by_company((const void *)&ip2, (const void *)&ip1) > 0);
	CU_ASSERT(ipv6_company_node_cmp_by_company((const void *)&ip1, (const void *)&ip1) == 0);
}
void test_ipv6_subnet_included()
{
	IPv6_company_node ipv6 = { {(uint64_t)2814749767106560, 0}, 64, "AAAA", 0};
	IPv6 ip1 = {(uint64_t)2814749767106560, 10};
	IPv6 ip2 = {(uint64_t)3096224743817216, 10};
	IPv6 ip3 = {(uint64_t)281474976710656, 10};

	CU_ASSERT(ipv6_subnet_included(ipv6, ip1) == 0);
	CU_ASSERT(ipv6_subnet_included(ipv6, ip2) == 1);
	CU_ASSERT(ipv6_subnet_included(ipv6, ip3) == -1);
}

void test_find_ipv4_subnet()
{
	IPv4_company_node ipv4_1 = { str_to_ipv4("192.128.0.0"), 9, "AAAA", 0};
	IPv4_company_node ipv4_2 = { str_to_ipv4("192.128.0.0"), 10, "AAAA", 0};
	IPv4_company_node ipv4_3 = { str_to_ipv4("192.128.0.0"), 11, "AAAA", 0};
	IPv4_company_node ipv4[] = {ipv4_1, ipv4_2, ipv4_3};
	IPv4 ip1 = str_to_ipv4("192.192.0.1");
	IPv4 ip2 = str_to_ipv4("192.160.0.1");
	IPv4 ip3 = str_to_ipv4("192.144.0.1");
	IPv4 ip4 = str_to_ipv4("140.192.0.1");
	CU_ASSERT(find_ipv4_subnet(ipv4, ip1, 3) == 0);
	CU_ASSERT(find_ipv4_subnet(ipv4, ip2, 3) == 1);
	CU_ASSERT(find_ipv4_subnet(ipv4, ip3, 3) == 2);
	CU_ASSERT(find_ipv4_subnet(ipv4, ip4, 3) == -1);
}

void test_find_ipv6_subnet()
{
	IPv6_company_node ipv6_1 = { {(uint64_t)2814749767106560, 0}, 64, "AAAA", 0};
	IPv6_company_node ipv6_2 = { {(uint64_t)2814749767106560, 0}, 65, "AAAA", 0};
	IPv6_company_node ipv6_3 = { {(uint64_t)2814749767106560, 0}, 66, "AAAA", 0};
	IPv6_company_node ipv6[] = {ipv6_1, ipv6_2, ipv6_3};

	IPv6 ip1 = {(uint64_t)2814749767106560, (uint64_t)18446744073709551616};
	IPv6 ip2 = {(uint64_t)2814749767106560, (uint64_t)9223372036854775808};
	IPv6 ip3 = {(uint64_t)2814749767106560, (uint64_t)4611686018427387904};
	IPv6 ip4 = {10, (uint64_t)4611686018427387904};

	CU_ASSERT(find_ipv6_subnet(ipv6, ip1, 3) == 0);
	CU_ASSERT(find_ipv6_subnet(ipv6, ip2, 3) == 1);
	CU_ASSERT(find_ipv6_subnet(ipv6, ip3, 3) == 2);
	CU_ASSERT(find_ipv6_subnet(ipv6, ip4, 3) == -1);
}

void test_str_count_to_uint()
{
	CU_ASSERT(str_count_to_uint("131315") == 131315);
	CU_ASSERT(str_count_to_uint("18446744073709551616") == (uint64_t)18446744073709551616);
}

int init_suite() { return 0; }
int clean_suite() { return 0; }

int main()
{
	CU_pSuite ipv4Suite, ipv6Suite = NULL;

	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	// Add suite1 to registry 
	ipv4Suite = CU_add_suite("IPv4_Test_Suite1", init_suite, clean_suite);
	if (NULL == ipv4Suite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	// add test1 to suite1 
	if ((NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_ip_version function……..\n\n", test_ip_version)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_str_count_to_uint function……..\n\n", test_str_count_to_uint)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_str_to_ipv4 function……..\n\n", test_str_to_ipv4)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_ipv4_cmp function……..\n\n", test_ipv4_cmp)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_ipv4_company_node_cmp_by_ip function……..\n\n", test_ipv4_company_node_cmp_by_ip)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_ipv4_company_node_cmp_by_company function……..\n\n", test_ipv4_company_node_cmp_by_company)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_ipv4_subnet_included function……..\n\n", test_ipv4_subnet_included)) ||
		(NULL == CU_add_test(ipv4Suite, "\n\n……… Testing test_find_ipv4_subnet function……..\n\n", test_find_ipv4_subnet)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add suite1 to registry 
	ipv6Suite = CU_add_suite("IPv6_Test_Suite1", init_suite, clean_suite);
	if (NULL == ipv6Suite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	// add test1 to suite1 
	if ((NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_ip_version function……..\n\n", test_ip_version)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_str_to_ipv6 function……..\n\n", test_str_to_ipv6)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_ipv6_cmp function……..\n\n", test_ipv6_cmp)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_ipv6_company_node_cmp_by_ip function……..\n\n", test_ipv6_company_node_cmp_by_ip)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_ipv6_company_node_cmp_by_company function……..\n\n", test_ipv6_company_node_cmp_by_company)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_ipv6_subnet_included function……..\n\n", test_ipv6_subnet_included)) ||
		(NULL == CU_add_test(ipv6Suite, "\n\n……… Testing test_find_ipv6_subnet function……..\n\n", test_find_ipv6_subnet)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_run_tests();// OUTPUT to the screen
	CU_cleanup_registry();//Cleaning the Registry
	return CU_get_error();
}