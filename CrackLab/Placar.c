/*
 * Placar.c
 * ---------------------------------------------------------
 * Placar e historico de partidas do CrackLab.
 * Cada partida tem um lado vencedor:
 *   - senha resistiu  -> ponto da VITIMA
 *   - senha quebrada  -> ponto do CRACKER
 * Alem disso, conta as vitorias/derrotas do proprio jogador,
 * qualquer que seja o modo escolhido.
 * O historico e salvo em "cracklab_historico.txt" (uma linha
 * por partida) e recarregado quando o jogo abre.
 * ---------------------------------------------------------
 */

#include "Placar.h"
#include "Idioma.h"
#include "Config.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define ARQUIVO_HISTORICO "cracklab_historico.txt"
#define MAX_PARTIDAS 1000
#define MOSTRAR_ULTIMAS 15

static Partida g_partidas[MAX_PARTIDAS];
static int g_total = 0;

void placar_carregar(void) {
    FILE *f = fopen(ARQUIVO_HISTORICO, "r");
    if (!f) return;

    char linha[256];
    while (g_total < MAX_PARTIDAS && fgets(linha, sizeof(linha), f)) {
        Partida p;
        memset(&p, 0, sizeof(p));
        long long quando = 0;
        char usuario[64] = {0};

        int lidos = sscanf(linha, "%lld|%d|%d|%d|%d|%lu|%lf|%63[^\n]",
                           &quando, &p.modo, &p.dificuldade, &p.usuarioVenceu,
                           &p.quebrada, &p.tentativas, &p.segundos, usuario);
        if (lidos < 7) continue;
        if (p.modo < 0 || p.modo > MODO_CRACKER) continue;
        if (p.dificuldade < 0 || p.dificuldade >= DIF_TOTAL) continue;

        p.quando = quando;
        strncpy(p.usuario, usuario, sizeof(p.usuario) - 1);
        g_partidas[g_total++] = p;
    }
    fclose(f);
}

void placar_registrar(const Partida *p) {
    if (g_total == MAX_PARTIDAS) { /* descarta a mais antiga da memoria */
        memmove(&g_partidas[0], &g_partidas[1], sizeof(Partida) * (MAX_PARTIDAS - 1));
        g_total--;
    }
    g_partidas[g_total++] = *p;

    FILE *f = fopen(ARQUIVO_HISTORICO, "a");
    if (!f) return; /* sem permissao de escrita: o jogo segue so em memoria */

    char usuario[64];
    strncpy(usuario, p->usuario, sizeof(usuario) - 1);
    usuario[sizeof(usuario) - 1] = '\0';
    for (char *c = usuario; *c; c++)
        if (*c == '|' || *c == '\n' || *c == '\r') *c = '_';

    fprintf(f, "%lld|%d|%d|%d|%d|%lu|%.4f|%s\n", p->quando, p->modo, p->dificuldade,
            p->usuarioVenceu, p->quebrada, p->tentativas, p->segundos, usuario);
    fclose(f);
}

void placar_mostrar_resumo(void) {
    int pontosVitima = 0, pontosCracker = 0, vitorias = 0, derrotas = 0;

    for (int i = 0; i < g_total; i++) {
        if (g_partidas[i].quebrada) pontosCracker++; else pontosVitima++;
        if (g_partidas[i].usuarioVenceu) vitorias++; else derrotas++;
    }

    printf("\n%s\n", T(M_SB_TITLE));
    if (g_total == 0) {
        printf("%s\n", T(M_SB_EMPTY));
        return;
    }
    printf(T(M_SB_SIDES), pontosVitima, pontosCracker);
    printf("\n");
    printf(T(M_SB_YOU), vitorias, derrotas);
    printf("\n");
}

void placar_mostrar_historico(void) {
    placar_mostrar_resumo();
    if (g_total == 0) return;

    int quantas = g_total < MOSTRAR_ULTIMAS ? g_total : MOSTRAR_ULTIMAS;
    printf("\n");
    printf(T(M_HIST_TITLE), quantas);
    printf("\n");

    for (int i = g_total - 1; i >= g_total - quantas; i--) { /* mais recentes primeiro */
        const Partida *p = &g_partidas[i];
        char data[32] = "--/--/---- --:--";
        time_t t = (time_t)p->quando;
        struct tm *tm_local = localtime(&t);
        if (tm_local) strftime(data, sizeof(data), "%d/%m/%Y %H:%M", tm_local);

        char dif[32];
        config_rotulo((Dificuldade)p->dificuldade, dif, sizeof(dif));

        printf("%s | %s | %s | %s | %lu %s\n",
               data,
               p->modo == MODO_VITIMA ? T(M_MODE_VICTIM) : T(M_MODE_CRACKER),
               dif,
               p->usuarioVenceu ? T(M_RES_WON_SHORT) : T(M_RES_LOST_SHORT),
               p->tentativas, T(M_TRIES));
    }
}