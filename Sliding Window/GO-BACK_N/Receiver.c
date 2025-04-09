#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAX 80
struct timeval timeout;


void func(int client_sock){

    char buffer[MAX];
    int f,ack=0,next=0,c;
    while(1){
        sleep(1);
        bzero(buffer,MAX);
        recv(client_sock,buffer,MAX,0);
        if(strcmp("Exit",buffer)==0){
            printf("Exit\n");
            break;
        }
        f=atoi(buffer);
        if(f!=next){
            printf("Frame %d discarded\n acknowledgement sent:%d\n",f,ack);
            bzero(buffer,MAX);
            snprintf(buffer,MAX,"%d",ack);
            send(client_sock,buffer,strlen(buffer),0);
            continue;
        }
        c=rand()%3;
        switch(c){
            case 0:
                printf("Frame %d not received\n",f);
                break;
            case 1:
                ack=f;
                sleep(2);
                printf("Frame %d received\nAcknowledgement sent :%d\n",f,ack);
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",ack);
                send(client_sock,buffer,strlen(buffer),0);
                next=ack+1;
                break;
            case 2:
                ack=f;
                printf("Frame %d received\nAcknowledgement sent :%d\n",f,ack);
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",ack);
                send(client_sock,buffer,strlen(buffer),0);
                next=ack+1;
                break;
        }
    }

}

int main(void){
    int socket_desc,client_size,client_sock,nf,ws;
    struct sockaddr_in server_addr,client_addr;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);

    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created succesfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    timeout.tv_sec=3;
    timeout.tv_usec=5;

    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Binding failed\n");
        return -1;
    }
    printf("Binding Succesful");
    if(listen(socket_desc,5)<0){
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening to incoming connections.....\n");

    client_size=sizeof(client_addr);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);

    if(client_sock<0){
        printf("Cant accept\n");
        return -1;
    }
    printf("Connection accepted\n");

    func(client_sock);

    close(socket_desc);
    close(client_sock);
    return 0;

}