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
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[50];
    int expected_seq_num = 0;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listening failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
    if (client_sock < 0) {
        perror("Accepting connection failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    while (expected_seq_num < TOTAL_MESSAGES) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer));
        if (n <= 0) {
            perror("Read error or connection closed");
            break;
        }

        int received_seq_num;
        sscanf(buffer, "Message %d", &received_seq_num);

        if (received_seq_num == expected_seq_num) {
            printf("Received: %s\n", buffer);
            char ack[20];
            sprintf(ack, "ACK %d", expected_seq_num);
            write(client_sock, ack, strlen(ack) + 1);
            printf("Sent: %s\n", ack);
            expected_seq_num++;
        } else {
            printf("Out of order message received. Expected %d but got %d\n", expected_seq_num, received_seq_num);
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}