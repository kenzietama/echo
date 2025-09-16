#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1313

int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Echo server listening on port %d...\n", PORT);

    // Main loop to accept client connections
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }
        printf("Client connected.\n");

        // Loop to read and echo client messages
        while (1) {
            memset(buffer, 0, 1024);
            n = read(newsockfd, buffer, 1023);
            if (n <= 0) {
                printf("Client disconnected.\n");
                close(newsockfd);
                break;
            }
            printf("Received: %s", buffer);
            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("ERROR writing to socket");
            }
        }
    }

    close(sockfd);
    return 0;
}