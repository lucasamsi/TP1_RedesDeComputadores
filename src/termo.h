#ifndef TERMO_H
#define TERMO_H
#define MSG_SIZE 128

typedef enum {
    MSG_START,
    MSG_GUESS,
    MSG_FEEDBACK,
    MSG_WIN,
    MSG_ERROR,
    MSG_EXIT
} MessageType;

typedef struct {
    int type;
    int guess[5];
    int feedback[5];
    int attempts;
    int win_status;
    char message[MSG_SIZE];
} GameMessage;

void calculaFeedback(int guess[5], int palavra[5], int feedback[5]);
void converteCase(char guess[]);
void paraValorNumerico(char palavra[], int valor[5]);
int validaRange(int valor[]);

#endif 