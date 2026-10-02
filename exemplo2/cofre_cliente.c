#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "cofre.h"

int main(int argc, char *argv[]) {
    // suponha que sockfd já foi criado e conectado aqui em cima,
    // igual ao seu client.c (com a detecção v4/v6 que você já fez)
    int sockfd;
    // ... socket() + connect() já prontos ...

    Mensagem inicio;
    recv(sockfd, &inicio, sizeof(Mensagem), 0);   // recebe o MSG_INICIO do servidor

    int jogando = 1;

    while (jogando) {
        printf("Digite sua tentativa (5 dígitos): ");

        int tentativa_usuario[5];
        for (int i = 0; i < 5; i++) {
            scanf("%d", &tentativa_usuario[i]);
        }

        Mensagem envio;
        envio.tipo = MSG_TENTATIVA;
        memcpy(envio.codigo, tentativa_usuario, sizeof(tentativa_usuario));

        send(sockfd, &envio, sizeof(Mensagem), 0);

        Mensagem resposta;
        recv(sockfd, &resposta, sizeof(Mensagem), 0);

        if (resposta.tipo == MSG_VENCEU) {
            printf("Você venceu!\n");
            jogando = 0;

        } else if (resposta.tipo == MSG_RESULTADO) {
            printf("Resultado: ");
            for (int i = 0; i < 5; i++) {
                printf("%d ", resposta.resultado[i]);
            }
            printf("\n");
        }
    }

    close(sockfd);
    return 0;
}