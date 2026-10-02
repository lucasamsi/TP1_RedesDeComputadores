#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "protocolo.h"
#include <stdlib.h>

#define PORT 8080

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
            return 1;
        }

    } else {
        fprintf(stderr, "Endereço IP inválido: %s\n", endereco_ip);
        return 1;
    }


    
    Pacote pedido;
    pedido.tipo = PED_SOMA;
    pedido.a = 3;
    pedido.b = 4;
    send(sockfd, &pedido, sizeof(Pacote), 0);   // manda a struct inteira

    Pacote resposta;
    recv(sockfd, &resposta, sizeof(Pacote), 0);  // recebe a struct inteira de volta

    if (resposta.tipo == RESP_SOMA) {
        printf("Resultado: %d\n", resposta.resultado);
    }

    close(sockfd);
    return 0;
}