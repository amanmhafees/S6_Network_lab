#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){

    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;
    char server_message[2000],client_message[2000];
    socklen_t client_struct_length=sizeof(client_addr);

    //clearing buffers
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));

    socket_desc(AF_INET,SOCK_STREAM,0);
    //checking whether socket was created or not
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket Succesfully created\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Can't bind to port\n");
        return -1;
    }
    printf("Binding Done\n");

    printf("Listening to incoming connections......!\n");
    
    if(recvfrom(socket_desc,client_message,sizeof(client_message),0,(struct sockassr*)&client_addr,&client_struct_length)<0){
        printf("Couldn't Receive\n");
        return -1;
    }
    printf("Message from client: %s\n",client_message);
    strcpy(server_message,"This is the server's message.");
    if(sendto(socket_desc,server_message,strlen(server_message),0,(struct sockaddr*)&client_addr,client_struct_length)<0){
        printf("Can't send\n");
        return -1;
    }
    close(socket_desc);

}