#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>

struct timeval timeout;

void func(int sock_desc,int num_frame,int window_size){

    timeout.tv_sec=5;
    timeout.tv_usec=0;

    char buffer[8000];
    int ack=-1,i=0,n,k,w1=0,w2=w-1,j,count_ack=0,flag=0;
    if(setsockopt(sock_desc,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout))<0){
        perror("setsockopt(SO_RCVTIMEO) failed");
        return;
    }

    for(i=0;i<num_frame && i<window_size; i++){
        bzero(buffer,sizeof(buffer));
        snprintf(buffer,sizeof(buffer),"%d",i);
        k=send(sock_desc,buffer,sizeof(buffer),0);
        printf("Frame %d sent\n",i);
    }

    while(1){
        if(count_ack==num_frame){
            strcpy(buffer,"Exit");
            k=send(sock_desc,buffer,sizeof(buffer),0);
            break;
        }

        if(w2-w1!=window_size-1 && flag==0 && i!=num_frame){
            bzero(buffer,sizeof(buffer));
            snprintf(buffer,sizeof(buffer),"%d",i);
            k=send(sock_desc,buffer,sizeof(buffer),0);
            printf("Frame %d sent\n",i);
            w2++;
            i++;
        }
        flag=0;
        bzero(buffer,sizeof(buffer));
        n=recv(sock_desc,buffer,sizeof(buffer),0);
        ack=atoi(buffer);
        if(n>0){
            if(ack+1==num_frame){
                printf("Acknowledgement %d received\nExit\n",ack);
                strcpy(buffer,"Exit");
                k=send(sock_desc,buffer,sizeof(buffer),0);
                break;
            }
            else if(ack==-1){
                printf("Acknowledgement not received for %d\nResending frame\n",w1);
                bzero(buffer,sizeof(buffer));
                snprintf(buffer,sizeof(buffer),"%d",w1);
                k=send(sock_desc,buffer,sizeof(buffer),0);
                printf("Frame %d sent\n",w1);
            }else{
                w1++;
                printf("Acknowledgement %d received\n",ack);
                count_ack++;
            }
        }
    }

}

int main(void){

    int sock_desc,num_frames,window_size;
    struct sockaddr_in server_addr;
    
    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    if(sock_desc==-1){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket Created Succesfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Can't connect to server\n");
        return -1;
    }
    printf("Connected to server\n");

    printf("Enter the number of frames to be sent: ");
    scanf("%d", &num_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    func(sock_desc,num_frames,window_size);
    close(sock_desc);
}