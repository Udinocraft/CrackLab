/*
* Config.c
 * ---------------------------------------------------------
 * Guarda a dificuldade escolhida (Easy / Ok / Hard) e converte
 * em segundos. Esse tempo vale para a vitima criar a senha e
 * para a espera do modo cracker.
 * ---------------------------------------------------------
 */

#include "Config.h"

#include <stdio.h>

static Dificuldade g_dificuldade = DIF_EASY;

static const int TEMPOS[DIF_TOTAL] = { 60, 30, 10 };
static const char *const NOMES[DIF_TOTAL] = { "Easy", "Ok", "Hard" };

void config_definir_dificuldade(Dificuldade d) {
    if (d >= 0 && d < DIF_TOTAL) g_dificuldade = d;
}

Dificuldade config_dificuldade(void) { return g_dificuldade; }

int config_tempo_de(Dificuldade d) {
    return (d >= 0 && d < DIF_TOTAL) ? TEMPOS[d] : TEMPOS[DIF_EASY];
}

int config_tempo_segundos(void) { return TEMPOS[g_dificuldade]; }

const char *config_nome(Dificuldade d) {
    return (d >= 0 && d < DIF_TOTAL) ? NOMES[d] : "?";
}

void config_rotulo(Dificuldade d, char *saida, size_t tam) {
    snprintf(saida, tam, "%s (%ds)", config_nome(d), config_tempo_de(d));
}