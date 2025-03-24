#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void server() {
    int port;
    printf("Enter the port number to start the server: ");
    scanf("%d", &port);
    printf("Starting server on port %d\n", port);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        
        int valread = read(new_socket, buffer, 1024);
        printf("Received message: %s\n", buffer);
        close(new_socket);
    }
}

void client() {
    char ip[100];
    int port;
    char message[1024];

    printf("Enter the server IP address: ");
    scanf("%s", ip);
    printf("Enter the port number: ");
    scanf("%d", &port);
    printf("Enter the message to send: ");
    scanf(" %[^\n]s", message);

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }

    send(sock, message, strlen(message), 0);
    printf("Message sent\n");
    int valread = read(sock, buffer, 1024);
    printf("Received message: %s\n", buffer);
    close(sock);
}

void debug() {
    printf("Start Debug on port 187\n");
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client: Debug";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(187);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }

    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    
}

int main() {
    int choice;
    do {
        printf("\n\n\n\n\n\n\n\n\n");
        printf("  _____       _____ _           _    _____               \n");
        printf(" / ____|     / ____| |         | |  / ____|              \n");
        printf("| |   ______| |    | |__   __ _| |_| |     ___  _ __ ___ \n");
        printf("| |  |______| |    | '_ \\ / _` | __| |    / _ \\| '__/ _ \\\n");
        printf("| |____     | |____| | | | (_| | |_| |___| (_) | | |  __/\n");
        printf(" \\_____|     \\_____|_| |_|\\__,_|\\__|\\_____\\___/|_|  \\___|\n");

        printf("Auswahlmenü:\n");
        printf("1. Client\n");
        printf("2. Server \n");
        printf("3. Debug\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                client();
                break;
            case 2:
                server();
                break;
            case 3:
                debug();
                break;
            default:
                printf("Ungültige Auswahl, bitte erneut versuchen\n");
        }
    } while (choice != 4);

    return 0;
}
