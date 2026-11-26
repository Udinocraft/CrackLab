#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

/* Niveis de dificuldade: definem o tempo do jogo. */
typedef enum {
    DIF_EASY,   /* 60 segundos */
    DIF_OK,     /* 30 segundos */
    DIF_HARD,   /* 10 segundos */
    DIF_TOTAL
} Dificuldade;

void config_definir_dificuldade(Dificuldade d);
Dificuldade config_dificuldade(void);

/* Tempo em segundos de uma dificuldade / da dificuldade atual. */
int config_tempo_de(Dificuldade d);
int config_tempo_segundos(void);

/* Nome fixo ("Easy", "Ok", "Hard") e rotulo com tempo ("Easy (60s)"). */
const char *config_nome(Dificuldade d);
void config_rotulo(Dificuldade d, char *saida, size_t tam);

#endif /* CONFIG_H */