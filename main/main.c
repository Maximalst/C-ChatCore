#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// Config


int port = 187; // port to listen on server

// Config end



int Server() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket creation failed");
        return -1;
    }

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Binding the socket to the port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind failed");
        close(sockfd);
        return -1;
    }

    // Listening for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("listen failed");
        close(sockfd);
        return -1;
    }

    printf("Server is listening on port %d\n", port);







    return 0;
}

int Client() {
    printf("Start Client\n");
    return 0;
}




int main() {
    int mode;
    printf("\n\n\n\n\n\n\n\n\n");
        printf("  _____       _____ _           _    _____                          \n");
        printf(" / ____|     / ____| |         | |  / ____|                         \n");
        printf("| |   ______| |    | |__   __ _| |_| |     ___  _ __ ___            \n");
        printf("| |  |______| |    | '_ \\ / _` | __| |    / _ \\| '__/ _ \\        \n");
        printf("| |____     | |____| | | | (_| | |_| |___| (_) | | |  __/           \n");
        printf(" \\_____|     \\_____|_| |_|\\__,_|\\__|\\_____\\___/|_|  \\___|    \n");
        printf("V0.005\n");
        printf("Please visit our Website vor more Information:\n maximalst.de \n\n\n");



        printf("Auswahlmenü:\n");
        printf("1. Server\n");
        printf("2. Client\n");
        printf("Please enter your choice: ");
        scanf("%d", &mode);

    switch (mode) {
        case 1:
            printf("Server\n");
            Server();
            break;
        case 2:
            printf("Client\n");
            break;
        case 3:
            printf("Debug Starting...\n");
            break;
        default:
            printf("Number is not 1, 2, or 3\n");
            break;
    }

    return 0;
}