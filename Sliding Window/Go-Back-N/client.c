#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 9009
#define WINDOW_SIZE 3
#define TOTAL_MESSAGES 10

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[50];
    int base = 0;
    int next_seq_num = 0;

    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    fd_set read_fds;
    struct timeval timeout;
    int rv;

    while (base < TOTAL_MESSAGES) {
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < TOTAL_MESSAGES) {
            sprintf(buffer, "Message %d", next_seq_num);
            printf("Sending: %s\n", buffer);
            write(client_sock, buffer, strlen(buffer) + 1);
            next_seq_num++;
        }

        FD_ZERO(&read_fds);
        FD_SET(client_sock, &read_fds);
        timeout.tv_sec = 2; // 2-second timeout
        timeout.tv_usec = 0;

        rv = select(client_sock + 1, &read_fds, NULL, NULL, &timeout);
        if (rv == -1) {
            perror("select error");
        } else if (rv == 0) {
            printf("Timeout. Resending window starting from message %d\n", base);
            next_seq_num = base; // Reset next_seq_num to the base
        } else {
            memset(buffer, 0, sizeof(buffer));
            read(client_sock, buffer, sizeof(buffer));
            int ack_num;
            sscanf(buffer, "ACK %d", &ack_num);
            printf("Received: %s\n", buffer);

            if (ack_num >= base) {
                base = ack_num + 1; // Slide the window
            }
        }
    }

    close(client_sock);
    return 0;
}