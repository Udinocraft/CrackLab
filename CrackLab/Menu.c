/*
 * Menu.c
 * ---------------------------------------------------------
 * Unico arquivo do projeto com main(). Porta de entrada do
 * CrackLab: menu principal, escolha de dificuldade e idioma,
 * placar/historico e os dois modos de jogo (Vitima e Cracker).
 * ---------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Perfil.h"
#include "Bruteforce.h"
#include "Config.h"
#include "Idioma.h"
#include "Placar.h"

#define OPCAO_INVALIDA (-1)
#define OPCAO_EOF      (-2)

typedef enum { ACAO_REPETIR, ACAO_JOGAR_VITIMA, ACAO_VOLTAR } Acao;

static void limpar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void aguardar_enter(void) {
    printf("\n%s", T(M_PRESS_ENTER));
    fflush(stdout);
    getchar();
}

/* Le um numero do teclado. Devolve OPCAO_INVALIDA ou OPCAO_EOF em caso de erro. */
static int ler_opcao(void) {
    int opcao;
    int lido = scanf("%d", &opcao);
    if (lido == EOF) return OPCAO_EOF;
    limpar_buffer_entrada();
    if (lido != 1) return OPCAO_INVALIDA;
    return opcao;
}

/* Registra a partida no historico e mostra o placar atualizado. */
static void registrar_partida(Modo modo, const Perfil *p, int usuarioVenceu, int quebrada,
                              const ResultadoAtaque *r) {
    Partida reg;
    memset(&reg, 0, sizeof(reg));
    reg.quando = (long long)time(NULL);
    reg.modo = modo;
    reg.dificuldade = config_dificuldade();
    reg.usuarioVenceu = usuarioVenceu;
    reg.quebrada = quebrada;
    if (r) { reg.tentativas = r->tentativas; reg.segundos = r->segundosGastos; }
    strncpy(reg.usuario, p->usuario[0] ? p->usuario : "-", sizeof(reg.usuario) - 1);
    placar_registrar(&reg);
    placar_mostrar_resumo();
}

/* ------------------------- MODO VITIMA ------------------------- */
/* Devolve 1 se o jogador venceu. */
static int jogar_vitima(void) {
    printf("\n");
    Perfil p = modo_vitima();

    if (!p.sucesso) {
        printf("\n%s\n", T(M_VIC_TIMEOUT));
        printf("%s\n", T(M_RES_LOSE));
        registrar_partida(MODO_VITIMA, &p, 0, 1, NULL); /* tempo esgotado: ponto do cracker */
        return 0;
    }

    printf("\n%s\n", T(M_VIC_CREATED_ATTACK));
    aguardar_enter();

    ResultadoAtaque r = atacar(p.usuario, p.hash);

    if (r.quebrada) {
        printf("\n%s\n", T(M_VIC_CRACKED));
        printf("%s\n", T(M_RES_LOSE));
    } else {
        printf("\n%s\n", T(M_VIC_RESISTED));
        printf("%s\n", T(M_RES_WIN));
    }
    registrar_partida(MODO_VITIMA, &p, !r.quebrada, r.quebrada, &r);
    return !r.quebrada;
}

/* ------------------------- MODO CRACKER ------------------------- */
static int jogar_invasor(void) {
    printf("\n");
    Perfil p = modo_invasor();

    printf("\n%s\n", T(M_CRK_DONE));
    aguardar_enter();

    ResultadoAtaque r = atacar(p.usuario, p.hash);

    if (r.quebrada) {
        printf("\n%s\n", T(M_CRK_CRACKED));
        printf("%s\n", T(M_RES_WIN));
    } else {
        printf("\n%s\n", T(M_CRK_RESISTED));
        printf("%s\n", T(M_RES_LOSE));
    }
    registrar_partida(MODO_CRACKER, &p, r.quebrada, r.quebrada, &r);
    return r.quebrada;
}

/* --------------------- SUBMENUS DE FIM DE JOGO --------------------- */
static Acao menu_pos_derrota(void) {
    while (1) {
        printf("\n%s\n%s\n%s\n%s\n", T(M_POST_LOSE_Q), T(M_POST_RETRY),
               T(M_POST_AS_VICTIM), T(M_POST_BACK));
        printf("%s", T(M_CHOICE));

        int op = ler_opcao();
        if (op == OPCAO_EOF) return ACAO_VOLTAR;
        if (op == 1) return ACAO_REPETIR;
        if (op == 2) return ACAO_JOGAR_VITIMA;
        if (op == 3) return ACAO_VOLTAR;
        printf("%s\n", T(M_INVALID));
    }
}

