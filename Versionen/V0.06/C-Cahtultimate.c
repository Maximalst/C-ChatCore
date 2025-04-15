#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 4444
#define MAX_CLIENTS 10
#define BUF_SIZE 2048

typedef struct {
    int sock;
    struct sockaddr_in addr;
    char ip[INET_ADDRSTRLEN];
} client_t;

client_t* clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
char key[] = "my_secret_key_1234"; // XOR key

// XOR-Verschlüsselung
void xor_crypt(char* data, int len) {
    for (int i = 0; i < len; i++) {
        data[i] ^= key[i % strlen(key)];
    }
}

void add_client(client_t* cl) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i]) {
            clients[i] = cl;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && clients[i]->sock == sock) {
            close(clients[i]->sock);
            free(clients[i]);
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast(int sender_sock, char* msg, int len) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && clients[i]->sock != sender_sock) {
            send(clients[i]->sock, msg, len, 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* handle_client(void* arg) {
    client_t* cli = (client_t*)arg;
    char buffer[BUF_SIZE];

    while (1) {
        int len = recv(cli->sock, buffer, BUF_SIZE, 0);
        if (len <= 0) break;

        xor_crypt(buffer, len);  // Entschlüsseln
        buffer[len] = '\0';
        printf("[Client %s] %s\n", cli->ip, buffer);

        xor_crypt(buffer, len);  // Wieder verschlüsseln
        broadcast(cli->sock, buffer, len);
    }

    remove_client(cli->sock);
    return NULL;
}

void start_server() {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(PORT) };
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, MAX_CLIENTS);
    printf("🟢 Server läuft auf Port %d...\n", PORT);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int client_sock = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

        client_t* cli = malloc(sizeof(client_t));
        cli->sock = client_sock;
        cli->addr = cli_addr;
        inet_ntop(AF_INET, &cli_addr.sin_addr, cli->ip, sizeof(cli->ip));
        add_client(cli);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, cli);
    }
}

void* receive_handler(void* socket_ptr) {
    int sock = *(int*)socket_ptr;
    char buffer[BUF_SIZE];

    while (1) {
        int len = recv(sock, buffer, BUF_SIZE, 0);
        if (len <= 0) break;
        xor_crypt(buffer, len);
        buffer[len] = '\0';
        printf("📥 Nachricht: %s\n", buffer);
    }
    return NULL;
}

void start_client() {
    char ip[100];
    printf("🌍 Server-IP: ");
    scanf("%99s", ip);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_port = htons(PORT) };
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Verbindung fehlgeschlagen");
        exit(1);
    }

    printf("✅ Verbunden mit %s:%d\n", ip, PORT);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, &sock);

    char input[BUF_SIZE];
    getchar(); // Consume leftover \n
    while (1) {
        printf("Du: ");
        fgets(input, BUF_SIZE, stdin);
        if (strlen(input) <= 1) continue;

        int len = strlen(input);
        xor_crypt(input, len);
        send(sock, input, len, 0);
    }
}

void menu() {
    int choice;
    do {
        printf("\n==== 🧠 Chat Menü ====\n");
        printf("1. Server starten\n");
        printf("2. Client starten\n");
        printf("0. Beenden\n");
        printf("Auswahl: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: start_server(); break;
            case 2: start_client(); break;
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
