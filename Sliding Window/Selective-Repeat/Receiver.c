#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX 80
#define PORT 8080
struct timeval timeout;


void func(int client_sock){
    char buffer[MAX];
    int f,c,ack,next=0;

    while(1){
        sleep(1);
        bzero(buffer,MAX);
        recv(client_sock,buffer,MAX,0);
        if(strcmp("Exit",buffer)==0){
            printf("Exit\n)");
            break;
        }
        f=atoi(buffer);
        c=rand()%3;
        switch(c){
            case 0:
                printf("Frame %d not received \n",f);
                ack=-1;
                printf("Negative Acknowledgement sent: %d\n",f);
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",ack);
                send(client_sock,buffer,MAX,0);
                break;
            case 1:
                ack=f;
                sleep(2);
                printf("Frame %d received\nAcknowledgement  sent : %d\n",f,ack);
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",ack);
                send(client_sock,buffer,MAX,0);
                next=ack+1;
                break;
            case 2:
                ack=f;
                printf("Frame %d received\nAcknowledgement  sent : %d\n",f,ack);
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",ack);
                send(client_sock,buffer,MAX,0);
                next=ack+1;
                break;
        }
    }
}


int main(void){
    int socket_desc,connfd,len;
    struct sockaddr_in server_addr,client_addr;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    else{
        printf("Socket succssfully created \n");
        bzero(&server_addr,sizeof(server_addr));

        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(2000);
        server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

        timeout.tv_sec=3;
        timeout.tv_usec=0;
    }
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Binding failed\n");
        return -1;
    }
    printf("Binding successful\n)");
    if(listen(socket_desc,5)<0){
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening for incoming connections........\n");
    len=sizeof(client_addr);
    connfd=accept(socket_desc,(struct sockaddr*)&client_addr,&len);
    if(connfd<0){
        printf("Error whiile accepting connections");
        return -1;
    }
    printf("Connection accepted\n");
    func(connfd);
    close(socket_desc);
    return 0;
}