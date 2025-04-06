#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 2000

void send_frames(int sock_desc, int num_frames, int window_size) {
    char buffer[2000];
    int ack = -1, i = 0, j, k, n, w1 = 0, w2 = window_size - 1, flag = 0;
    struct timeval timeout;

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if (setsockopt(sock_desc, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt(SO_RCVTIMEO) failed");
        return;
    }

    for (i = 0; i < num_frames && i < window_size; i++) {
        bzero(buffer, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%d", i);
        k = send(sock_desc, buffer, sizeof(buffer), 0);
        printf("Frame %d sent\n", i);
    }

    while (1) {
        if (w2 - w1 != window_size - 1 && flag == 0 && i != num_frames) {
            bzero(buffer, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", i);
            k = send(sock_desc, buffer, sizeof(buffer), 0);
            printf("Frame %d sent\n", i);
            w2++;
            i++;
        }
        flag = 0;
        bzero(buffer, sizeof(buffer));
        n = recv(sock_desc, buffer, sizeof(buffer), 0);
        if (n > 0) {
            ack = atoi(buffer);
            if (ack + 1 == num_frames) {
                printf("Acknowledgement %d received\nExit\n", ack);
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "Exit");
                k = send(sock_desc, buffer, sizeof(buffer), 0);
                break;
            }
            if (ack >= w1) {
                w1 = ack + 1;
                printf("Acknowledgement %d received\n", ack);
            }
        } else {
            printf("Acknowledgement not received for %d\nResending Frames\n", w1);
            for (j = w1; j < num_frames && j <= w1 + window_size - 1; j++) {
                bzero(buffer, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%d", j);
                k = send(sock_desc, buffer, sizeof(buffer), 0);
                printf("Frame %d sent\n", j);
            }
            flag = 1;
        }
    }
}

int main() {
    int sock_desc, num_frames, window_size;
    struct sockaddr_in server_addr;

    printf("Enter the number of frames to be sent: ");
    scanf("%d", &num_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket successfully created\n");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in connecting\n");
        return -1;
    }
    printf("Connected to the server\n");

    send_frames(sock_desc, num_frames, window_size);

    close(sock_desc);
    return 0;
}