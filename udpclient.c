#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int s=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in server;
    socklen_t len;
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(12345);
    char file[100],data[1024];
    len=sizeof(server);
    printf("----CLIENT>>>>>\n");
    while(1){ 
    printf("enter filename:\n");
    scanf("%99s", file);
    sendto(s,file,strlen(file),0,(struct sockaddr *)&server,len);
    int n=recvfrom(s,data,sizeof(data)-1,0,(struct sockaddr *)&server,&len);
    data[n]='\0';
    printf("file content:\n%s\n",data);}
    close(s);
    return 0;
}
