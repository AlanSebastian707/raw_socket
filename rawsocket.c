#include <stdio.h> //printf,perror
#include <stdlib.h>// malloc,free
#include <unistd.h>//close
#include <string.h>//string manipulation
#include <net/ethernet.h>// for ethhdr
#include <sys/socket.h>//socket,recvfrom
#include <arpa/inet.h>//htons,ntohs
#include <linux/if_packet.h>//os to handle packets
#include <netinet/ip.h>//ip hdr
#include <netinet/in.h>//in_addr
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
void printMAC(unsigned char *mac){//unsigned-values 0to positive
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);//%02X->format for add 0 if needed hexadecimal representation with word length 2
}
int main(){
    int sock;//socket
    unsigned char buffer[65536];//store recieved stream of packet
    int data_size;//size of packet recieved
    struct sockaddr saddr;//sender addr
    socklen_t saddr_len;//store size of sender addr
//socket creation
    sock=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));//socket creation
    if(sock<0){perror("socket");return 1;}//error handling
    printf("socket creation sucessfull!\n");
    while(1){
        saddr_len=sizeof(saddr);
        data_size=recvfrom(sock,buffer,sizeof(buffer),0,&saddr,&saddr_len);//recv
        if(data_size<0){perror("recvfrom");return 1;}
        printf("Packet received:%d bytes\n",data_size);
  //    eternet hdr extraction
        struct ethhdr *eth;
        if(data_size<sizeof(struct ethhdr))
         continue;//check size of packet is not too low than ether size 14bytes
        eth=(struct ethhdr *)buffer;//ethhdr eth at begin of buffer ,buffer convert to ethhdr structure
        printf("ethernet header:\n");
        printf("dest. mac:");printMAC(eth->h_dest);//dest
        printf("source. mac:");printMAC(eth->h_source);
        printf("protocol type:0x%04X\n",ntohs(eth->h_proto));//proto->ntohs

        struct iphdr *iph;
        if(ntohs(eth->h_proto)==ETH_P_IP) {
          if( data_size < sizeof(struct ethhdr) + sizeof(struct iphdr))
                continue;
        iph=(struct iphdr *)(buffer + sizeof(struct ethhdr));//create ipheadr pointer 
        struct in_addr src,des;//in_addr->stores ipv4 addr in binary(32)
        int ip_len=iph->ihl*4;
        if(ip_len<20||data_size<sizeof(struct ethhdr)+ip_len)continue;
        
        int remaining=data_size-sizeof(struct ethhdr)-ip_len;
        src.s_addr=iph->saddr;//saddr,daddr
        des.s_addr=iph->daddr;
        
        printf("\nip header\n");
        printf("source IP     :%s\n",inet_ntoa(src));//inet_ntoa->binary to address
        printf("destination IP:%s\n",inet_ntoa(des));
        printf("TTL           :%d\n",iph->ttl);//ttl -time to live -> no of router hops after being discarded
        printf("Protocol      :%d\n",iph->protocol);
        if(iph->protocol==IPPROTO_TCP){
            if(remaining<sizeof(struct tcphdr))
              continue;
            printf("\ntcp Packet\n");
            struct tcphdr *tcp;
            tcp=(struct tcphdr *)(buffer +sizeof(struct ethhdr)+iph->ihl*4);
            printf("\nTCP Header\n");
            printf("Source Port      : %u\n", ntohs(tcp->source));
            printf("Destination Port : %u\n", ntohs(tcp->dest));
 
             }
         else if(iph->protocol==IPPROTO_UDP){
             if(remaining<sizeof(struct udphdr))
               continue;
             printf("\nudp Packet\n");
             struct udphdr *udp;
             udp=(struct udphdr *)(buffer +sizeof(struct ethhdr)+iph->ihl*4);
             printf("\nUDP Header\n");
             printf("Source Port      : %u\n", ntohs(udp->source));
             printf("Destination Port : %u\n", ntohs(udp->dest));}
         else if(iph->protocol==IPPROTO_ICMP){
             if(remaining<sizeof(struct icmphdr))
               continue;
             printf("\nicmp Packet\n");
             struct icmphdr *icmp;
             icmp=(struct icmphdr *)(buffer+sizeof(struct ethhdr)+iph->ihl*4);
             printf("\nicmp header\n");
             printf("Type: %d\n",icmp->type);
             printf("code:%d\n",icmp->code);
 
         }   
         else{printf("\nother packet\n");}
        }
    }
    close(sock);
    return 0;
}
