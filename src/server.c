#include "termo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {

    // checar se ha 4 argumentos (programa, protocolo, porta, palavra) termos
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <protocolo> <porta> <palavra>\n", argv[0]);
        return 1;
    }
    // checar se a palavra tem exatamente 5 caracteres (para evitar menores)
    if (strlen(argv[3]) != 5) {
        fprintf(stderr, "A palavra deve ter exatamente 5 caracteres.\n");
        return 1;
    }
    // attribuição das variáveis de entrada
    char *protocolo = argv[1];
    int porta = atoi(argv[2]);
    char *palavraChar = argv[3];
    int palavra[5];
    int contagem = 0;

    converteCase(palavraChar);
    paraValorNumerico(palavraChar, palavra);
    
    // socket servidor
    int server_fd;
    // conexão ipv4 ou ipv6
    if (strcmp(protocolo, "v4") == 0) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in addr4;
        memset(&addr4, 0, sizeof(addr4));
        addr4.sin_family = AF_INET;
        addr4.sin_addr.s_addr = INADDR_ANY;
        addr4.sin_port = htons(porta);
        printf("Servidor iniciado em modo IPv4 na porta %d\n", porta);

        if (bind(server_fd, (struct sockaddr *)&addr4, sizeof(addr4)) < 0) {
            perror("bind");
            close(server_fd);
            return 1;
        }

    } else if (strcmp(protocolo, "v6") == 0) {
        server_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in6 addr6;
        memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        addr6.sin6_addr = in6addr_any;
        addr6.sin6_port = htons(porta);
        printf("Servidor iniciado em modo IPv6 na porta %d\n", porta);


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

    //socket cliente
    int client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    printf("Cliente conectado.\n");

    // logica do jogo
    GameMessage inicio;
    inicio.type = MSG_START;
    send(client_fd, &inicio, sizeof(GameMessage), 0);

    while (1){
        GameMessage recebida;
        recv(client_fd, &recebida, sizeof(GameMessage), 0);

        if (recebida.type == MSG_GUESS) {
            int resultado[5];

            GameMessage erro;
            int dentroAZ = validaRange(recebida.guess);
            if (!dentroAZ) {
                erro.type = MSG_ERROR;
                erro.win_status = -1;
                send(client_fd, &erro, sizeof(GameMessage), 0);
                continue;
            }

            // implementa a contagem 
            contagem++;
            
            calculaFeedback(recebida.guess, palavra, resultado);

            int somaResultado = 0;
            for (int i = 0; i < 5; i++) {
                somaResultado += resultado[i];
            }

            if (somaResultado == 10) {
                GameMessage resposta;
                resposta.type = MSG_WIN;
                resposta.attempts = contagem;
                resposta.win_status = 1;
                send(client_fd, &resposta, sizeof(GameMessage), 0);
                close(client_fd);
                printf("Cliente desconectado.\n");
                break;

            } else {
                GameMessage resposta;
                resposta.type = MSG_FEEDBACK;
                resposta.attempts = contagem;
                resposta.win_status = 0;
                memcpy(resposta.feedback, resultado, sizeof(resultado));
                send(client_fd, &resposta, sizeof(GameMessage), 0);
            }      
        }
    }
    
    

    close(server_fd);
    return 0;
}