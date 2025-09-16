#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int sockfd, n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);

    if (inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr)) <= 0) {
        perror("Invalid address/Address not supported");
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection Failed");
        close(sockfd);
        return 1;
    }

    printf("Connected to server. Type messages (CTRL+C to exit):\n");

    while (1) {
        bzero(sendline, 100);
        bzero(recvline, 100);
        if (fgets(sendline, 100, stdin) == NULL) {
            break;
        }

        n = write(sockfd, sendline, strlen(sendline) + 1);
        if (n < 0) {
            perror("ERROR writing to socket");
            close(sockfd);
            break;
        }

        n = read(sockfd, recvline, 100);
        if (n <= 0) {
            if (n == 0) {
                printf("Server disconnected.\n");
            } else {
                perror("ERROR reading from socket");
            }
            close(sockfd);
            break;
        }

        printf("%s", recvline);
    }

    close(sockfd);
    return 0;
}
