#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h>	
#include<stdlib.h>	
#include<string.h>	
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>	
#include<netinet/ip.h>	
#include<netinet/if_ether.h>	
#include<net/ethernet.h>	
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>


void printIP(unsigned char* , int);
void printICMP(unsigned char* , int );



struct sockaddr_in source,dest;


int main()
{
	int saddr_size , data_size;
	struct sockaddr saddr;		
	unsigned char *buffer = (unsigned char *) malloc(65536);
	int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
	
	if(sock_raw < 0)
	{
		perror("Socket Error");
		return 1;
	}
	
	while(1)
	{
		saddr_size = sizeof saddr;
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);//receive packet
		if(data_size <0 )
		{
			printf("Recvfrom error , failed to get packets\n");
			return 1;
		}

		printICMP(buffer , data_size);//print packet details
	}
	close(sock_raw);
	return 0;
}




void printIP(unsigned char* Buffer, int Size)
{  
	unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );//iterator to ip header
	iphdrlen =iph->ihl*4;
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;
	printf("\n");
	printf("IP_SRC: %s\n",inet_ntoa(source.sin_addr));
	printf("IP_DST: %s\n",inet_ntoa(dest.sin_addr));
}


void printICMP(unsigned char* Buffer , int Size)
{
	unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
	iphdrlen = iph->ihl * 4;
	struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));//iterator to icmp header
	int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
	printIP(Buffer , Size);
	printf("TYPE: %d\n",(unsigned int)(icmph->type));	
	printf("CODE: %d\n",(unsigned int)(icmph->code));
	printf("---------------- NEXT PACKET -------------------");

	

}


