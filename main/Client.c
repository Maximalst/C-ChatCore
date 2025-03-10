#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.178.148"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Get message from user
        printf("Enter message (type 'exit' to quit): ");
        fgets(message, BUFFER_SIZE, stdin);

        // Remove newline character from message
        message[strcspn(message, "\n")] = 0;

        // Check if the user wants to exit
        if (strcmp(message, "exit") == 0) {
            break;
        }

        // Send message to server
        send(sockfd, message, strlen(message), 0);
        printf("Message sent to %s:%d\n", SERVER_IP, SERVER_PORT);
    }

    // Close socket
    close(sockfd);

    return 0;
}