static Acao menu_pos_vitoria(void) {
    while (1) {
        printf("\n%s\n%s\n%s\n", T(M_POST_WIN_Q), T(M_POST_AGAIN), T(M_POST_WIN_BACK));
        printf("%s", T(M_CHOICE));

        int op = ler_opcao();
        if (op == OPCAO_EOF) return ACAO_VOLTAR;
        if (op == 1) return ACAO_REPETIR;
        if (op == 2) return ACAO_VOLTAR;
        printf("%s\n", T(M_INVALID));
    }
}

/* Roda partidas em loop (sem recursao) ate o jogador voltar ao menu. */
static void executar_modo(Modo modo) {
    while (1) {
        int venceu = (modo == MODO_VITIMA) ? jogar_vitima() : jogar_invasor();
        Acao acao = venceu ? menu_pos_vitoria() : menu_pos_derrota();

        if (acao == ACAO_VOLTAR) return;
        if (acao == ACAO_JOGAR_VITIMA) modo = MODO_VITIMA;
        /* ACAO_REPETIR: mantem o mesmo modo */
    }
}

/* ---------------------- DIFICULDADE E IDIOMA ---------------------- */
static void menu_dificuldade(void) {
    char rotulo[32];

    printf("\n%s\n%s\n", T(M_DIFF_TITLE), T(M_DIFF_HELP));
    for (int d = 0; d < DIF_TOTAL; d++) {
        config_rotulo((Dificuldade)d, rotulo, sizeof(rotulo));
        printf("%d. %s\n", d + 1, rotulo);
    }
    printf("%s", T(M_CHOICE));

    int op = ler_opcao();
    if (op >= 1 && op <= DIF_TOTAL) {
        config_definir_dificuldade((Dificuldade)(op - 1));
        config_rotulo(config_dificuldade(), rotulo, sizeof(rotulo));
        printf(T(M_DIFF_SET), rotulo);
        printf("\n");
    } else if (op != OPCAO_EOF) {
        printf("%s\n", T(M_INVALID));
    }
}

static void menu_idioma(void) {
    printf("\n%s\n", T(M_LANG_TITLE));
    for (int i = 0; i < IDI_TOTAL; i++)
        printf("%d. %s\n", i + 1, idioma_nome((Idioma)i));
    printf("%s", T(M_CHOICE));

    int op = ler_opcao();
    if (op >= 1 && op <= IDI_TOTAL) {
        idioma_definir((Idioma)(op - 1));
        printf(T(M_LANG_SET), idioma_nome(idioma_atual()));
        printf("\n");
    } else if (op != OPCAO_EOF) {
        printf("%s\n", T(M_INVALID));
    }
}

/* ------------------------------ MAIN ------------------------------ */
int main(void) {
    placar_carregar();

    while (1) {
        char rotuloDif[32];
        config_rotulo(config_dificuldade(), rotuloDif, sizeof(rotuloDif));

        printf("\n==========================================\n");
        printf("   %s\n", T(M_MENU_TITLE));
        printf("==========================================\n");
        placar_mostrar_resumo();
        printf("------------------------------------------\n");
        printf("%s\n", T(M_MENU_VICTIM));
        printf("%s\n", T(M_MENU_CRACKER));
        printf(T(M_MENU_DIFF), rotuloDif);                          printf("\n");
        printf(T(M_MENU_LANG), idioma_nome(idioma_atual()));        printf("\n");
        printf("%s\n", T(M_MENU_HISTORY));
        printf("%s\n", T(M_MENU_EXIT));
        printf("%s", T(M_CHOICE));

        int op = ler_opcao();
        if (op == OPCAO_EOF) { printf("\n%s\n", T(M_INPUT_END)); break; }

        if (op == 1) executar_modo(MODO_VITIMA);
        else if (op == 2) executar_modo(MODO_CRACKER);
        else if (op == 3) menu_dificuldade();
        else if (op == 4) menu_idioma();
        else if (op == 5) { placar_mostrar_historico(); aguardar_enter(); }
        else if (op == 6) { printf("\n%s\n", T(M_BYE)); break; }
        else printf("%s\n", T(M_INVALID));
    }

    return 0;
}