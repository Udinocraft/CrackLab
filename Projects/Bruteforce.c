/*
 * bruteforce.c
 * ---------------------------------------------------------
 * Motor de forca bruta do jogo "Vitima vs Invasor".
 * Sem main() de proposito: este arquivo e um MODULO, chamado
 * diretamente por menu.c (unico arquivo com main() no projeto).
 * Nada aqui usa system(), fork nem arquivos em disco - tudo
 * acontece na memoria do mesmo processo, ideal para rodar
 * direto no CLion.
 * Funcao publica: atacar(usuario, hashAlvo) -> ResultadoAtaque
 * ---------------------------------------------------------
 */

#include "Bruteforce.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TAM_MAX_CANDIDATO 20
#define LIMITE_TEMPO_EXECUCAO 20.0 /* segundos reais max. por tamanho testado de verdade */

const char CHARSET[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "!@#$%^&*()-_=+[]{};:,.<>?/|~";

unsigned long hash_senha(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

static void formatar_tempo(double segundos, char *saida, size_t tam) {
    const double MINUTO = 60.0, HORA = 3600.0, DIA = 86400.0, ANO = 365.25 * DIA;
    if (segundos < MINUTO)      snprintf(saida, tam, "%.2f segundos", segundos);
    else if (segundos < HORA)   snprintf(saida, tam, "%.2f minutos", segundos / MINUTO);
    else if (segundos < DIA)    snprintf(saida, tam, "%.2f horas", segundos / HORA);
    else if (segundos < ANO)    snprintf(saida, tam, "%.2f dias", segundos / DIA);
    else                        snprintf(saida, tam, "%.3e anos", segundos / ANO);
}

/* Tenta de verdade, uma por uma, todas as combinacoes de tamanho L. */
static int forcar_tamanho(int L, unsigned long hashAlvo, int tamanhoCharset,
                           unsigned long *tentativas, char *achada, double *segundosGastos) {
    int indices[TAM_MAX_CANDIDATO] = {0};
    char candidato[TAM_MAX_CANDIDATO + 1];
    clock_t inicio = clock();

    while (1) {
        for (int i = 0; i < L; i++) candidato[i] = CHARSET[indices[i]];
        candidato[L] = '\0';
        (*tentativas)++;

        if (hash_senha(candidato) == hashAlvo) {
            strcpy(achada, candidato);
            *segundosGastos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            return 1;
        }

        int pos = L - 1;
        while (pos >= 0) {
            indices[pos]++;
            if (indices[pos] < tamanhoCharset) break;
            indices[pos] = 0;
            pos--;
        }
        if (pos < 0) break; /* esgotou todas as combinacoes deste tamanho */
    }

    *segundosGastos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    return 0;
}

ResultadoAtaque atacar(const char *usuario, unsigned long hashAlvo) {
    ResultadoAtaque resultado = {0};
    int tamanhoCharset = (int)strlen(CHARSET);

    printf("=== ATAQUE DE FORCA BRUTA - TODA A FORCA DISPONIVEL ===\n");
    printf("Alvo: usuario \"%s\"\n\n", usuario);

    double taxaEstimada = 3000000.0; /* chute inicial, recalibrado com dados reais */
    unsigned long tentativasTotais = 0;
    clock_t inicioGeral = clock();
    char achada[TAM_MAX_CANDIDATO + 1] = {0};

    for (int L = 1; L <= TAM_MAX_CANDIDATO; L++) {
        double totalCombinacoes = pow((double)tamanhoCharset, (double)L);
        double tempoEstimado = totalCombinacoes / taxaEstimada;

        char bufTempo[64];
        formatar_tempo(tempoEstimado, bufTempo, sizeof(bufTempo));
        printf("Tamanho %2d | combinacoes: %.3e | tempo estimado: %s\n", L, totalCombinacoes, bufTempo);

        if (tempoEstimado > LIMITE_TEMPO_EXECUCAO) {
            printf("\n>>> Mesmo com toda a forca disponivel, quebrar uma senha deste\n");
            printf(">>> tamanho levaria aproximadamente %s.\n", bufTempo);
            printf(">>> Ataque abortado: inviavel na pratica.\n\n");
            printf("#####################################################\n");
            printf("RESULTADO TECNICO: A SENHA RESISTIU AO ATAQUE.\n");
            printf("#####################################################\n");
            resultado.quebrada = 0;
            resultado.tentativas = tentativasTotais;
            resultado.segundosGastos = (double)(clock() - inicioGeral) / CLOCKS_PER_SEC;
            return resultado;
        }

        unsigned long antes = tentativasTotais;
        double segundosGastos = 0.0;
        int encontrado = forcar_tamanho(L, hashAlvo, tamanhoCharset, &tentativasTotais, achada, &segundosGastos);

        unsigned long tentativasDesteTamanho = tentativasTotais - antes;
        if (segundosGastos > 0.001 && tentativasDesteTamanho > 0)
            taxaEstimada = tentativasDesteTamanho / segundosGastos; /* recalibra com velocidade real */

        if (encontrado) {
            double segundosTotais = (double)(clock() - inicioGeral) / CLOCKS_PER_SEC;
            printf("\n>>> Senha encontrada: \"%s\"\n", achada);
            printf(">>> Tentativas: %lu | Tempo real: %.4f segundos\n", tentativasTotais, segundosTotais);
            printf("\n#####################################################\n");
            printf("RESULTADO TECNICO: A SENHA FOI QUEBRADA.\n");
            printf("#####################################################\n");
            resultado.quebrada = 1;
            strncpy(resultado.senhaEncontrada, achada, sizeof(resultado.senhaEncontrada) - 1);
            resultado.tentativas = tentativasTotais;
            resultado.segundosGastos = segundosTotais;
            return resultado;
        }
    }

    printf("\nBusca esgotada ate o tamanho maximo testado sem sucesso.\n");
    resultado.quebrada = 0;
    resultado.tentativas = tentativasTotais;
    return resultado;
}