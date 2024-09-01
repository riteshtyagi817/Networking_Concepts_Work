/*
 * Compile: gcc ip_maths.c -o ip_maths -lm
 * Run: ./ip_maths
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>

#define PREFIX_LEN 15      /* Max IPv4 len in A.B.C.D format */
#define MAX_MASK_LEN 32    /* MASK Maximum value */
#define COMPLEMENT(num) (num = num ^ 0xFFFFFFFF)
#define SET_BIT(val, pos) (val = val | (1 << pos))

unsigned int get_network_mask(char mask)
{
	unsigned int network_mask = 0x00000000;
	unsigned char pos = 31;
	while (mask) {
		SET_BIT(network_mask, pos);
		--mask, --pos;
	}
	return network_mask;
}

void get_abcd_ip_format(unsigned int ip_addr, char *output_buffer)
{
	sprintf(output_buffer, "%d.%d.%d.%d",
		((ip_addr & 0xFF000000) >> 24),
		((ip_addr & 0xFF0000) >> 16),
		((ip_addr & 0xFF00) >> 8),
		(ip_addr & 0xFF));
}
unsigned int get_ip_integer_equivalent(char *address)
{
	unsigned char ipbytes[4];

	sscanf(address, "%hhu.%hhu.%hhu.%hhu", ipbytes + 3,
	       ipbytes + 2,
	       ipbytes + 1,
	       ipbytes);

	return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}

void get_network_id(char *ip_addr, char mask, char *output_buffer)
{
	unsigned int network_mask = get_network_mask(mask);
	unsigned int network_ip = get_ip_integer_equivalent(ip_addr);
	unsigned int network_id = network_ip & network_mask;

	get_abcd_ip_format(network_id, output_buffer);
}

void get_broadcast_address(char *ip_addr, char mask, char *output_buffer)
{
	unsigned int network_mask = get_network_mask(mask);
	unsigned int network_ip = get_ip_integer_equivalent(ip_addr);
	unsigned int network_id = network_ip & network_mask;
	unsigned int broadcast_addr = network_id | COMPLEMENT(network_mask);

	get_abcd_ip_format(broadcast_addr, output_buffer);
}

unsigned int get_subnet_cardinality(char mask)
{
	return (pow(2, MAX_MASK_LEN - mask) - 2);
}

/*
 * Return 0; if true else return -1
 */
int check_ip_subnet_membership(char *network_id, char mask, char *check_ip)
{
	unsigned int nw_ip = get_ip_integer_equivalent(network_id);
	unsigned int ip = get_ip_integer_equivalent(check_ip);
	unsigned network_mask = get_network_mask(mask);
	unsigned int chk_ip_network = ip & network_mask;

	if (chk_ip_network == nw_ip)
		return 0;
	else
		return -1;
}

int main(void)
{

	/* Test Condition for the excercies */
	/*Q1. Get broadcast address */
	{
		char output_buffer[PREFIX_LEN + 1] = {0};
		get_broadcast_address("192.168.1.2", 24, output_buffer);
		if (strcmp("192.168.1.255", output_buffer)) {
			error(-1, 0, "Broadcast address test1 failed");
		}

		get_broadcast_address("10.1.23.10", 20, output_buffer);
		if (strcmp("10.1.31.255", output_buffer)) {
			error(-1, 0, "Broadcast address test2 failed");
		}
		printf("Broadcast address Test Passed\n");
	}

	/*Q2. Get IP integer equivalent */
	{
		unsigned int ip = get_ip_integer_equivalent("192.168.1.2");
		if (!(ip == 0xC0A80102)) {
			error(-1, 0, "Integer equivalent test1 failed");
		}

		ip = get_ip_integer_equivalent("10.11.12.13");
		if (!(ip == 0x0A0B0C0D)) {
			error(-1, 0, "Integer equivalent test1 failed");
		}
		printf("Get IP Integer equivalent test Passed\n");
	}

	/*Q3. Get A.B.C.D IP Format */
	{
		char output_buffer[PREFIX_LEN + 1] = {0};
		get_abcd_ip_format(0xC0A80102, output_buffer);
		if (strcmp(output_buffer, "192.168.1.2")) {
			error(-1, 0, "Get A.B.C.D IP format test1 failed");
		}

		get_abcd_ip_format(0x0A0B0C0D, output_buffer);
		if (strcmp(output_buffer, "10.11.12.13")) {
			error(-1, 0, "Get A.B.C.D IP format test2 failed");
		}
		printf("Get A.B.C.D ip format test passed\n");
	}

	/* Q4. Get network ID */
	{
		char output_buffer[PREFIX_LEN + 1] = {0};
		get_network_id("192.168.1.2", 24, output_buffer);
		if (strcmp(output_buffer, "192.168.1.0")) {
			error(-1, 0, "Get Network_ID test1 failed");
		}

		get_network_id("10.11.12.13", 16, output_buffer);
		if (strcmp(output_buffer, "10.11.0.0")) {
			error(-1, 0, "Get Network_ID test2 failed");
		}
		printf("Get Network_ID Test passed\n");
	}

	/* Q5. Get subnet cardinality */
	{
		unsigned int cardinality = get_subnet_cardinality(24);
		if (!(cardinality == 254)) {
			error(-1, 0, "Subnet Cardinality Test1 failed");
		}

		cardinality = get_subnet_cardinality(30);
		if (!(cardinality == 2)) {
			error(-1, 0, "Subnet Cardinality Test2 failed");
		}
		printf("Subnet cardinality test passed\n");
	}

	/* Q6. Check IP Subnet Membership */
	{
		if (check_ip_subnet_membership("192.168.1.0", 24, "192.168.1.10")) {
			error(-1, 0, "IP Subnet Membership test1 failed");
		}

		if (check_ip_subnet_membership("172.1.0.0", 16, "172.1.1.10")) {
			error(-1, 0, "IP Subnet Membership test1 failed");
		}

		printf("Check ip subnet membership test passed\n");
	}

	return 0;
}
