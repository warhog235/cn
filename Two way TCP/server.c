#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close()

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
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

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

    // Receive message from client
    int recv_len;
    if ((recv_len = recv(clientsocket, buffer, BUFFERSIZE, 0)) < 0) {
        perror("Receive failed");
        close(serversocket);
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0'; // Null-terminate the received string
    printf("Received message: %s\n", buffer);

    // Reverse the message
    int j = strlen(buffer) - 1;
    for (int i = 0; i < j; i++, j--) {
        int temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    // Send reversed message back to client
    if (send(clientsocket, buffer, recv_len, 0) < 0) {
        perror("Send failed");
        close(serversocket);
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    printf("Reversed message sent: %s\n", buffer);

    // Close sockets
    close(clientsocket);
    close(serversocket);

    return 0;
}
