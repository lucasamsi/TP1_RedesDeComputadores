#include <string.h>
#include <ctype.h>

void calculaFeedback(int guess[5], int palavra[5], int feedback[5]) {
    int incidencia[26] = {0};
    
    for (int s = 0; s < 5; s++){
        feedback[s] = 0;
    }

    for (int g = 0; g < 5; g++){
        incidencia[palavra[g] - 'A']++;
    }
    
    for (int i = 0; i < 5; i++) {
        if (guess[i] == palavra[i]) {
            feedback[i] = 2;
            incidencia[palavra[i] - 'A']--;
        }
    }
    
    
    for (int k = 0; k < 5; k++){
        if ((incidencia[guess[k] - 'A']) != 0){
            if (feedback[k] != 2) {
                feedback[k] = 1;
                incidencia[guess[k] - 'A']--;
            }
        }
    }

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