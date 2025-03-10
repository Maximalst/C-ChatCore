#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int server() {
    printf("Starte Server...\n");
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;
    char client_message[200];
    FILE *file;

    // Datei öffnen
    file = fopen("messages.txt", "a");
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei");
        return 1;
    }

    // 1. Socket erstellen
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Fehler beim Erstellen des Sockets");
        fclose(file);
        return 1;
    }
    printf("Socket erstellt\n");

    // 2. Server-Adresse konfigurieren
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // 3. Bind
    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Bind fehlgeschlagen");
        fclose(file);
        return 1;
    }
    printf("Bind erfolgreich\n");

    // 4. Listen
    listen(server_sock, 3);

    // 5. Accept in einer Endlosschleife
    printf("Warte auf eingehende Verbindungen...\n");
    c = sizeof(struct sockaddr_in);
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("Accept fehlgeschlagen");
            continue; // Fehler beim Accept, nächste Iteration
        }
        printf("Verbindung akzeptiert\n");

        // 6. Client-IP anzeigen
        printf("Verbindung von IP: %s\n", inet_ntoa(client.sin_addr));

        // 7. Nachricht empfangen und antworten in einer Schleife
        while (1) {
            int read_size;
            if ((read_size = recv(client_sock, client_message, sizeof(client_message), 0)) < 0) {
                perror("Fehler beim Empfangen");
                close(client_sock);
                break; // Fehler beim Empfangen, Verbindung schließen
            }
            if (read_size == 0) {
                printf("Client hat die Verbindung geschlossen\n");
                close(client_sock);
                break; // Client hat die Verbindung geschlossen
            }
            client_message[read_size] = '\0';
            printf("Nachricht empfangen: %s\n", client_message);

            // Nachricht in Datei speichern
            fprintf(file, "Nachricht von %s: %s\n", inet_ntoa(client.sin_addr), client_message);
            fflush(file); // Sicherstellen, dass die Nachricht sofort geschrieben wird

            // Antwort senden
            char reply[] = "Hallo, Client!";
            if (send(client_sock, reply, strlen(reply), 0) < 0) {
                perror("Fehler beim Senden");
                close(client_sock);
                break; // Fehler beim Senden, Verbindung schließen
            }
            printf("Antwort gesendet: %s\n", reply);
        }

        printf("Verbindung geschlossen\n");
    }

    fclose(file);
    close(server_sock);
    return 0;
}

int main() {
    return server();
}