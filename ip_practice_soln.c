#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<arpa/inet.h>
#include<math.h>

#define IS_BIT_SET(n,pos) ((n & (1 << pos)) != 0)
#define TOGGLE_BIT(n,pos) (n = (n ^(1 << pos)))
#define COMPLEMENT(num) (num = (num ^ 0xFFFFFFFF))
#define UNSET_BIT(n,pos) (n = (n & ((1 << pos)^0xFFFFFFFF)))
#define SET_BIT(n,pos) (n =  (n | (1 << pos)))

#define PREFIX_LEN 15
#define MAX_MASK_LEN 32 /* Max Mask value in decimal notation */


unsigned int get_mask_value_in_integer(char mask_value){

	unsigned int mask = 0XFFFFFFFF;
	char n_trail_bits = MAX_MASK_LEN - mask_value;
	int i = 0;
	for(i = 0; i < n_trail_bits;i++){
		UNSET_BIT(mask,i);
	}
	return mask;
}
void get_broadcast_address(char *ip_address, char mask, char *output_buffer){

	unsigned int ip_addr_integer = 0;

	/* convert input ip address from A.B.C.D to equivalent unsigned int format */
	inet_pton(AF_INET, ip_address, &ip_addr_integer);

	/* might need to verify below once */
	ip_addr_integer = htonl(ip_addr_integer);

	unsigned int mask_integer_format = get_mask_value_in_integer(mask);

	//prinf
	COMPLEMENT(mask_integer_format); /* reverse the bits */


	unsigned int broadcast_address = ip_addr_integer | mask_integer_format;

	broadcast_address = htonl(broadcast_address);

	inet_ntop(AF_INET, &broadcast_address, output_buffer, PREFIX_LEN + 1);

	output_buffer[PREFIX_LEN] = '\0';

	return;
}
unsigned int get_ip_integer_equivalent(char *ip_address){

	unsigned int ip_addr_integer = 0;
	inet_pton(AF_INET, ip_address, &ip_addr_integer);
	return htonl(ip_addr_integer);
}
void get_abcd_ip_format(unsigned int ip_address, char *output_buffer){

	inet_ntop(AF_INET, &ip_address, output_buffer, PREFIX_LEN + 1);
	output_buffer[PREFIX_LEN] = '\0';
}
void get_network_id(char *ip_address, char mask, char *output_buffer){
	
	unsigned int mask_integer_format = get_mask_value_in_integer(mask);
	unsigned int ip_address_integer = 0;
	inet_pton(AF_INET, ip_address, &ip_address_integer);
	unsigned int network_id = ip_address_integer & mask_integer_format;
	network_id = htonl(network_id);
	inet_ntop(AF_INET, &network_id, output_buffer, PREFIX_LEN + 1);

}
int main(int argc, char *argv[]){

/* Testing get broadcast address */
{
	printf("Testing Q1..\n");
	char ip_address[PREFIX_LEN + 1],output_buffer[PREFIX_LEN + 1];
	
	memset(ip_address, '\0', sizeof(ip_address));
	memset(output_buffer, '\0', sizeof(output_buffer));

	memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));

	ip_address[strlen(ip_address)] = '\0';

	char mask = 24;

	get_broadcast_address(ip_address, mask , output_buffer);

	printf("Broadcast address = %s\n",output_buffer);

	printf("Testing Q1 done\n");

}
/* get ip_integer equivalent */
{
	printf("Testing Q2..\n");
	char ip_address[PREFIX_LEN + 1];
	memset(ip_address, '\0',sizeof(ip_address));
	memcpy(ip_address, "192.168.2.10",strlen("192.168.2.10"));
	ip_address[strlen(ip_address)] = '\0';
	unsigned int a = get_ip_integer_equivalent(ip_address);
	printf("a = %u\n",a );
	printf("Testing Q2 done.\n");
}
/* Testing get_abcd_ip_format() */
{
	printf("Testing Q3..\n");
	char output_buffer[PREFIX_LEN + 1];
	memset(output_buffer, '\0', sizeof(output_buffer));

	unsigned int a = 2058138165;
	printf("Testing Q3..\n");
	get_abcd_ip_format(htonl(a), output_buffer);
	printf("Ip address in A.B.C.D format = %s\n",output_buffer);
	printf("Testing Q3 done\n");


}
/* Tesing get_network_id() */
{
	printf("Testing Q4..\n");
	char ip_address[PREFIX_LEN + 1], output_buffer[PREFIX_LEN + 1];
	memset(ip_address, '\0', sizeof(ip_address));
	memset(output_buffer, '\0', sizeof(output_buffer));
	memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
	ip_address[strlen(ip_address)] = '\0';

	char mask  = 20;
	get_network_id(ip_address, mask, output_buffer);
	printf("Network id = %s/%u\n",output_buffer, mask);
	printf("Testing Q4 done.\n");

}


	



}
