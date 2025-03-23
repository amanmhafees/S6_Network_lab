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

    //clearing buffers
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    //checking whether socket was created or not
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket Succesfully created\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(socket_desc,(struct sockaddr*),&server_addr,sizeof(server_addr))<0){
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
    if(recv(client_sock,client_message,sizeof(client_message),0)<0){
        printf("Couldn't Receive\n");
        return -1;
    }
    printf("Message from client: %s\n",client_message);
    strcpy(server_message,"This is the server's message.");
    if(send(client_sock,server_message,strlen(Server_message),0)<0){
        printf("Can't send\n");
        return -1;
    }
    close(client_sock);
    close(socket_desc);

    return 0;
}
