#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for close()
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024
#define PORT 8080

int main() {
    int clientsocket;
    struct sockaddr_in serveraddress;
    char buffer[BUFFERSIZE];

    // Create client socket
    if ((clientsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0) {
        perror("Connect failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    while (1) {
        // Send message to server
        printf("Client: ");
        if (!fgets(buffer, BUFFERSIZE, stdin)) {
            perror("Failed to read input");
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        if (send(clientsocket, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            break;
        }

        // Check for "bye" to close the connection
        if (strcmp(buffer, "bye") == 0) {
            printf("Client has ended the chat.\n");
            break;
        }

        // Receive message from server
        int recv_len = recv(clientsocket, buffer, BUFFERSIZE, 0);
        if (recv_len < 0) {
            perror("Receive failed");
            break;
        }
        buffer[recv_len] = '\0';
        printf("Server: %s\n", buffer);

        // Check for "bye" to close the connection
        if (strcmp(buffer, "bye") == 0) {
            printf("Server has ended the chat.\n");
            break;
        }
    }

    // Close the socket
    close(clientsocket);

    return 0;
}
