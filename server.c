#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    char str[100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr;

    // Create socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    // Initialize server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    servaddr.sin_port = htons(22000);

    // Bind socket
    if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("ERROR on binding");
        close(listen_fd);
        return 1;
    }

    // Listen for connections
    if (listen(listen_fd, 10) < 0) {
        perror("ERROR on listen");
        close(listen_fd);
        return 1;
    }

    printf("Server listening on port 22000...\n");

    // Accept connections
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
    if (comm_fd < 0) {
        perror("ERROR on accept");
        close(listen_fd);
        return 1;
    }

    printf("Client connected.\n");

    // Main loop to read and echo client messages
    while (1) {
        bzero(str, 100);
        int n = read(comm_fd, str, 100);
        if (n <= 0) {
            if (n == 0) {
                printf("Client disconnected.\n");
            } else {
                perror("ERROR reading from socket");
            }
            close(comm_fd);
            break;
        }

        printf("Echoing back - %s", str);
        if (write(comm_fd, str, strlen(str) + 1) < 0) {
            perror("ERROR writing to socket");
            close(comm_fd);
            break;
        }
    }

    close(listen_fd);
    return 0;
}
