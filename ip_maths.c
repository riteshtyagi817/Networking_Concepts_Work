#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#define PREFIX_LEN 16

// although it would be a longer program but just for practice doing it in a single file

int convert_into_integer(char input_str[], int oct_num,char dot_pos[])
{
	// need to implement so that we can get octet correctly even if 1  , 2 or 3 digits in single octet

			
	int start = 0;
	int end = 0;
	switch(oct_num)
	{
		case 1:
		start = 0;
		end = dot_pos[0]-'0'-1;
		break;
		case 2:
		start = dot_pos[0] - '0' +1;
		end = dot_pos[1] - '0' -1;
		break;
		case 3:
		start = dot_pos[1] - '0' + 1;
		end = dot_pos[2] - '0'-1;
		break;
		case 4:
		start = dot_pos[2] - '0' + 1;
		end = 14;
		break;
		default:
		start = 0;
		end = 0;

	}
	char temp[4];
	memset(temp,0, 4);
	uint8_t temp_idx = 0;
	for(int i = start; i <= end;i++)
	{
		if(input_str[i] != '.')
		temp[temp_idx++] = input_str[i];
		

	}
	return atoi(temp);


}

int get_converted_or_value(uint8_t mask)
{
	int value = 32 - mask;
	int temp = 1;
	while(value--)
	{
		temp = temp << 1;
		

	}
	return temp-1;


}
unsigned int octet_value_from_right(unsigned int input, int num)
{
	unsigned int mask = 0;
	if(num == 1)
		mask  = 0x000000FF;
	else if(num == 2)
		mask = 0x0000FF00;
	else if(num == 3) 
		mask = 0x00FF0000;
	else if(num == 4)
		mask = 0xFF000000;
	
	unsigned int temp = (mask&input);
	if(num == 4) temp  = temp >> 24;	
        else if(num == 3) temp = temp >> 16;
	else if(num == 2) temp = temp >> 8;
	
	return temp;




}
void find_decimal_pos(char input_ip[], char out[])
{
	int index = 0;
	for(int i = 0;i < PREFIX_LEN;i++)
	{
		if(input_ip[i] == '.')
		{
			out[index++] = i + '0';
		}
	} 
	
	return;
} 
void create_int_from_octets(unsigned int *out, uint8_t octet)
{
	unsigned int temp = octet;
	//printf("%u\n",octet);
	(*out) = ((*out) << 8);	
	(*out) = (*out)|temp;
	return;
		

}
void get_broadcast_address(char input_ip[], uint8_t mask, char brd_addr[])
{
	// firstly taking each octet value from input ip ( all 32 bits into one integer value )
	// so to achieve that first we need to have funtion to get the octet value into one 1 byte integer
	char dot_pos_store[4];
	memset(dot_pos_store,0,4);
	find_decimal_pos(input_ip, dot_pos_store);
	uint8_t value = 0;
        uint8_t first_octet = convert_into_integer(input_ip,1,dot_pos_store);
	uint8_t second_octet = convert_into_integer(input_ip,2,dot_pos_store);
	uint8_t third_octet = convert_into_integer(input_ip,3, dot_pos_store);
	uint8_t fourth_octet = convert_into_integer(input_ip,4,dot_pos_store);

	#ifdef DEBUG
	printf("first octet is %u\n", first_octet);
	printf("second octet is %u\n", second_octet);
	printf("third octet is %u\n", third_octet);
	printf("fourth octet is %u\n", fourth_octet);
    	#endif 
	unsigned int out = first_octet;
	create_int_from_octets(&out, first_octet);		
	create_int_from_octets(&out, second_octet);		
	create_int_from_octets(&out, third_octet);		
	create_int_from_octets(&out, fourth_octet);

	//getchar();
	/* getchar();
	int oct_num = 0;
	printf("Enter the octet no you want \n");
	scanf("%d",&oct_num);
	printf("octet value %u\n",octet_value_from_right(out,oct_num));
	*/

	unsigned int maskValue = 0xFFFFFFFF;
	printf("mask %d\n", mask);
	while(mask--)
	{

		maskValue >>= 1;
		
	}
	unsigned int finalValue = (maskValue | out);

	//printf("printing final_value %u\n",finalValue);

	int idx = 0;
	int num = 4;
	while(num > 0)
	{
		unsigned int value = octet_value_from_right(finalValue, num);
		printf("Octet: %d from right is %d\n",num, value);
		int start = idx;
		while(value)
		{
			brd_addr[idx++] = (value%10)+'0';
			value /= 10;
		}
		int end = idx-1;
		while(start < end)
		{
			char tmp = brd_addr[start];
			brd_addr[start] = brd_addr[end];
			brd_addr[end] = tmp;
			start++;
			end--;
		}
		brd_addr[idx++] = '.';
		num--;
	}
	idx--;
	brd_addr[idx] = '\0';
	
   
	return;
}


int main()
{
	// usage of funtion get_broadcast_address

	// Output and input arrays to store the output broadcast address and input ip address
	char brd_addr[PREFIX_LEN];
	char input_ip[PREFIX_LEN];

	// output and input arrays initialization using memset
	memset(brd_addr, '\0', PREFIX_LEN);
	memset(input_ip,0,PREFIX_LEN);

	// taking input from the user
	printf("Enter the ip address in the format A.B.C.D for which you want to display broadcast address\n");
	scanf("%s",input_ip);
	getchar();	
	// taking mask value from the user
	printf("Enter the mask value eg(24)\n");
	uint8_t mask = 0;
	scanf("%d",&mask);

	// calling get_broadcast_address function to get the broadcast address from input
	get_broadcast_address(input_ip, mask, brd_addr);
	

	// printing the broadcast address 
	printf("Broadcast address of input ip %s is %s\n", input_ip, brd_addr);
		
	return 0;
}
