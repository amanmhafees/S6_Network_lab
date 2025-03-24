#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){

    int socket_desc;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int k=5,m=1,p;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket Succesfully created\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Can't connect to server\n");
        return -1;
    }
    printf("Connected to server\n");

    while(K!=0){
        if(m<=5)printf("Sending frame %d\n",m);
        if(m%2==0){
            strcpy(buffer,"frame");
        }
        else{
            strcpy(buffer,"error");
            printf("packet loss\n");

            for(p=1;p<=3;p++){
                printf("Waiting for %d seconds\n",p);

            }
            printf("Retransmitting......\n");
            strcpy(buffer,"frame");

            sleep(3);
        }
        int y=send(socket_desc,buffer,19,0);
        if(y==-1){
            printf("Error in sending\n");
            return -1;
        }
        else{
            printf("Send frame %d\n",m);
        }
        int z=recv(socket_desc,buffer,1024,0);
        if(z==-1){
            printf("Error in receiving\n");
        }
        if(strncmp(buffer,"ACK",3)==0){
            printf("Recieved ACK %d\n",m);
        }
        m++;
        k--;
    }
    close(socket_desc);
    return 0;

}