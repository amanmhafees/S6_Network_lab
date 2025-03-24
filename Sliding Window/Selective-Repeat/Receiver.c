#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>

struct timeval timeout;

void func(int client_sock){

    char buffer[8000];
    int f,c,ack,next=0;
    while(1){
        sleep(1);
        bzero(buffer,sizeof(buffer));
        recv(client_sock,buffer,sizeof(buffer),0);
        if(strcmp(buffer,"Exit")==0){
            printf("Exit\n");
            break;
        }
        f=atoi(buffer);
        c=rand()%3;

        switch(c){

            case 0:
                printf("Frame %d not received\n",f);
                ack=-1;
                printf("Negative Acknowledgement sent: %d\n",ack);
                snprintf(buffer,sizeof(buffer),"%d",ack);
                send(client_sock,buffer,sizeof(buffer),0);
                break;
            case 1:
                ack=f;
                sleep(2);
                printf("Frame %d received\nAcknowledgement sent: %d\n",f,ack);
                bzero(buffer,sizeof(buffer));
                snprintf(buffer,sizeof(buffer),"%d",ack);
                send(client_sock,buffer,sizeof(buffer),0);
                next=ack+1;
                break;

            case 2:
            ack=f;
            printf("Frame %d received\nAcknowledgement sent: %d\n",f,ack);
            bzero(buffer,sizeof(buffer));
            snprintf(buffer,sizeof(buffer),"%d",ack);
            send(client_sock,buffer,sizeof(buffer),0);
            next=ack+1;
            break;

        }
    }

}


int main(void){

    int socket_desc,client_size,client_sock;
    struct sockaddr_in server_addr,client_addr;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);

    if(socket_desc==-1){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket Created Succesfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Binding Done\n");

    if(listen(socket_desc,1)<0){
        printf("Error in listening\n");
        return -1;
    }
    printf("Listening to incoming connections......!\n");

    client_size=sizeof(client_addr);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
    if(client_sock<0){
        printf("Can't Accept\n");
        return -1;
    }
    printf("Client conected at IP: %s and port: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    func(client_sock);

}