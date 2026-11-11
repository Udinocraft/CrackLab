#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <stddef.h>

/* Conjunto de caracteres usado tanto para criar quanto para
   atacar senhas (definido em bruteforce.c). */
extern const char CHARSET[];

/* Hash didatico (djb2) - nao e criptografico, serve so para nao
   guardar a senha em texto puro na memoria/struct. Reaproveitado
   por perfil.c para gerar o hash da senha do usuario e do bot. */
unsigned long hash_senha(const char *str);

typedef struct {
    int quebrada;                /* 1 = senha encontrada, 0 = resistiu ao ataque */
    char senhaEncontrada[32];    /* valida somente se quebrada == 1 */
    unsigned long tentativas;    /* total de combinacoes testadas de verdade */
    double segundosGastos;       /* tempo real gasto no ataque */
} ResultadoAtaque;

/* Ataca com toda a forca o hash informado. Imprime o progresso
   no stdout e devolve o resultado para quem chamou decidir o
   vencedor do jogo. */
ResultadoAtaque atacar(const char *usuario, unsigned long hashAlvo);

#endif /* BRUTEFORCE_H */