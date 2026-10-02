#include <string.h>
#include <ctype.h>

// converte para maiusculo e checa se tem tamanho 5
int converte_case(char palavraChute[]) {
    if (strlen(palavraChute) == 5) {
        for (int i = 0; i < 5; i++) {
        palavraChute[i] = toupper(palavraChute[i]);
        }
        return 1;
    } else {
        return 0;
    }
}

// ve se  a palavra esta entre A e Z (seus valores)
int valida_range(char palavraChute[]) {
    for (int i = 0; i < 5; i++) {
        if (palavraChute[i] < 'A' || palavraChute[i] > 'Z') {
            return 0;
        }
    }
    return 1;
}

void calcula_feedback(int guess[5], int palavra[5], int feedback[5]) {

}

int main