#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_client(int client_sock) {
    char buffer[2000];
    int frame, ack = 0, next = 0;

    while (1) {
        bzero(buffer, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "Exit") == 0) {
            printf("Exit\n");
            break;
        }

        frame = atoi(buffer);
        if (frame != next) {
            printf("Frame %d discarded\nAcknowledgement sent: %d\n", frame, ack - 1);
            snprintf(buffer, sizeof(buffer), "%d", ack - 1);
            send(client_sock, buffer, sizeof(buffer), 0);
            continue;
        }

        int rand_choice = rand() % 3;
        if (rand_choice == 0) {
            printf("Frame %d not received\n", frame);
        } else {
            ack = frame;
            if (rand_choice == 1) sleep(2);
            printf("Frame %d received\nAcknowledgement sent: %d\n", frame, ack);
            snprintf(buffer, sizeof(buffer), "%d", ack);
            send(client_sock, buffer, sizeof(buffer), 0);
            next = ack + 1;
        }
    }
}

int main() {
    int sock_desc, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size;

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket successfully created\n");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Binding done\n");

    if (listen(sock_desc, 5) < 0) {
        printf("Error in listening\n");
        return -1;
    }
    printf("Listening to incoming connections...\n");

    client_size = sizeof(client_addr);
    client_sock = accept(sock_desc, (struct sockaddr*)&client_addr, &client_size);
    if (client_sock < 0) {
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    handle_client(client_sock);

    close(client_sock);
    close(sock_desc);
    return 0;
}