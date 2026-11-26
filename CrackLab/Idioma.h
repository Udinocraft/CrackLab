#ifndef IDIOMA_H
#define IDIOMA_H

/* Idiomas disponiveis no jogo. */
typedef enum {
    IDI_PT_BR,
    IDI_PT_PT,
    IDI_EN_US,
    IDI_EN_GB,
    IDI_ES,
    IDI_ZH,
    IDI_FR,
    IDI_TOTAL
} Idioma;

/* Identificadores de todas as mensagens do jogo. */
typedef enum {
    /* menu principal */
    M_MENU_TITLE, M_MENU_VICTIM, M_MENU_CRACKER, M_MENU_DIFF, M_MENU_LANG,
    M_MENU_HISTORY, M_MENU_EXIT, M_CHOICE, M_INVALID, M_INPUT_END, M_BYE, M_PRESS_ENTER,
    /* dificuldade / idioma */
    M_DIFF_TITLE, M_DIFF_HELP, M_DIFF_SET, M_LANG_TITLE, M_LANG_SET,
    /* modo vitima */
    M_VIC_TITLE, M_VIC_ASK_USER, M_DEFAULT_USER, M_VIC_TIMER_INTRO, M_PRESS_START,
    M_PASSWORD, M_CLOCK, M_TIME_UP, M_VIC_TIMEOUT, M_VIC_CREATED_ATTACK,
    M_PROFILE_CREATED, M_VIC_CRACKED, M_VIC_RESISTED, M_RES_WIN, M_RES_LOSE,
    /* avaliacao da senha */
    M_EV_TITLE, M_EV_LEN, M_EV_CASE, M_EV_NUMSYM, M_EV_CLASS,
    M_YES, M_NO, M_WEAK, M_MEDIUM, M_STRONG,
    /* modo cracker */
    M_CRK_TITLE, M_CRK_INTRO, M_CRK_WAIT_START, M_CRK_WAITING, M_CRK_BOT_DONE_BAR,
    M_CRK_DONE, M_CRK_CRACKED, M_CRK_RESISTED, M_MANUAL_TITLE, M_MANUAL_BODY,
    /* submenus de fim de jogo */
    M_POST_LOSE_Q, M_POST_RETRY, M_POST_AS_VICTIM, M_POST_BACK,
    M_POST_WIN_Q, M_POST_AGAIN, M_POST_WIN_BACK,
    /* ataque de forca bruta */
    M_ATK_TITLE, M_ATK_TARGET, M_ATK_LINE, M_ATK_ABORT, M_ATK_RESISTED, M_ATK_FOUND,
    M_ATK_STATS, M_ATK_CRACKED, M_ATK_EXHAUSTED,
    M_T_SEC, M_T_MIN, M_T_HOUR, M_T_DAY, M_T_YEAR,
    /* placar e historico */
    M_SB_TITLE, M_SB_SIDES, M_SB_YOU, M_SB_EMPTY, M_HIST_TITLE,
    M_MODE_VICTIM, M_MODE_CRACKER, M_RES_WON_SHORT, M_RES_LOST_SHORT, M_TRIES,
    M_TOTAL
} MsgId;

void idioma_definir(Idioma idioma);
Idioma idioma_atual(void);

/* Nome do idioma, sempre escrito na propria lingua. */
const char *idioma_nome(Idioma idioma);

/* Devolve o texto da mensagem no idioma atual. */
const char *T(MsgId id);

#endif /* IDIOMA_H */