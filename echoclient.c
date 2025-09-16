#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1313

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    char message[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Configure server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "192.168.235.251", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(1);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(1);
    }
    printf("Connected to echo server. Type messages (CTRL+C to exit):\n");

    // Loop to send messages to server and receive echo
    while (1) {
        printf("> ");
        fgets(message, sizeof(message), stdin);
        n = write(sockfd, message, strlen(message));
        if (n < 0) {
            perror("ERROR writing to socket");
            break;
        }
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd, buffer, 1023);
        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("Echo: %s", buffer);
    }

    close(sockfd);
    return 0;
}