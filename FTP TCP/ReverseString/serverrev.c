#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

// Function to reverse a string
void reverseString(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int main(void) {
    FILE *fp;
    int socket_desc, client_sock, client_size, n;
    struct sockaddr_in server_addr, client_addr;
    char rcv[100], fileread[100];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2003); // Port number 2000
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Accept connections from any IP address

    // Bind to the set port and IP
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients
    if (listen(socket_desc, 1) < 0) {
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening for incoming connections...\n");

    // Accept an incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
    if (client_sock < 0) {
        printf("\nCan't accept\n");
        return -1;
    }
    printf("Client connected at IP:%s and port=%i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    printf("Connection accepted\n");

    // Receive filename from client
    n = recv(client_sock, rcv, 100, 0);
    rcv[n] = '\0'; // Null-terminate the received data as it's a string (filename)

    // Open the requested file
    fp = fopen(rcv, "r");
    if (fp == NULL) {
        // If file doesn't exist or cannot be opened, send an error message to client
        send(client_sock, "error", 5, 0);
        close(client_sock);
    } else {
        // If file opened successfully, read its contents
        while (fgets(fileread, sizeof(fileread), fp)) {
            // Reverse the string
            reverseString(fileread);
            
            // Print the reversed string to the console
            printf("Reversed content of the file:\n%s\n", fileread);
        }
        fclose(fp);
        send(client_sock, "completed", 9, 0); // Notify client that file transmission is completed
    }

    // Close client socket and server socket
    close(client_sock);
    close(socket_desc);

    return 0;
}

// Function to reverse a string

