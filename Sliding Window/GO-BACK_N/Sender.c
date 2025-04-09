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


void func(int socket_desc,int nf,int ws){
    char buffer[MAX];
    int i=0,j,ack,w1=0,w2=ws-1,flag=0,n;

    if(setsockopt(socket_desc,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout))<0){
        perror("setsockopt(SO_RCVTIMEO) failed\n");
        return;
    }
    for(i=w1;i<=w2 && i<nf; i++){
        bzero(buffer,MAX);
        snprintf(buffer,MAX,"%d",i);
        send(socket_desc,buffer,strlen(buffer),0);
        printf("Frame %d sent\n",i);
    }
    while(1){
        if(w2-w1!=ws-1 && flag==0 && i!=nf){
            bzero(buffer,MAX);
            snprintf(buffer,MAX,"%d",i);
            send(socket_desc,buffer,strlen(buffer),0);
            printf("Frame %d sent\n",i);
            w2++;
            i++;
        }
        flag=0;
        bzero(buffer,MAX);
        n=recv(socket_desc,buffer,MAX,0);
        ack=atoi(buffer);
        if(n>0){
            if(ack+1==nf){
                printf("Acknowledgement received : %d\nExit\n\n",ack);
                bzero(buffer,MAX);
                strcpy(buffer,"Exit");
                send(socket_desc,buffer,MAX,0);
                break;
            }
            if(ack==w1){
                w1++;
                printf("Acknowledgement received %d\n",ack);
            }
        }
        else{
            printf("Acknowledgement not received for %d\nResending framess\n",w1);
            for(i=w1;i<=w2 && i<nf; i++){
                bzero(buffer,MAX);
                snprintf(buffer,MAX,"%d",i);
                send(socket_desc,buffer,MAX,0);
                printf("Frame %d sent\n",i);   
            }
            flag=1;
        }
    }

}

int  main(void){
    int socket_desc,nf,ws;
    struct sockaddr_in server_addr,client_addr;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    timeout.tv_sec=3;
    timeout.tv_usec=5;

    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Connection failed\n");
        return -1;
    }
    printf("Connected successfully\n");
        printf("Enter the number of frames: ");
    scanf("%d",&nf);
    printf("Enter the window size: ");
    scanf("%d",&ws);

    func(socket_desc,nf,ws);

    close(socket_desc);
    return 0;

}