#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#define PORT 8080
#define WINDOWSIZE 4

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int base = 0;
    bool ackreceived[999];

    for (int i = 0; i < 999; i++) {
        ackreceived[i] = false;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        for (int i = base; i < base + WINDOWSIZE; i++) {
            printf("\n\t\t\t\t\t\t[%d][%d][%d][%d]", base, base + 1, base + 2, base + 3);
            printf("\nFrame[%d] sent", i);

            sprintf(buffer, "Frame[%d]", i);
            send(sock, buffer, strlen(buffer), 0);
            sleep(1);

            srand(time(NULL));
            if (rand() % 2 == 0 && base == i) {
                valread = read(sock, buffer, 1024);
                buffer[valread] = '\0';
                printf("\n\t\t\tAcknowledge[%d] received", i);
                ackreceived[i] = true;
                base++;
            }
        }
    }

    return 0;
}
