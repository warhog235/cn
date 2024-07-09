#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for close()
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024
#define PORT 8080

int main() {
    int serversocket, clientsocket;
    struct sockaddr_in serveraddress, clientaddress;
    int clientaddresslength = sizeof(clientaddress);
    char buffer[BUFFERSIZE];

    // Create server socket
    if ((serversocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serversocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0) {
        perror("Bind failed");
        close(serversocket);
        exit(EXIT_FAILURE);
    }

    // Listen on the socket
    if (listen(serversocket, 3) < 0) {
        perror("Listen failed");
        close(serversocket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    if ((clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&clientaddresslength)) < 0) {
        perror("Accept failed");
        close(serversocket);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted\n");

    while (1) {
        // Receive message from client
        int recv_len = recv(clientsocket, buffer, BUFFERSIZE, 0);
        if (recv_len < 0) {
            perror("Receive failed");
            break;
        }
        buffer[recv_len] = '\0';
        printf("Client: %s\n", buffer);

        // Check for "bye" to close the connection
        if (strcmp(buffer, "bye") == 0) {
            printf("Client has ended the chat.\n");
            break;
        }

        // Send message to client
        printf("Server: ");
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
            printf("Server has ended the chat.\n");
            break;
        }
    }

    // Close sockets
    close(clientsocket);
    close(serversocket);

    return 0;
}
