/*
 * menu.c
 * ---------------------------------------------------------
 * Unico arquivo do projeto com main(). E a porta de entrada do
 * jogo "Vitima vs Invasor": chama direto as funcoes de perfil.c
 * (modo_vitima/modo_invasor) e bruteforce.c (atacar), sem
 * system(), sem processos separados e sem arquivos em disco -
 * tudo roda num unico executavel, ideal para o CLion.
 * ---------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Perfil.h"
#include "Bruteforce.h"

typedef enum { MODO_VITIMA, MODO_INVASOR } Modo;

void jogar_vitima(void);
void jogar_invasor(void);
void menu_pos_derrota(Modo modoAnterior);
void menu_pos_vitoria(Modo modoAnterior);

void limpar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void aguardar_enter(void) {
    printf("\nPressione ENTER para continuar...");
    fflush(stdout);
    getchar();
}

/* ------------------------- MODO VITIMA ------------------------- */
void jogar_vitima(void) {
    printf("\n=== MODO VITIMA ===\n");
    printf("Voce vai criar um usuario e, em seguida, tera 60 segundos\n");
    printf("para criar a senha mais segura possivel.\n");
    aguardar_enter();

    Perfil p = modo_vitima();

    if (!p.sucesso) {
        printf("\n### TEMPO ESGOTADO! Voce nao concluiu a senha a tempo. ###\n");
        printf("### RESULTADO: VOCE PERDEU ###\n");
        menu_pos_derrota(MODO_VITIMA);
        return;
    }

    printf("\nSenha criada! Agora o invasor vai atacar com toda a forca...\n");
    aguardar_enter();

    ResultadoAtaque r = atacar(p.usuario, p.hash);

    if (r.quebrada) {
        printf("\n### O INVASOR CONSEGUIU QUEBRAR SUA SENHA! ###\n");
        printf("### RESULTADO: VOCE PERDEU ###\n");
        menu_pos_derrota(MODO_VITIMA);
    } else {
        printf("\n### SUA SENHA RESISTIU AO ATAQUE DE FORCA BRUTA! ###\n");
        printf("### RESULTADO: VOCE VENCEU! ###\n");
        menu_pos_vitoria(MODO_VITIMA);
    }
}

/* ------------------------- MODO INVASOR ------------------------- */
void jogar_invasor(void) {
    printf("\n=== MODO INVASOR ===\n");
    printf("O bot vai criar uma senha segura para se proteger.\n");
    printf("Voce tera ate 60 segundos de espera, podendo consultar\n");
    printf("o manual do brute force digitando 'manual'.\n");
    aguardar_enter();

    Perfil p = modo_invasor();

    printf("\nO bot terminou! Hora de atacar com toda a forca...\n");
    aguardar_enter();

    ResultadoAtaque r = atacar(p.usuario, p.hash);

    if (r.quebrada) {
        printf("\n### VOCE CONSEGUIU QUEBRAR A SENHA DO BOT! ###\n");
        printf("### RESULTADO: VOCE VENCEU! ###\n");
        menu_pos_vitoria(MODO_INVASOR);
    } else {
        printf("\n### A SENHA DO BOT RESISTIU - LEVARIA ANOS PARA QUEBRAR! ###\n");
        printf("### RESULTADO: VOCE PERDEU ###\n");
        menu_pos_derrota(MODO_INVASOR);
    }
}

/* --------------------- SUBMENUS DE FIM DE JOGO --------------------- */
void menu_pos_derrota(Modo modoAnterior) {
    int opcao;
    while (1) {
        printf("\nO que deseja fazer?\n");
        printf("1. Tentar novamente\n");
        printf("2. Jogar como VITIMA\n");
        printf("3. Voltar ao menu principal\n");
        printf("Escolha: ");

        int lido = scanf("%d", &opcao);
        if (lido == EOF) return; /* entrada encerrada: evita loop infinito */
        if (lido != 1) { limpar_buffer_entrada(); continue; }
        limpar_buffer_entrada();

        if (opcao == 1) {
            if (modoAnterior == MODO_VITIMA) jogar_vitima(); else jogar_invasor();
            return;
        } else if (opcao == 2) {
            jogar_vitima();
            return;
        } else if (opcao == 3) {
            return;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

void menu_pos_vitoria(Modo modoAnterior) {
    int opcao;
    while (1) {
        printf("\nParabens! O que deseja fazer agora?\n");
        printf("1. Jogar novamente (mesmo modo)\n");
        printf("2. Voltar ao menu principal\n");
        printf("Escolha: ");

        int lido = scanf("%d", &opcao);
        if (lido == EOF) return; /* entrada encerrada: evita loop infinito */
        if (lido != 1) { limpar_buffer_entrada(); continue; }
        limpar_buffer_entrada();

        if (opcao == 1) {
            if (modoAnterior == MODO_VITIMA) jogar_vitima(); else jogar_invasor();
            return;
        } else if (opcao == 2) {
            return;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

/* ------------------------------ MAIN ------------------------------ */
int main(void) {
    int opcao;

    while (1) {
        printf("\n==========================================\n");
        printf("   JOGO: VITIMA vs INVASOR - Forca Bruta\n");
        printf("==========================================\n");
        printf("1. Jogar como VITIMA  (crie a senha mais segura)\n");
        printf("2. Jogar como INVASOR (espere o bot e tente invadir)\n");
        printf("3. Sair\n");
        printf("Escolha: ");

        int lido = scanf("%d", &opcao);
        if (lido == EOF) { printf("\nEntrada encerrada. Saindo...\n"); break; }
        if (lido != 1) { limpar_buffer_entrada(); continue; }
        limpar_buffer_entrada();

        if (opcao == 1) jogar_vitima();
        else if (opcao == 2) jogar_invasor();
        else if (opcao == 3) { printf("\nAte a proxima!\n"); break; }
        else printf("Opcao invalida.\n");
    }

    return 0;
}