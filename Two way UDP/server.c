#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024
#define PORT 8080

void main()
{
    int serversocket;
    struct sockaddr_in serveraddress, clientaddress;
    int clientaddresslength = sizeof(clientaddress);
    char buffer[BUFFERSIZE];

    // Create UDP socket
    if ((serversocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP socket created successfully.\n");

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
    printf("Socket bind successful on port %d.\n", PORT);

    // Receive message from client
    printf("Waiting to receive message from client...\n");
    if (recvfrom(serversocket, buffer, BUFFERSIZE, 0, (struct sockaddr *)&clientaddress, (socklen_t *)&clientaddresslength) < 0) {
        perror("Receive failed");
        close(serversocket);
        exit(EXIT_FAILURE);
    }
    printf("Message received from client: %s\n", buffer);

    // Reverse the message
    int j = strlen(buffer) - 1;
    for (int i = 0; i < j; i++, j--) {
        int temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    // Send reversed message back to client
    if (sendto(serversocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)) < 0) {
        perror("Send failed");
        close(serversocket);
        exit(EXIT_FAILURE);
    }
    printf("Reversed message sent back to client: %s\n", buffer);

    // Close the socket
    close(serversocket);
    printf("Socket closed.\n");
}
