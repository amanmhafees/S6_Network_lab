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
    socklen_t server_struct_length=sizeof(server_addr);

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



    printf("Enter the client message: ");
    scanf("%s",client_message);

    if(sendto(socket_desc,client_message,strlen(client_message),0,(struct sockaddr*)&server_addr,server_struct_length)<0){
        printf("\nCan't send\n");
        return -1;

    }

    printf("Message to Server Send\n");
        if(recvfrom(socket_desc,server_message,sizeof(server_message),0,(struct sockaddr*)&server_addr,&server_struct_length)<0){
        printf("Can't receive\n");
        return -1;
    }
    printf("\nThe server message is : %s\n",server_message);

    close(socket_desc);
    return 0;
}
