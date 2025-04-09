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
    int m=1,k=5,p;

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

    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Binding failed\n");
        return -1;
    }    
    printf("Binding successful\n");

    if(listen(socket_desc,0)<0){
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening for incoming connections...................................\n");

    client_size=sizeof(client_addr);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);

    if(client_sock<0){
        printf("Can't Accpet\n");
        return -1;
    }
    printf("Connection accepted\n");


    while(k!=0){

        int y=recv(client_sock,buffer,sizeof(buffer),0);
        if(y<0){
            printf("Error in receiving\n");
            return -1;
        }
        if(strncmp(buffer,"Frame",5)==0){
            printf("Received Frame %d\n",m);
        }
        else{
            printf("Error in receiving frame %d\n",m);
        }
        if(m%2==0){
            strcpy(buffer,"ACK");
        }
        else{
            strcpy(buffer,"NACK");
            printf("ACK Lost\n");

            for(p=1;p<=3;p++){
                printf("Waiting for %d seconds\n",p);
            }
            printf("Retransmitting ACK %d\n",m);
            strcpy(buffer,"ACK");
            sleep(3);
        }
        printf("Sending ACK %d\n",m);
        int z=send(client_sock,buffer,strlen(buffer),0);
        if(z<0){
            printf("Error sending message\n");
            return -1;
        }
        m++;
        k--;
    }
    close(socket_desc);
    close(client_sock);
    return 0;
}