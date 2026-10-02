#include "cofre.h"

void calculaFeedback(int guess[5], int palavra[5], int feedback[5]) {
    while (1) {
        for (int i = 0; i < 5; i++) {
            if (guess[i] == palavra[i]) {
                feedback[i] = 2; // correto
            } else {
                for (int j = 0; j < 5; j++) {
                    if (guess[i] == palavra[j]) {
                        feedback[i] = 1;
                    }
                }
                feedback[i] = 0; // incorreto
            }
        }
        break;
}

void converteCase(char guess[]) {
    for (int i = 0; i < 5; i++) {
        guess[i] = toupper(guess[i]);
    }
}

void paraValorNumerico(char palavra[], int valor[5]) {
    for (int i = 0; i < 5; i++) {
        valor[i] = (int)palavra[i]; 
    }
}

int validaRange(int valor[]) {
    for (int i = 0; i < 5; i++) {
        if (valor[i] < 'A' || valor[i] > 'Z') {
            return 0;
        }
    }
    return 1;
}

void paraValorNumerico(char *palavra, int valor[5]) {
    for (int i = 0; i < 5; i++) {
        valor[i] = (int)palavra[i]; 
    }
}
