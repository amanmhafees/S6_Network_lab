#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void){
    FILE *fp;
    char name[100],rcvmsg[100],rcvg[100],fname[100];
    int n,s;
    int sock_desc;
    struct sockaddr_in server_addr;

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

    if(connect(sock_desc,(struct socaddr*)&server_addr,sizeof(server_addr))<0){
        printf("Connection failed\n");
        return -1;
    }
    printf("Connected to server\n");

    printf("Enter the file name: ");
    scanf("%s",name);
    printf("Enter the new file name: ");
    scanf("%s",fname);
    fp=fopen(fname,"w");
    send(sock_desc,name,sizeof(name),0);
    while(1){
        s=recv(sock_desc,rcvg,100,0);
        rcvg[s]='\0';
        if(strcmp(rcvg,"error")==0){
            printf("File not found\n");
            return -1;
        }
        if(strcmp(rcvg,"completed")==0){
            printf("\nFile is transfered....\n");
            fclose(fp);
            return -1;
        }
        else{
            fputs(rcvg,stdout);
            fprintf(fp,"%s",rcvg);
        }
    }


    close(sock_desc);
}