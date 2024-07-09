#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024
#define PORT 8080

void main()
{
    int clientsocket;
    struct sockaddr_in serveraddress;
    int serveraddresslength = sizeof(serveraddress);
    char buffer[BUFFERSIZE];

    // Create UDP socket
    if ((clientsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP socket created successfully.\n");

    // Prepare the sockaddr_in structure
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);

    // Prompt user for input
    printf("\nEnter the string to be reversed : ");
    if (!fgets(buffer, BUFFERSIZE, stdin)) {
        perror("Failed to read input");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }

    // Send data to server
    if (sendto(clientsocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0) {
        perror("Send failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server.\n");

    // Receive data from server
    memset(buffer, 0, BUFFERSIZE); // Clear buffer
    if (recvfrom(clientsocket, buffer, BUFFERSIZE, 0, (struct sockaddr *)&serveraddress, (socklen_t *)&serveraddresslength) < 0) {
        perror("Receive failed");
        close(clientsocket);
        exit(EXIT_FAILURE);
    }
    printf("Reversed string received from server: %s\n", buffer);

    // Close the socket
    close(clientsocket);
    printf("Socket closed.\n");
}
