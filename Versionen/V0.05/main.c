#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h> // Für Multithreading

// Config
int port = 8080; // port to listen on server

// Config end

// Funktion, die von jedem Thread ausgeführt wird
void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg); // Speicher freigeben, da der Socket-Wert kopiert wurde

    // Hier kannst du mit dem Client kommunizieren
    char buffer[1024];
    int bytes_read;

    printf("Client connected. Thread ID: %lu\n", (unsigned long)pthread_self());

    while ((bytes_read = read(client_sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminieren
        printf("Received from client: %s\n", buffer);

        // Antwort an den Client senden
        write(client_sock, "Message received\n", 18);
    }

    if (bytes_read == 0) {
        printf("Client disconnected. Thread ID: %lu\n", (unsigned long)pthread_self());
    } else {
        perror("Read error");
    }

    close(client_sock); // Verbindung schließen
    return NULL;
}

int Server() {
    int sockfd, client_sock;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t addr_len = sizeof(clientaddr);

    // Socket erstellen
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket creation failed");
        return -1;
    }

    // Server-Informationen füllen
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Socket binden
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind failed");
        close(sockfd);
        return -1;
    }

    // Auf Verbindungen warten
    if (listen(sockfd, 5) == -1) {
        perror("listen failed");
        close(sockfd);
        return -1;
    }

    printf("Server is listening on port %d\n", port);

    while (1) {
        // Verbindung akzeptieren
        client_sock = accept(sockfd, (struct sockaddr *)&clientaddr, &addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }

        printf("Verbindung akzepteren");
        // Neuen Thread für den Client erstellen
        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int)); // Speicher für den Socket-Wert reservieren
        *new_sock = client_sock;

        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) != 0) {
            perror("pthread_create failed");
            close(client_sock);
            free(new_sock);
        }

        // Thread-Detachment, damit Ressourcen automatisch freigegeben werden
        pthread_detach(thread_id);
    }


    close(sockfd);
    return 0;
}

int Client() {
    printf("Start Client\n");
    return 0;
}

int Debug() {
    printf("Start Debug\n");
    // Debugging tool: Print server configuration and status
    printf("Debugging Tool:\n");
    printf("Server Configuration:\n");
    printf("Port: %d\n", port);

    // Check if the server socket can be created and bound
    int debug_sockfd;
    struct sockaddr_in debug_servaddr;

    if ((debug_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Debug: Socket creation failed");
    } else {
        printf("Debug: Socket creation successful\n");
    }

    debug_servaddr.sin_family = AF_INET;
    debug_servaddr.sin_addr.s_addr = INADDR_ANY;
    debug_servaddr.sin_port = htons(port);

    if (bind(debug_sockfd, (const struct sockaddr *)&debug_servaddr, sizeof(debug_servaddr)) == -1) {
        perror("Debug: Bind failed");
    } else {
        printf("Debug: Bind successful\n");
    }

    // Test sending a message to a client
    printf("Debug: Testing message sending...\n");
    int test_client_sock;
    struct sockaddr_in test_clientaddr;

    test_clientaddr.sin_family = AF_INET;
    test_clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    test_clientaddr.sin_port = htons(port);

    if ((test_client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Debug: Test client socket creation failed");
    } else if (connect(test_client_sock, (struct sockaddr *)&test_clientaddr, sizeof(test_clientaddr)) == -1) {
        perror("Debug: Test client connection failed");
    } else {
        char *test_message = "Test message from debug tool";
        if (write(test_client_sock, test_message, strlen(test_message)) == -1) {
            perror("Debug: Test message sending failed");
        } else {
            printf("Debug: Test message sent successfully\n");
        }
        close(test_client_sock);
    }

    close(debug_sockfd);
    printf("Debugging completed.\n");
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
            Client();
            break;
        case 3:
            printf("Debug Starting...\n");
            Debug();
            break;
        default:
            printf("Number is not 1, 2, or 3\n");
            break;
    }

    return 0;
}