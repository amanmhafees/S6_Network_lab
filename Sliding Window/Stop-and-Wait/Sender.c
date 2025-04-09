#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void){
    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;
    char buffer[1024];
        int k=5,m=1,p;

    //creating socket
    socket_desc=socket(AF_INET,SOCK_STREAM,0);

    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket creation succeful\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Connection Failed\n");
        return -1;
    }
    printf("Connection to server succesful\n");

    while(k!=0){
        if(m<=5){
            printf("Sending Frame %d\n",m);
        }
        if(m%2==0){
            strcpy(buffer,"Frame");
        }
        else{
            strcpy(buffer,"error");
            printf("packet loss\n");

            for(p=1;p<=3;p++){
                printf("Waiting for %d seconds\n",p);
            }
            printf("Retransmiting........");
            strcpy(buffer,"Frame");
            sleep(3);
        }
        int y=send(socket_desc,buffer,strlen(buffer),0);
        if(y<0){
            printf("Error in sending\n");
            return -1;
        }
        else{
            printf("Send frame %d\n",m);
        }
        int z=recv(socket_desc,buffer,sizeof(buffer),0);
        if(z<0){
            printf("Error in receiving\n");
        }
        if(strncmp(buffer,"ACK",3)==0){
            printf("Received ACK %d\n",m);
        }
        m++;
        k--;
    }
    close(socket_desc);
    return 0;
}