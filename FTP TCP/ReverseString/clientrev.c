#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int socket_desc, s;
    struct sockaddr_in server_addr;
    char filename[50], rcvg[101];

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_desc < 0) {
        printf("Error while creating a socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2003); // Server port number
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    
    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    // Get input from the user:
    printf("Enter filename : ");
    scanf("%49s", filename); // Read user input for filename

    // Send the filename to the server:
    send(socket_desc, filename, strlen(filename), 0);

    // Receive and print reversed data from server:
    while (1) {
        s = recv(socket_desc, rcvg, 100, 0);
        if (s <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
        rcvg[s] = '\0'; // Null-terminate received data
        
        // Check if server sent "error" indicating file not found:
        if (strcmp(rcvg, "error") == 0) {
            printf("File is not available\n");
            exit(1); // Exit the program
        }
        
        // Check if server sent "completed" indicating end of file transfer:
        if (strcmp(rcvg, "completed") == 0) {
            printf("File content received and processed\n");
            break; // Exit the loop
        } else {
            // Print received reversed data to stdout:
            printf("Reversed content from server:\n%s\n", rcvg);
        }
    }

    // Close the socket:
    close(socket_desc);

    return 0;
}

