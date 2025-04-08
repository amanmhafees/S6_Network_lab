#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void){
    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;
    char server_message[2000],client_message[2000];

    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));

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

    printf("\nEnter the client message : ");
    scanf("%s",client_message);

    if(send(socket_desc,client_message,sizeof(client_message),0)<0){
        printf("\nCan't Send\n");
        return -1;
    }

    if(recv(socket_desc,server_message,sizeof(server_message),0)<0){
        printf("Can't receive\n");
        return -1;
    }
    printf("The server Message is:\n");
    printf(server_message);
    printf("\n");


    close(socket_desc);



    
}