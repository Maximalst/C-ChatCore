#include <stdio.h>      // Standard Input/Output
#include <stdlib.h>     // Standardbibliothek
#include <string.h>     // Stringfunktionen
#include <unistd.h>     // close()-Funktion
#include <arpa/inet.h>  // inet_addr(), htons()
#include <sys/socket.h> // Socket-Funktionen

int client() {
    // 1. Socket erstellen
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Fehler beim Erstellen des Sockets");
        return 1;
    }

    // 2. Server-Adresse konfigurieren
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Verbindung aufbauen
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Verbindung fehlgeschlagen");
        return 1;
    }
    printf("Verbunden mit dem Server\n");

    char message[200];
    char server_reply[200];
    int recv_size;

    while (1) {
        // 4. Nachricht eingeben
        printf("Geben Sie eine Nachricht ein: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Entfernen des Zeilenumbruchs

        // 5. Nachricht senden
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Fehler beim Senden");
            return 1;
        }
        printf("Nachricht gesendet: %s\n", message);

        // 6. Antwort empfangen
        if ((recv_size = recv(sock, server_reply, sizeof(server_reply), 0)) < 0) {
            perror("Fehler beim Empfangen");
            return 1;
        }
        server_reply[recv_size] = '\0';
        printf("Antwort vom Server: %s\n", server_reply);

        // Beenden, wenn der Benutzer "exit" eingibt
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }

    // 7. Socket schließen
    close(sock);
    printf("Verbindung geschlossen\n");
    return 0;
}

int server() {
    printf("Starte Server...\n");
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;
    char client_message[200];

    // 1. Socket erstellen
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Fehler beim Erstellen des Sockets");
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
            FILE *file = fopen("messages.txt", "a");
            if (file != NULL) {
                fprintf(file, "Nachricht von %s: %s\n", inet_ntoa(client.sin_addr), client_message);
                fclose(file);
            }

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

    close(server_sock);
    return 0;
}

int main() {
    int choice;

    printf("Auswahlmenü:\n");
    printf("1. Client\n");
    printf("2. Server\n");
    printf("Bitte wählen Sie eine Option: ");
    scanf("%d", &choice);
    getchar(); // Puffer leeren

    switch (choice) {
        case 1:
            printf("Client ausgewählt.\n");
            client();
            break;
        case 2:
            printf("Server ausgewählt.\n");
            server();
            break;
        default:
            printf("Ungültige Auswahl.\n");
            break;
    }

    return 0;
}