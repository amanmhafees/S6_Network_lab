#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr,client_addr;
    char server_message[2000],client_message[2000];
    socklen_t client_struct_length=sizeof(client_addr);

    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));

    //socket creation

    socket_desc=socket(AF_INET,SOCK_DGRAM,0);
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

    printf("Listening for incoming messages\n");

    if(recvfrom(socket_desc,client_message,sizeof(client_message),0,(struct sockaddr*)&client_addr,&client_struct_length)<0){
        printf("Can't receive\n");
        return -1;
    }
    printf("The client message is : %s\n",client_message);

    printf("Enter the server message: ");
    scanf("%s",server_message);

    if(sendto(socket_desc,server_message,sizeof(server_message),0,(struct sockaddr*)&client_addr,client_struct_length)<0){
        printf("\nCan't send\n");
        return -1;

    }

    printf("Message to Client Send");

    close(socket_desc);
    return 0;
}
