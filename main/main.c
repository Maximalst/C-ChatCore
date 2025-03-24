#include <stdio.h>

int Server() {
    printf("Server\n");
    return 0;
}






int main() {
    int mode;
    printf("\n\n\n\n\n\n\n\n\n");
        printf("  _____       _____ _           _    _____               \n");
        printf(" / ____|     / ____| |         | |  / ____|              \n");
        printf("| |   ______| |    | |__   __ _| |_| |     ___  _ __ ___ \n");
        printf("| |  |______| |    | '_ \\ / _` | __| |    / _ \\| '__/ _ \\\n");
        printf("| |____     | |____| | | | (_| | |_| |___| (_) | | |  __/\n");
        printf(" \\_____|     \\_____|_| |_|\\__,_|\\__|\\_____\\___/|_|  \\___|\n");
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