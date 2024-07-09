#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
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
    serveraddress.sin_addr.s_addr = INADDR_ANY; 
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0) {
        perror("Connect failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");
    printf("Enter the string to be reversed: ");
    if (!fgets(buffer, BUFFERSIZE, stdin)) {
        perror("Failed to read input");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    // Remove newline character from the buffer
    buffer[strcspn(buffer, "\n")] = 0;

    // Send the string to the server
    if (send(clientsocket, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    // Clear the buffer
    memset(buffer, 0, BUFFERSIZE);

    // Receive the reversed string from the server
    int recv_len;
    if ((recv_len = recv(clientsocket, buffer, BUFFERSIZE, 0)) < 0) {
        perror("Receive failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0'; // Null-terminate the received string
    printf("Reversed string from server: %s\n", buffer);

    // Close the socket
    close(clientsocket);

    return 0;
}
