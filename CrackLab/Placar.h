#ifndef PLACAR_H
#define PLACAR_H

typedef enum { MODO_VITIMA, MODO_CRACKER } Modo;

/* Registro de uma partida. */
typedef struct {
    long long quando;            /* timestamp (time_t) */
    int modo;                    /* Modo: o que o jogador escolheu */
    int dificuldade;             /* Dificuldade usada na partida */
    int usuarioVenceu;           /* 1 = o jogador venceu */
    int quebrada;                /* 1 = senha quebrada (ponto do CRACKER), 0 = resistiu (ponto da VITIMA) */
    unsigned long tentativas;
    double segundos;
    char usuario[64];
} Partida;

/* Le o historico salvo em disco (cracklab_historico.txt), se existir. */
void placar_carregar(void);

/* Guarda a partida na memoria e acrescenta no arquivo. */
void placar_registrar(const Partida *p);

/* Placar: VITIMA x CRACKER + campanha do jogador. */
void placar_mostrar_resumo(void);

/* Placar + lista das ultimas partidas. */
void placar_mostrar_historico(void);

#endif /* PLACAR_H */