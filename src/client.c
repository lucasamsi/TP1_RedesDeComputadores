#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "termo.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <endereco_ip> <porta>\n", argv[0]);
        return 1;
    }

    char *endereco_ip = argv[1];
    int porta = atoi(argv[2]);
    struct in_addr v4_addr;
    struct in6_addr v6_addr;

    int sockfd;

    if (inet_pton(AF_INET, endereco_ip, &v4_addr) == 1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(porta);
        inet_pton(AF_INET, endereco_ip, &addr.sin_addr);

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("connect");
            close(sockfd);
            return 1;
        }

    } else if (inet_pton(AF_INET6, endereco_ip, &v6_addr) == 1) {
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in6 addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(porta);
        inet_pton(AF_INET6, endereco_ip, &addr.sin6_addr);

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("connect");
            close(sockfd);
            return 1;
        }

    } else {
        fprintf(stderr, "Endereço IP inválido: %s\n", endereco_ip);
        return 1;
    }

    GameMessage inicio;
    recv(sockfd, &inicio, sizeof(GameMessage), 0);

    while (1) {
        printf("Insira seu palpite:\n> ");

        char entrada[10];
        fgets(entrada, sizeof(entrada), stdin);
        size_t tam = strlen(entrada);
        if (tam > 0 && entrada[tam - 1] == '\n') {
            entrada[tam - 1] = '\0';
            tam--;
        }

        int palavraint[5];
        if (tam != 5) {
            for (int i = 0; i < 5; i++) {
                palavraint[i] = 0;
            }
        } else {
            converteCase(entrada);
            paraValorNumerico(entrada, palavraint);

        }

        GameMessage tentativa;
        tentativa.type = MSG_GUESS;
        memcpy(tentativa.guess, palavraint, sizeof(palavraint));
        send(sockfd, &tentativa, sizeof(GameMessage), 0);

        GameMessage resposta;
        recv(sockfd, &resposta, sizeof(GameMessage), 0);

        if (resposta.type == MSG_ERROR) {
            printf("ERRO: Insira uma sequencia de 5 caracteres de A a Z!\n");
            continue;
        }

        if (resposta.type == MSG_WIN) {
            printf("Parabéns! Você venceu!\n");
            break;
        }

        if (resposta.type == MSG_FEEDBACK) {
            printf("Dica: ");
            for (int i = 0; i < 5; i++) {
                if (resposta.feedback[i] == 2) {
                    printf("%c ", palavraint[i]);
                } else if (resposta.feedback[i] == 1) {
                    printf("* ");
                } else {
                    printf("_ ");
                }
            }
            printf("\n");
            printf("Tentativas realizadas: %d\n", resposta.attempts);
        }
    }

    close(sockfd);
    return 0;
}