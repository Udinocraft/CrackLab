#ifndef PERFIL_H
#define PERFIL_H

typedef struct {
    char usuario[64];
    unsigned long hash;  /* hash da senha, nunca a senha em texto puro */
    int sucesso;         /* 1 = perfil criado; 0 = tempo esgotado (so no modo vitima) */
} Perfil;

/* Humano cria usuario + senha, com cronometro de 60s so para a senha. */
Perfil modo_vitima(void);

/* Bot cria uma senha sozinho; o humano so espera (ate 60s) e pode
   consultar o manual do brute force digitando "manual". */
Perfil modo_invasor(void);

#endif /* PERFIL_H */