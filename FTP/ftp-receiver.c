#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void){
    FILE *fp;
    char name[100],fileread[100],fname[100],ch,file[100],rcv[100];
    int n;
    int sock_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;

    //creating a socket
    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    if(sock_desc<0){
        printf("Error While creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    //set port and ip
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //Bind to the set port and ip
    if(bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Binding Failed\n");
        return -1;
    }
    printf("\nSocket binding succesful\n");
    if(listen(sock_desc,1)<0){
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening for incoming connections\n");

    client_size=sizeof(client_addr);
    client_sock=accept(sock_desc,(struct sockaddr*)&client_addr,&client_size);
    
    if(client_sock<0){
        printf("Can't accept\n");
        return -1;
    }
    printf("Connection accepted\n");

    n=recv(client_sock,rcv,100,0);
    rcv[n]='\0';
    fp=fopen(rcv,"r");
    if(fp==NULL){
        send(client_sock,"error",sizeof("error"),0);
        return -1;
    }
    else{
        while(fgets(fileread,100,fp)!=NULL){
            if(send(client_sock,fileread,sizeof(fileread),0)<0){
                printf("Can't Send File content\n");
            }
            sleep(1);
        }
        if(!fgets(fileread,sizeof(fileread),fp)){
            printf("Done......\n");
            send(client_sock,"completed",sizeof("completed"),0);
        }
    }

    close(sock_desc);
    close(client_sock);
    return 0;
}
