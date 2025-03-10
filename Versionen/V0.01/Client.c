#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Socket erstellen
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket Fehler");
        exit(1);
    }

    // Server-Adresse konfigurieren
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.178.148"); // Server-IP anpassen

    // Verbindung zum Server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Verbindungsfehler");
        exit(1);
    }
    printf("Verbunden mit Server\n");

    // Nachrichten senden
    while (1) {
        printf("Du: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Newline entfernen

        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}
