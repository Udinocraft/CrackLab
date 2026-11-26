/*
 * Perfil.c
 * ---------------------------------------------------------
 * Modulo de criacao de perfil/senha do CrackLab.
 * Sem main() de proposito: e chamado por Menu.c.
 * O tempo limite vem da dificuldade escolhida (Config.h) e
 * todos os textos vem de Idioma.h.
 * ---------------------------------------------------------
 */

#include "Perfil.h"
#include "Bruteforce.h"
#include "Config.h"
#include "Idioma.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#define TAM_MAX 64

/* Le uma linha do teclado mostrando uma contagem regressiva ate
   'prazoFinal'. Retorna 1 se leu algo a tempo, 0 se o tempo
   esgotou antes de um ENTER. */
static int ler_com_cronometro(const char *rotulo, char *buffer, size_t tamanho, time_t prazoFinal) {
    while (1) {
        int restante = (int)(prazoFinal - time(NULL));
        if (restante <= 0) {
            printf("%s", T(M_TIME_UP));
            return 0;
        }

        printf(T(M_CLOCK), restante, rotulo);
        fflush(stdout);

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv = {1, 0}; /* verifica a cada 1s, atualizando o relogio */

        int pronto = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (pronto > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(buffer, (int)tamanho, stdin) == NULL) return 0;
            buffer[strcspn(buffer, "\n")] = 0;
            printf("\n");
            return 1;
        }
    }
}

static void avaliar_forca(const char *senha) {
    int len = (int)strlen(senha);
    int minus = 0, maius = 0, digito = 0, simbolo = 0;

    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)senha[i];
        if (islower(c)) minus = 1;
        else if (isupper(c)) maius = 1;
        else if (isdigit(c)) digito = 1;
        else simbolo = 1;
    }

    int variedade = minus + maius + digito + simbolo;
    int pontuacao = 0;
    if (len >= 12) pontuacao += 2;
    else if (len >= 8) pontuacao += 1;
    pontuacao += variedade;

    const char *classificacao = (pontuacao <= 2) ? T(M_WEAK) : (pontuacao <= 4) ? T(M_MEDIUM) : T(M_STRONG);

    printf("\n%s\n", T(M_EV_TITLE));
    printf(T(M_EV_LEN), len);                                    printf("\n");
    printf(T(M_EV_CASE), minus ? T(M_YES) : T(M_NO), maius ? T(M_YES) : T(M_NO));   printf("\n");
    printf(T(M_EV_NUMSYM), digito ? T(M_YES) : T(M_NO), simbolo ? T(M_YES) : T(M_NO)); printf("\n");
    printf(T(M_EV_CLASS), classificacao);                        printf("\n");
    printf("================================\n");
}

/* ---------------------- MODO VITIMA ---------------------- */
Perfil modo_vitima(void) {
    Perfil p = {0};
    char usuario[64];
    const int tempo = config_tempo_segundos();

    printf("%s\n", T(M_VIC_TITLE));
    printf("%s", T(M_VIC_ASK_USER));
    fflush(stdout);
    if (fgets(usuario, sizeof(usuario), stdin) == NULL) {
        p.sucesso = 0;
        return p;
    }
    usuario[strcspn(usuario, "\n")] = 0;
    if (strlen(usuario) == 0) strncpy(usuario, T(M_DEFAULT_USER), sizeof(usuario) - 1);
    strncpy(p.usuario, usuario, sizeof(p.usuario) - 1); /* ja guardado: aparece no historico mesmo se o tempo acabar */

    printf(T(M_VIC_TIMER_INTRO), tempo);
    printf("\n%s", T(M_PRESS_START));
    fflush(stdout);
    getchar();

    time_t prazo = time(NULL) + tempo;

    char senha[TAM_MAX];
    if (!ler_com_cronometro(T(M_PASSWORD), senha, sizeof(senha), prazo) || strlen(senha) == 0) {
        p.sucesso = 0;
        return p;
    }

    avaliar_forca(senha);

    p.hash = hash_senha(senha);
    p.sucesso = 1;

    printf("\n%s\n", T(M_PROFILE_CREATED));
    return p;
}

/* ---------------------- MODO CRACKER ---------------------- */

/* Gera uma senha aleatoria para o bot. A forca varia a cada
   partida para dar replay ao jogo. */
static void gerar_senha_bot(char *saida) {
    int tamanhoCharset = (int)strlen(CHARSET);
    int sorteio = rand() % 100;
    int tamanho;

    if (sorteio < 30)      tamanho = 3 + rand() % 3;   /* 3-5  caracteres -> fraca */
    else if (sorteio < 70) tamanho = 6 + rand() % 3;   /* 6-8  caracteres -> media */
    else                   tamanho = 10 + rand() % 7;  /* 10-16 caracteres -> forte */

    for (int i = 0; i < tamanho; i++)
        saida[i] = CHARSET[rand() % tamanhoCharset];
    saida[tamanho] = '\0';
}

static void mostrar_manual(void) {
    printf("\n%s\n", T(M_MANUAL_TITLE));
    printf(T(M_MANUAL_BODY), (int)strlen(CHARSET));
    printf("===========================================================\n\n");
}

Perfil modo_invasor(void) {
    Perfil p = {0};
    const int tempo = config_tempo_segundos();

    printf("%s\n", T(M_CRK_TITLE));
    printf(T(M_CRK_INTRO), tempo);
    printf("\n%s", T(M_CRK_WAIT_START));
    fflush(stdout);
    getchar();

    srand((unsigned int)time(NULL));
    char senhaBot[32];
    gerar_senha_bot(senhaBot);

    time_t prazo = time(NULL) + tempo;
    while (1) {
        int restante = (int)(prazo - time(NULL));
        if (restante <= 0) {
            printf("%s", T(M_CRK_BOT_DONE_BAR));
            break;
        }

        printf(T(M_CRK_WAITING), restante);
        fflush(stdout);

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv = {1, 0};

        int pronto = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (pronto > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            char linha[32];
            if (fgets(linha, sizeof(linha), stdin)) {
                linha[strcspn(linha, "\n")] = 0;
                for (int i = 0; linha[i]; i++) linha[i] = (char)tolower((unsigned char)linha[i]);
                if (strcmp(linha, "manual") == 0) mostrar_manual();
            } else {
                break; /* entrada encerrada */
            }
        }
    }

    strncpy(p.usuario, "bot", sizeof(p.usuario) - 1);
    p.hash = hash_senha(senhaBot);
    p.sucesso = 1;
    return p;
}