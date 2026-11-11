/*
 * perfil.c
 * ---------------------------------------------------------
 * Modulo de criacao de perfil/senha do jogo "Vitima vs Invasor".
 * Sem main() de proposito: este arquivo e um MODULO, chamado
 * diretamente por menu.c (unico arquivo com main() no projeto).
 * Reaproveita hash_senha() e CHARSET de bruteforce.h, assim a
 * senha criada aqui e sempre compativel com o que o modulo de
 * ataque vai tentar quebrar depois.
 * ---------------------------------------------------------
 */

#include "Perfil.h"
#include "Bruteforce.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#define TAM_MAX 64
#define TEMPO_LIMITE_SEGUNDOS 60

/* Le uma linha do teclado mostrando uma contagem regressiva ate
   'prazoFinal'. Retorna 1 se leu algo a tempo, 0 se o tempo
   esgotou antes de um ENTER. 'rotulo' aparece junto do relogio. */
static int ler_com_cronometro(const char *rotulo, char *buffer, size_t tamanho, time_t prazoFinal) {
    while (1) {
        time_t agora = time(NULL);
        int restante = (int)(prazoFinal - agora);
        if (restante <= 0) {
            printf("\r[TEMPO ESGOTADO]                                              \n");
            return 0;
        }

        printf("\r[%2ds restantes] %s: ", restante, rotulo);
        fflush(stdout);

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv = {1, 0}; /* verifica a cada 1s, atualizando o relogio */

        int pronto = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (pronto > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(buffer, tamanho, stdin) == NULL) return 0;
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

    const char *classificacao = (pontuacao <= 2) ? "FRACA" : (pontuacao <= 4) ? "MEDIA" : "FORTE";

    printf("\n===== Avaliacao da senha =====\n");
    printf("Tamanho ...........: %d caracteres\n", len);
    printf("Minusculas/Maiusc. : %s / %s\n", minus ? "sim" : "nao", maius ? "sim" : "nao");
    printf("Numeros/Simbolos ..: %s / %s\n", digito ? "sim" : "nao", simbolo ? "sim" : "nao");
    printf("Classificacao .....: %s\n", classificacao);
    printf("================================\n");
}

/* ---------------------- MODO VITIMA ---------------------- */
Perfil modo_vitima(void) {
    Perfil p = {0};
    char usuario[64];

    printf("=== MODO VITIMA ===\n");
    printf("Primeiro, digite um nome de usuario (sem pressa): ");
    fflush(stdout);
    if (fgets(usuario, sizeof(usuario), stdin) == NULL) {
        p.sucesso = 0;
        return p;
    }
    usuario[strcspn(usuario, "\n")] = 0;
    if (strlen(usuario) == 0) strcpy(usuario, "jogador");

    printf("\nAgora o cronometro vai comecar: voce tem %d segundos para\n", TEMPO_LIMITE_SEGUNDOS);
    printf("criar a senha MAIS SEGURA possivel.\n");
    printf("Pressione ENTER para comecar...");
    fflush(stdout);
    getchar();

    time_t prazo = time(NULL) + TEMPO_LIMITE_SEGUNDOS;

    char senha[TAM_MAX];
    if (!ler_com_cronometro("Senha", senha, sizeof(senha), prazo) || strlen(senha) == 0) {
        p.sucesso = 0;
        return p;
    }

    avaliar_forca(senha);

    strncpy(p.usuario, usuario, sizeof(p.usuario) - 1);
    p.hash = hash_senha(senha);
    p.sucesso = 1;

    printf("\nPerfil criado com sucesso!\n");
    return p;
}

/* ---------------------- MODO INVASOR ---------------------- */

/* Gera uma senha aleatoria para o bot. A forca varia a cada
   partida para dar replay ao jogo (as vezes fraca, as vezes
   forte, usando o mesmo conjunto de caracteres do bruteforce). */
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
    printf("\n================= MANUAL DO BRUTE FORCE =================\n");
    printf("- A ferramenta testa combinacoes de caracteres de tamanho\n");
    printf("  crescente (1, 2, 3, ...) ate encontrar a senha.\n");
    printf("- O conjunto usado tem %d caracteres possiveis: minusculas,\n", (int)strlen(CHARSET));
    printf("  maiusculas, numeros e simbolos.\n");
    printf("- Para tamanhos pequenos ela tenta DE VERDADE, na marra,\n");
    printf("  medindo o tempo real gasto.\n");
    printf("- Quando o numero de combinacoes fica grande demais, ela\n");
    printf("  calcula uma ESTIMATIVA de tempo (podendo chegar a anos)\n");
    printf("  e desiste do ataque.\n");
    printf("- Moral da historia: senhas longas e variadas sao\n");
    printf("  praticamente impossiveis de quebrar por forca bruta.\n");
    printf("===========================================================\n\n");
}

Perfil modo_invasor(void) {
    Perfil p = {0};

    printf("=== MODO INVASOR ===\n");
    printf("O bot esta criando uma senha segura para se proteger.\n");
    printf("Digite 'manual' a qualquer momento e pressione ENTER para\n");
    printf("consultar o manual do brute force enquanto espera.\n");
    printf("Pressione ENTER para comecar a espera...");
    fflush(stdout);
    getchar();

    srand((unsigned int)time(NULL));
    char senhaBot[32];
    gerar_senha_bot(senhaBot);

    time_t prazo = time(NULL) + TEMPO_LIMITE_SEGUNDOS;
    while (1) {
        time_t agora = time(NULL);
        int restante = (int)(prazo - agora);
        if (restante <= 0) {
            printf("\r[O bot terminou de preparar a senha!]                          \n");
            break;
        }

        printf("\r[%2ds restantes] Aguardando o bot... (digite 'manual'+ENTER p/ ajuda) ", restante);
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
            }
        }
    }

    strncpy(p.usuario, "bot", sizeof(p.usuario) - 1);
    p.hash = hash_senha(senhaBot);
    p.sucesso = 1;

    printf("\nO bot protegeu o perfil dele. Hora de tentar invadir...\n");
    return p;
}