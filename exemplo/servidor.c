#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "protocolo.h"

#define PORT 8080

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <v4|v6> <porta> <palavra>\n", argv[0]);
        return 1;
    }

    char *protocolo = argv[1];
    int porta = atoi(argv[2]);
    char *palavra = argv[3];

    // Criação do socket e configuração do endereço
    int server_fd;
    // identificador do protocolo v4 ou v6
    if (strcmp(protocolo, "v4") == 0) {
        printf("Servidor iniciado em modo IPv4 na porta %d\n", porta);

        server_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (server_fd < 0) {
            perror("socket");
        return 1;
        }

        struct sockaddr_in addr4;
        memset(&addr4, 0, sizeof(addr4));
        addr4.sin_family = AF_INET;
        addr4.sin_addr.s_addr = INADDR_ANY; // aceita conexões de qualquer endereço IPv4, como mostrado nos exemplos didaticos 
        addr4.sin_port = htons(porta);

        if (bind(server_fd, (struct sockaddr *)&addr4, sizeof(addr4)) < 0) {
            perror("bind");
            close(server_fd);
            return 1;
        }
    }
    else if (strcmp(protocolo, "v6") == 0) {
        printf("Servidor iniciado em modo IPv6 na porta %d\n", porta);

        server_fd = socket(AF_INET6, SOCK_STREAM, 0);

        if (server_fd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in6 addr6;
        memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        addr6.sin6_addr = in6addr_any; // aceita conexões de qualquer endereço IPv6
        addr6.sin6_port = htons(porta);

        if (bind(server_fd, (struct sockaddr *)&addr6, sizeof(addr6)) < 0) {
            perror("bind");
            close(server_fd);
            return 1;
        }
    } else {
        fprintf(stderr, "Protocolo inválido. Use 'v4' ou 'v6'.\n");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    int client_fd = accept(server_fd, NULL, NULL);

    GameMessage msg;
    recv(client_fd, &msg, sizeof(msg), 0);

    if (msg.type == MSG_GUESS) {
        int feedback[5];
        calculate_feedback(msg.guess, palavra, int feedback);

        GameMessage feedbackmsg;
        feedbackmsg.type = MSG_FEEDBACK;
        send(client_fd, &feedbackmsg, sizeof(GameMessage), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}