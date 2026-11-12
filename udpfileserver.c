
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
int main()
{
    int s, n;
    char file[100], data[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr *)&server, sizeof(server));
    printf("UDP Server running>>>>>>>\n");
    while (1)
    {
        n=recvfrom(s,file,sizeof(file)-1,0,(struct sockaddr*)&client, &len);
        file[n]='\0';
        printf("%s requested\n",file);
        if(fork()==0)
        {
            FILE *fp = fopen(file, "r");
            if (fp == NULL)
                sprintf(data, "File not found! PID: %d", getpid());
            else
            {
                n=fread(data, 1, sizeof(data)-50, fp);
                data[n]='\0';
                fclose(fp);
                char temp[1024];
                strcpy(temp, data);
                snprintf(data, sizeof(data), "%.*s\nServer PID: %d",970, temp, getpid());
            }
            sendto(s, data, strlen(data), 0,(struct sockaddr *)&client, len);
            close(s);
            exit(0);
        }
    }

    return 0;
}
