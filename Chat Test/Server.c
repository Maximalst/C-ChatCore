#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Socket erstellen
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket Fehler");
        exit(1);
    }

    // Server-Adresse konfigurieren
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Socket an Port binden
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Fehler");
        exit(1);
    }

    // Lauschen auf Verbindungen
    if (listen(server_fd, 1) < 0) {
        perror("Listen Fehler");
        exit(1);
    }
    printf("Server wartet auf eine Verbindung...\n");

    // Verbindung akzeptieren
    client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket < 0) {
        perror("Accept Fehler");
        exit(1);
    }
    printf("Client verbunden!\n");

    // Nachrichten empfangen
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client getrennt.\n");
            break;
        }
        printf("Client: %s\n", buffer);
    }

    close(client_socket);
    close(server_fd);
    return 0;
}
