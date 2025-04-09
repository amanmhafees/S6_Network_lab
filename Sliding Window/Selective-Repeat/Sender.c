#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX 80
struct timeval timeout;


void func(int socket_desc,int f,int wz){
    char buffer[MAX];
    int ack,i=0,n,k,w1=0,w2=wz-1,j,flag=0,count_ack=0;

    if(setsockopt(socket_desc,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout))<0){
        perror("setsockopt(SO_RCVTIMEO) failed");
    }
    for(i=0;i<f && i<=w2;i++){
        bzero(buffer,sizeof(buffer));
        snprintf(buffer,sizeof(buffer),"%d",i);
        send(socket_desc,buffer,sizeof(buffer),0);
        printf("Frame %d sent\n",i);
    }
    while(1){
        if(count_ack==f){
            strcpy(buffer,"Exit");
            k=send(socket_desc,buffer,sizeof(buffer),0);
            break;
        }
        if(w2-w1!=wz-1 && flag==0 && i!=f){
            bzero(buffer,sizeof(buffer));
            snprintf(buffer,sizeof(buffer),"%d",i);
            send(socket_desc,buffer,sizeof(buffer),0);
            printf("frame %d sent\n",i);
            w2++;
            i++;
        }
        flag=0;
        bzero(buffer,sizeof(buffer));
        n=recv(socket_desc,buffer,sizeof(buffer),0);
        ack=atoi(buffer);
        if(n>0){
            if(ack+1==f){
                printf("Acknowledgement received : %d\nExit\n",ack);
                count_ack++;
                bzero(buffer,sizeof(buffer));
                strcpy(buffer,"Exit");
                k=send(socket_desc,buffer,sizeof(buffer),0);
                break;
            }
            else if(ack==-1){
                printf("acknowledgement not received for %d\nResending frame\n",w1);
                bzero(buffer,sizeof(buffer)); 
                    snprintf(buffer,sizeof(buffer),"%d",w1); 
                    k = send(socket_desc,buffer,sizeof(buffer),0); 
                    printf("Frame %d sent \n",w1);   
            }
            else{
                w1++;
                printf("acknowledgement received: %d\n",ack);
                count_ack++;
            }
        }
    }
}

int main(void){
    int socket_desc,f,wz;
    struct sockaddr_in server_addr,client_addr;

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    timeout.tv_sec=3;
    timeout.tv_usec=5;

    if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Connection Failed\n)");
        return -1;
    }
    printf("Connection to server succesful\n");
    printf("Enter the number of frames: ");
    scanf("%d",&f);
    printf("Enter the window size: ");
    scanf("%d",&wz);

    func(socket_desc,f,wz);

    close(socket_desc);
    return 0;

}