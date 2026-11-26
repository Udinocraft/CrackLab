/*
 * Idioma.c
 * ---------------------------------------------------------
 * Tabela de traducoes do CrackLab (arquivo em UTF-8).
 * Ordem das colunas:
 *   pt-BR | pt-PT | en-US | en-GB | es | zh (mandarim) | fr
 * Regra: os especificadores de printf (%d, %s...) de uma mesma
 * mensagem precisam estar na MESMA ORDEM em todos os idiomas.
 * ---------------------------------------------------------
 */

#include "Idioma.h"

static Idioma g_idioma = IDI_PT_BR;

static const char *const NOMES[IDI_TOTAL] = {
    "Português (Brasil)", "Português (Portugal)", "English (US)",
    "English (UK)", "Español", "中文 (普通话)", "Français"
};

static const char *const TABELA[M_TOTAL][IDI_TOTAL] = {
    /* ---------------- menu principal ---------------- */
    [M_MENU_TITLE] = {
        "CRACKLAB: VÍTIMA vs CRACKER - Força Bruta",
        "CRACKLAB: VÍTIMA vs CRACKER - Força Bruta",
        "CRACKLAB: VICTIM vs CRACKER - Brute Force",
        "CRACKLAB: VICTIM vs CRACKER - Brute Force",
        "CRACKLAB: VÍCTIMA vs CRACKER - Fuerza Bruta",
        "CRACKLAB：受害者 vs 破解者 - 暴力破解",
        "CRACKLAB : VICTIME vs CRACKER - Force brute" },
    [M_MENU_VICTIM] = {
        "1. Jogar como VÍTIMA  (crie a senha mais segura)",
        "1. Jogar como VÍTIMA  (crie a palavra-passe mais segura)",
        "1. Play as VICTIM  (create the strongest password)",
        "1. Play as VICTIM  (create the strongest password)",
        "1. Jugar como VÍCTIMA  (crea la contraseña más segura)",
        "1. 扮演受害者（创建最安全的密码）",
        "1. Jouer en VICTIME  (créez le mot de passe le plus sûr)" },
    [M_MENU_CRACKER] = {
        "2. Jogar como CRACKER (espere o bot e tente invadir)",
        "2. Jogar como CRACKER (aguarde o bot e tente invadir)",
        "2. Play as CRACKER (wait for the bot, then break in)",
        "2. Play as CRACKER (wait for the bot, then try to break in)",
        "2. Jugar como CRACKER (espera al bot e intenta entrar)",
        "2. 扮演破解者（等待机器人，然后尝试入侵）",
        "2. Jouer en CRACKER (attendez le bot et tentez de pénétrer)" },
    [M_MENU_DIFF] = {
        "3. Dificuldade (atual: %s)",
        "3. Dificuldade (atual: %s)",
        "3. Difficulty (current: %s)",
        "3. Difficulty (current: %s)",
        "3. Dificultad (actual: %s)",
        "3. 难度（当前：%s）",
        "3. Difficulté (actuelle : %s)" },
    [M_MENU_LANG] = {
        "4. Idioma (atual: %s)",
        "4. Idioma (atual: %s)",
        "4. Language (current: %s)",
        "4. Language (current: %s)",
        "4. Idioma (actual: %s)",
        "4. 语言（当前：%s）",
        "4. Langue (actuelle : %s)" },
    [M_MENU_HISTORY] = {
        "5. Placar e histórico de partidas",
        "5. Marcador e histórico de jogos",
        "5. Scoreboard and match history",
        "5. Scoreboard and game history",
        "5. Marcador e historial de partidas",
        "5. 比分与对局历史",
        "5. Score et historique des parties" },
    [M_MENU_EXIT] = {
        "6. Sair", "6. Sair", "6. Exit", "6. Exit", "6. Salir", "6. 退出", "6. Quitter" },
    [M_CHOICE] = {
        "Escolha: ", "Escolha: ", "Choice: ", "Choice: ", "Elige: ", "请选择：", "Choix : " },
    [M_INVALID] = {
        "Opção inválida.", "Opção inválida.", "Invalid option.", "Invalid option.",
        "Opción no válida.", "无效选项。", "Option invalide." },
    [M_INPUT_END] = {
        "Entrada encerrada. Saindo...", "Entrada encerrada. A sair...",
        "Input closed. Exiting...", "Input closed. Exiting...",
        "Entrada cerrada. Saliendo...", "输入已结束，正在退出……",
        "Entrée fermée. Fermeture..." },
    [M_BYE] = {
        "Até a próxima!", "Até à próxima!", "See you next time!", "Cheerio, see you next time!",
        "¡Hasta la próxima!", "下次再见！", "À la prochaine !" },
    [M_PRESS_ENTER] = {
        "Pressione ENTER para continuar...", "Prima ENTER para continuar...",
        "Press ENTER to continue...", "Press ENTER to continue...",
        "Pulsa ENTER para continuar...", "按回车键继续……",
        "Appuyez sur ENTRÉE pour continuer..." },

    /* ---------------- dificuldade / idioma ---------------- */
    [M_DIFF_TITLE] = {
        "=== DIFICULDADE ===", "=== DIFICULDADE ===", "=== DIFFICULTY ===", "=== DIFFICULTY ===",
        "=== DIFICULTAD ===", "=== 难度 ===", "=== DIFFICULTÉ ===" },
    [M_DIFF_HELP] = {
        "O tempo vale para a vítima criar a senha e para a espera no modo cracker.",
        "O tempo aplica-se à criação da palavra-passe (vítima) e à espera (cracker).",
        "The time limit applies to the victim creating a password and to the wait in cracker mode.",
        "The time limit applies to the victim creating a password and to the wait in cracker mode.",
        "El tiempo se aplica a la creación de la contraseña (víctima) y a la espera (cracker).",
        "该时间适用于受害者创建密码，以及破解者模式下的等待。",
        "Le temps s'applique à la création du mot de passe (victime) et à l'attente (cracker)." },
    [M_DIFF_SET] = {
        "Dificuldade definida: %s", "Dificuldade definida: %s", "Difficulty set: %s",
        "Difficulty set: %s", "Dificultad establecida: %s", "难度已设置：%s",
        "Difficulté définie : %s" },
    [M_LANG_TITLE] = {
        "=== IDIOMA / LANGUAGE ===", "=== IDIOMA / LANGUAGE ===", "=== IDIOMA / LANGUAGE ===",
        "=== IDIOMA / LANGUAGE ===", "=== IDIOMA / LANGUAGE ===", "=== IDIOMA / LANGUAGE ===",
        "=== IDIOMA / LANGUAGE ===" },
    [M_LANG_SET] = {
        "Idioma alterado para: %s", "Idioma alterado para: %s", "Language changed to: %s",
        "Language changed to: %s", "Idioma cambiado a: %s", "语言已切换为：%s",
        "Langue changée en : %s" },

    /* ---------------- modo vitima ---------------- */
    [M_VIC_TITLE] = {
        "=== MODO VÍTIMA ===", "=== MODO VÍTIMA ===", "=== VICTIM MODE ===", "=== VICTIM MODE ===",
        "=== MODO VÍCTIMA ===", "=== 受害者模式 ===", "=== MODE VICTIME ===" },
    [M_VIC_ASK_USER] = {
        "Primeiro, digite um nome de usuário (sem pressa): ",
        "Primeiro, escreva um nome de utilizador (sem pressa): ",
        "First, type a username (no rush): ",
        "First, type a username (no rush): ",
        "Primero, escribe un nombre de usuario (sin prisa): ",
        "首先输入用户名（不着急）：",
        "D'abord, saisissez un nom d'utilisateur (sans hâte) : " },
    [M_DEFAULT_USER] = {
        "jogador", "jogador", "player", "player", "jugador", "玩家", "joueur" },
    [M_VIC_TIMER_INTRO] = {
        "\nAgora o cronômetro vai começar: você tem %d segundos para\ncriar a senha MAIS SEGURA possível.",
        "\nAgora o cronómetro vai começar: tem %d segundos para\ncriar a palavra-passe MAIS SEGURA possível.",
        "\nThe timer is about to start: you have %d seconds to\ncreate the STRONGEST password you can.",
        "\nThe timer is about to start: you have %d seconds to\ncreate the STRONGEST password possible.",
        "\nAhora empieza el cronómetro: tienes %d segundos para\ncrear la contraseña MÁS SEGURA posible.",
        "\n计时即将开始：你有 %d 秒时间\n创建尽可能最安全的密码。",
        "\nLe chronomètre va démarrer : vous avez %d secondes pour\ncréer le mot de passe le PLUS SÛR possible." },
    [M_PRESS_START] = {
        "Pressione ENTER para começar...", "Prima ENTER para começar...",
        "Press ENTER to start...", "Press ENTER to start...",
        "Pulsa ENTER para empezar...", "按回车键开始……", "Appuyez sur ENTRÉE pour commencer..." },
    [M_PASSWORD] = {
        "Senha", "Palavra-passe", "Password", "Password", "Contraseña", "密码", "Mot de passe" },
    [M_CLOCK] = {
        "\r[%2ds restantes] %s: ", "\r[%2ds restantes] %s: ", "\r[%2ds left] %s: ",
        "\r[%2ds left] %s: ", "\r[%2ds restantes] %s: ", "\r[剩余 %2d 秒] %s：",
        "\r[%2ds restantes] %s : " },
    [M_TIME_UP] = {
        "\r[TEMPO ESGOTADO]                                              \n",
        "\r[TEMPO ESGOTADO]                                              \n",
        "\r[TIME'S UP]                                                   \n",
        "\r[TIME'S UP]                                                   \n",
        "\r[TIEMPO AGOTADO]                                              \n",
        "\r[时间到]                                                      \n",
        "\r[TEMPS ÉCOULÉ]                                                \n" },
    [M_VIC_TIMEOUT] = {
        "### TEMPO ESGOTADO! Você não concluiu a senha a tempo. ###",
        "### TEMPO ESGOTADO! Não concluiu a palavra-passe a tempo. ###",
        "### TIME'S UP! You did not finish your password in time. ###",
        "### TIME'S UP! You did not finish your password in time. ###",
        "### ¡TIEMPO AGOTADO! No terminaste la contraseña a tiempo. ###",
        "### 时间到！你没有及时完成密码。###",
        "### TEMPS ÉCOULÉ ! Vous n'avez pas terminé votre mot de passe à temps. ###" },
    [M_VIC_CREATED_ATTACK] = {
        "Senha criada! Agora o cracker vai atacar com toda a força...",
        "Palavra-passe criada! Agora o cracker vai atacar com toda a força...",
        "Password created! Now the cracker will attack with everything it has...",
        "Password created! Now the cracker will attack with everything it has...",
        "¡Contraseña creada! Ahora el cracker atacará con toda su fuerza...",
        "密码已创建！现在破解者将全力攻击……",
        "Mot de passe créé ! Le cracker va maintenant attaquer de toutes ses forces..." },
    [M_PROFILE_CREATED] = {
        "Perfil criado com sucesso!", "Perfil criado com sucesso!",
        "Profile created successfully!", "Profile created successfully!",
        "¡Perfil creado con éxito!", "资料创建成功！", "Profil créé avec succès !" },
    [M_VIC_CRACKED] = {
        "### O CRACKER CONSEGUIU QUEBRAR SUA SENHA! ###",
        "### O CRACKER CONSEGUIU QUEBRAR A SUA PALAVRA-PASSE! ###",
        "### THE CRACKER BROKE YOUR PASSWORD! ###",
        "### THE CRACKER BROKE YOUR PASSWORD! ###",
        "### ¡EL CRACKER LOGRÓ ROMPER TU CONTRASEÑA! ###",
        "### 破解者成功破解了你的密码！###",
        "### LE CRACKER A RÉUSSI À CASSER VOTRE MOT DE PASSE ! ###" },
    [M_VIC_RESISTED] = {
        "### SUA SENHA RESISTIU AO ATAQUE DE FORÇA BRUTA! ###",
        "### A SUA PALAVRA-PASSE RESISTIU AO ATAQUE DE FORÇA BRUTA! ###",
        "### YOUR PASSWORD SURVIVED THE BRUTE-FORCE ATTACK! ###",
        "### YOUR PASSWORD SURVIVED THE BRUTE-FORCE ATTACK! ###",
        "### ¡TU CONTRASEÑA RESISTIÓ EL ATAQUE DE FUERZA BRUTA! ###",
        "### 你的密码经受住了暴力破解攻击！###",
        "### VOTRE MOT DE PASSE A RÉSISTÉ À L'ATTAQUE PAR FORCE BRUTE ! ###" },
    [M_RES_WIN] = {
        "### RESULTADO: VOCÊ VENCEU! ###", "### RESULTADO: VENCEU! ###",
        "### RESULT: YOU WIN! ###", "### RESULT: YOU WIN! ###",
        "### RESULTADO: ¡GANASTE! ###", "### 结果：你赢了！###",
        "### RÉSULTAT : VOUS AVEZ GAGNÉ ! ###" },
    [M_RES_LOSE] = {
        "### RESULTADO: VOCÊ PERDEU ###", "### RESULTADO: PERDEU ###",
        "### RESULT: YOU LOSE ###", "### RESULT: YOU LOSE ###",
        "### RESULTADO: PERDISTE ###", "### 结果：你输了 ###",
        "### RÉSULTAT : VOUS AVEZ PERDU ###" },

    /* ---------------- avaliacao da senha ---------------- */
    [M_EV_TITLE] = {
        "===== Avaliação da senha =====", "===== Avaliação da palavra-passe =====",
        "===== Password evaluation =====", "===== Password evaluation =====",
        "===== Evaluación de la contraseña =====", "===== 密码评估 =====",
        "===== Évaluation du mot de passe =====" },
    [M_EV_LEN] = {
        "Tamanho: %d caracteres", "Tamanho: %d caracteres", "Length: %d characters",
        "Length: %d characters", "Longitud: %d caracteres", "长度：%d 个字符",
        "Longueur : %d caractères" },
    [M_EV_CASE] = {
        "Minúsculas / maiúsculas: %s / %s", "Minúsculas / maiúsculas: %s / %s",
        "Lowercase / uppercase: %s / %s", "Lowercase / uppercase: %s / %s",
        "Minúsculas / mayúsculas: %s / %s", "小写 / 大写：%s / %s",
        "Minuscules / majuscules : %s / %s" },
    [M_EV_NUMSYM] = {
        "Números / símbolos: %s / %s", "Números / símbolos: %s / %s",
        "Digits / symbols: %s / %s", "Numbers / symbols: %s / %s",
        "Números / símbolos: %s / %s", "数字 / 符号：%s / %s",
        "Chiffres / symboles : %s / %s" },
    [M_EV_CLASS] = {
        "Classificação: %s", "Classificação: %s", "Rating: %s", "Rating: %s",
        "Clasificación: %s", "强度：%s", "Niveau : %s" },
    [M_YES] = { "sim", "sim", "yes", "yes", "sí", "是", "oui" },
    [M_NO]  = { "não", "não", "no", "no", "no", "否", "non" },
    [M_WEAK]   = { "FRACA", "FRACA", "WEAK", "WEAK", "DÉBIL", "弱", "FAIBLE" },
    [M_MEDIUM] = { "MÉDIA", "MÉDIA", "MEDIUM", "MEDIUM", "MEDIA", "中", "MOYEN" },
    [M_STRONG] = { "FORTE", "FORTE", "STRONG", "STRONG", "FUERTE", "强", "FORT" },

    /* ---------------- modo cracker ---------------- */
    [M_CRK_TITLE] = {
        "=== MODO CRACKER ===", "=== MODO CRACKER ===", "=== CRACKER MODE ===", "=== CRACKER MODE ===",
        "=== MODO CRACKER ===", "=== 破解者模式 ===", "=== MODE CRACKER ===" },
    [M_CRK_INTRO] = {
        "O bot está criando uma senha segura para se proteger.\nVocê terá %d segundos de espera. Digite 'manual' e pressione ENTER\npara consultar o manual do brute force enquanto espera.",
        "O bot está a criar uma palavra-passe segura para se proteger.\nTerá %d segundos de espera. Escreva 'manual' e prima ENTER\npara consultar o manual do brute force enquanto espera.",
        "The bot is creating a strong password to protect itself.\nYou will wait %d seconds. Type 'manual' and press ENTER\nto read the brute-force manual while you wait.",
        "The bot is creating a strong password to protect itself.\nYou will be waiting %d seconds. Type 'manual' and press ENTER\nto read the brute-force manual in the meantime.",
        "El bot está creando una contraseña segura para protegerse.\nEsperarás %d segundos. Escribe 'manual' y pulsa ENTER\npara consultar el manual de fuerza bruta mientras esperas.",
        "机器人正在创建安全密码来保护自己。\n你需要等待 %d 秒。输入 'manual' 并按回车，\n即可在等待时查看暴力破解手册。",
        "Le bot crée un mot de passe sûr pour se protéger.\nVous attendrez %d secondes. Tapez 'manual' puis ENTRÉE\npour consulter le manuel de la force brute pendant l'attente." },
    [M_CRK_WAIT_START] = {
        "Pressione ENTER para começar a espera...", "Prima ENTER para começar a espera...",
        "Press ENTER to start waiting...", "Press ENTER to start waiting...",
        "Pulsa ENTER para empezar la espera...", "按回车键开始等待……",
        "Appuyez sur ENTRÉE pour commencer l'attente..." },
    [M_CRK_WAITING] = {
        "\r[%2ds restantes] Aguardando o bot... (digite 'manual'+ENTER p/ ajuda) ",
        "\r[%2ds restantes] A aguardar o bot... (escreva 'manual'+ENTER p/ ajuda) ",
        "\r[%2ds left] Waiting for the bot... (type 'manual'+ENTER for help) ",
        "\r[%2ds left] Waiting for the bot... (type 'manual'+ENTER for help) ",
        "\r[%2ds restantes] Esperando al bot... (escribe 'manual'+ENTER para ayuda) ",
        "\r[剩余 %2d 秒] 等待机器人……（输入 'manual'+回车 查看帮助） ",
        "\r[%2ds restantes] Attente du bot... (tapez 'manual'+ENTRÉE pour l'aide) " },
    [M_CRK_BOT_DONE_BAR] = {
        "\r[O bot terminou de preparar a senha!]                              \n",
        "\r[O bot terminou de preparar a palavra-passe!]                       \n",
        "\r[The bot has finished preparing its password!]                      \n",
        "\r[The bot has finished preparing its password!]                      \n",
        "\r[¡El bot terminó de preparar su contraseña!]                        \n",
        "\r[机器人已完成密码设置！]                                            \n",
        "\r[Le bot a fini de préparer son mot de passe !]                      \n" },
    [M_CRK_DONE] = {
        "O bot terminou! Hora de atacar com toda a força...",
        "O bot terminou! Hora de atacar com toda a força...",
        "The bot is done! Time to attack with everything you've got...",
        "The bot is done! Time to attack with everything you've got...",
        "¡El bot terminó! Es hora de atacar con toda la fuerza...",
        "机器人已完成！现在全力攻击吧……",
        "Le bot a terminé ! C'est l'heure d'attaquer de toutes vos forces..." },
    [M_CRK_CRACKED] = {
        "### VOCÊ CONSEGUIU QUEBRAR A SENHA DO BOT! ###",
        "### CONSEGUIU QUEBRAR A PALAVRA-PASSE DO BOT! ###",
        "### YOU CRACKED THE BOT'S PASSWORD! ###",
        "### YOU CRACKED THE BOT'S PASSWORD! ###",
        "### ¡LOGRASTE ROMPER LA CONTRASEÑA DEL BOT! ###",
        "### 你成功破解了机器人的密码！###",
        "### VOUS AVEZ CASSÉ LE MOT DE PASSE DU BOT ! ###" },
    [M_CRK_RESISTED] = {
        "### A SENHA DO BOT RESISTIU - LEVARIA ANOS PARA QUEBRAR! ###",
        "### A PALAVRA-PASSE DO BOT RESISTIU - LEVARIA ANOS A QUEBRAR! ###",
        "### THE BOT'S PASSWORD HELD OUT - IT WOULD TAKE YEARS TO CRACK! ###",
        "### THE BOT'S PASSWORD HELD OUT - IT WOULD TAKE YEARS TO CRACK! ###",
        "### LA CONTRASEÑA DEL BOT RESISTIÓ - TARDARÍA AÑOS EN ROMPERSE! ###",
        "### 机器人的密码坚不可摧——破解需要数年！###",
        "### LE MOT DE PASSE DU BOT A RÉSISTÉ - IL FAUDRAIT DES ANNÉES POUR LE CASSER ! ###" },
    [M_MANUAL_TITLE] = {
        "================= MANUAL DO BRUTE FORCE =================",
        "================= MANUAL DO BRUTE FORCE =================",
        "================= BRUTE FORCE MANUAL =================",
        "================= BRUTE FORCE MANUAL =================",
        "================= MANUAL DE FUERZA BRUTA =================",
        "================= 暴力破解手册 =================",
        "================= MANUEL DE LA FORCE BRUTE =================" },
    [M_MANUAL_BODY] = {
        "- A ferramenta testa combinações de caracteres de tamanho\n  crescente (1, 2, 3, ...) até encontrar a senha.\n- O conjunto usado tem %d caracteres possíveis: minúsculas,\n  maiúsculas, números e símbolos.\n- Para tamanhos pequenos ela tenta DE VERDADE, medindo o tempo real.\n- Quando o número de combinações fica grande demais, ela calcula\n  uma ESTIMATIVA de tempo (podendo chegar a anos) e desiste.\n- Moral da história: senhas longas e variadas são praticamente\n  impossíveis de quebrar por força bruta.\n",
        "- A ferramenta testa combinações de caracteres de tamanho\n  crescente (1, 2, 3, ...) até encontrar a palavra-passe.\n- O conjunto usado tem %d caracteres possíveis: minúsculas,\n  maiúsculas, números e símbolos.\n- Para tamanhos pequenos tenta MESMO, medindo o tempo real.\n- Quando o número de combinações é demasiado grande, calcula\n  uma ESTIMATIVA de tempo (podendo chegar a anos) e desiste.\n- Moral da história: palavras-passe longas e variadas são\n  praticamente impossíveis de quebrar por força bruta.\n",
        "- The tool tries character combinations of increasing length\n  (1, 2, 3, ...) until it finds the password.\n- The character set has %d possible characters: lowercase,\n  uppercase, digits and symbols.\n- For short lengths it REALLY tries them all, timing the real run.\n- When the number of combinations gets too large, it computes a\n  time ESTIMATE (possibly years) and gives up.\n- Moral of the story: long, varied passwords are practically\n  impossible to brute-force.\n",
        "- The tool tries character combinations of increasing length\n  (1, 2, 3, ...) until it finds the password.\n- The character set has %d possible characters: lowercase,\n  uppercase, digits and symbols.\n- For short lengths it genuinely tries them all, timing the real run.\n- When the number of combinations becomes too large, it works out a\n  time ESTIMATE (possibly years) and gives up.\n- The moral: long, varied passwords are practically\n  impossible to brute-force.\n",
        "- La herramienta prueba combinaciones de caracteres de longitud\n  creciente (1, 2, 3, ...) hasta encontrar la contraseña.\n- El conjunto tiene %d caracteres posibles: minúsculas,\n  mayúsculas, números y símbolos.\n- Para longitudes pequeñas lo intenta DE VERDAD, midiendo el tiempo real.\n- Cuando hay demasiadas combinaciones, calcula una ESTIMACIÓN\n  de tiempo (puede llegar a años) y se rinde.\n- Moraleja: las contraseñas largas y variadas son prácticamente\n  imposibles de romper por fuerza bruta.\n",
        "- 该工具会按长度递增（1、2、3……）尝试字符组合，\n  直到找到密码。\n- 字符集共有 %d 个可能的字符：小写字母、\n  大写字母、数字和符号。\n- 对于较短的长度，它会真正逐个尝试，并记录实际耗时。\n- 当组合数量过大时，它会计算预计耗时（可能长达数年）\n  然后放弃攻击。\n- 结论：又长又多样的密码几乎无法被暴力破解。\n",
        "- L'outil teste des combinaisons de caractères de longueur\n  croissante (1, 2, 3, ...) jusqu'à trouver le mot de passe.\n- Le jeu contient %d caractères possibles : minuscules,\n  majuscules, chiffres et symboles.\n- Pour les petites longueurs, il essaie VRAIMENT, en mesurant le temps réel.\n- Quand le nombre de combinaisons devient trop grand, il calcule\n  une ESTIMATION du temps (parfois des années) et abandonne.\n- Morale : les mots de passe longs et variés sont pratiquement\n  impossibles à casser par force brute.\n" },

    /* ---------------- submenus de fim de jogo ---------------- */
    [M_POST_LOSE_Q] = {
        "O que deseja fazer?", "O que pretende fazer?", "What would you like to do?",
        "What would you like to do?", "¿Qué deseas hacer?", "接下来想做什么？",
        "Que souhaitez-vous faire ?" },
    [M_POST_RETRY] = {
        "1. Tentar novamente", "1. Tentar novamente", "1. Try again", "1. Try again",
        "1. Intentar de nuevo", "1. 再试一次", "1. Réessayer" },
    [M_POST_AS_VICTIM] = {
        "2. Jogar como VÍTIMA", "2. Jogar como VÍTIMA", "2. Play as VICTIM", "2. Play as VICTIM",
        "2. Jugar como VÍCTIMA", "2. 扮演受害者", "2. Jouer en VICTIME" },
    [M_POST_BACK] = {
        "3. Voltar ao menu principal", "3. Voltar ao menu principal", "3. Back to the main menu",
        "3. Return to the main menu", "3. Volver al menú principal", "3. 返回主菜单",
        "3. Retour au menu principal" },
    [M_POST_WIN_Q] = {
        "Parabéns! O que deseja fazer agora?", "Parabéns! O que pretende fazer agora?",
        "Congratulations! What next?", "Well done! What would you like to do next?",
        "¡Felicidades! ¿Qué deseas hacer ahora?", "恭喜！接下来想做什么？",
        "Félicitations ! Que souhaitez-vous faire maintenant ?" },
    [M_POST_AGAIN] = {
        "1. Jogar novamente (mesmo modo)", "1. Jogar novamente (mesmo modo)",
        "1. Play again (same mode)", "1. Play again (same mode)",
        "1. Jugar de nuevo (mismo modo)", "1. 再玩一次（相同模式）", "1. Rejouer (même mode)" },
    [M_POST_WIN_BACK] = {
        "2. Voltar ao menu principal", "2. Voltar ao menu principal", "2. Back to the main menu",
        "2. Return to the main menu", "2. Volver al menú principal", "2. 返回主菜单",
        "2. Retour au menu principal" },

    /* ---------------- ataque de forca bruta ---------------- */
    [M_ATK_TITLE] = {
        "=== ATAQUE DE FORÇA BRUTA - TODA A FORÇA DISPONÍVEL ===",
        "=== ATAQUE DE FORÇA BRUTA - TODA A FORÇA DISPONÍVEL ===",
        "=== BRUTE-FORCE ATTACK - FULL POWER ===",
        "=== BRUTE-FORCE ATTACK - FULL POWER ===",
        "=== ATAQUE DE FUERZA BRUTA - TODA LA POTENCIA ===",
        "=== 暴力破解攻击 - 全力出击 ===",
        "=== ATTAQUE PAR FORCE BRUTE - PUISSANCE MAXIMALE ===" },
    [M_ATK_TARGET] = {
        "Alvo: usuário \"%s\"\n", "Alvo: utilizador \"%s\"\n", "Target: user \"%s\"\n",
        "Target: user \"%s\"\n", "Objetivo: usuario \"%s\"\n", "目标：用户 \"%s\"\n",
        "Cible : utilisateur \"%s\"\n" },
    [M_ATK_LINE] = {
        "Tamanho %2d | combinações: %.3e | tempo estimado: %s\n",
        "Tamanho %2d | combinações: %.3e | tempo estimado: %s\n",
        "Length %2d | combinations: %.3e | estimated time: %s\n",
        "Length %2d | combinations: %.3e | estimated time: %s\n",
        "Longitud %2d | combinaciones: %.3e | tiempo estimado: %s\n",
        "长度 %2d | 组合数：%.3e | 预计耗时：%s\n",
        "Longueur %2d | combinaisons : %.3e | temps estimé : %s\n" },
    [M_ATK_ABORT] = {
        "\n>>> Mesmo com toda a força disponível, quebrar uma senha deste\n>>> tamanho levaria aproximadamente %s.\n>>> Ataque abortado: inviável na prática.\n\n",
        "\n>>> Mesmo com toda a força disponível, quebrar uma palavra-passe deste\n>>> tamanho levaria aproximadamente %s.\n>>> Ataque abortado: inviável na prática.\n\n",
        "\n>>> Even with all available power, cracking a password of this\n>>> length would take roughly %s.\n>>> Attack aborted: not feasible in practice.\n\n",
        "\n>>> Even with all available power, cracking a password of this\n>>> length would take roughly %s.\n>>> Attack aborted: not feasible in practice.\n\n",
        "\n>>> Incluso con toda la potencia disponible, romper una contraseña de esta\n>>> longitud tardaría aproximadamente %s.\n>>> Ataque abortado: inviable en la práctica.\n\n",
        "\n>>> 即使动用全部算力，破解这种长度的密码\n>>> 也大约需要 %s。\n>>> 攻击中止：实际上不可行。\n\n",
        "\n>>> Même avec toute la puissance disponible, casser un mot de passe de cette\n>>> longueur prendrait environ %s.\n>>> Attaque abandonnée : irréalisable en pratique.\n\n" },
    [M_ATK_RESISTED] = {
        "RESULTADO TÉCNICO: A SENHA RESISTIU AO ATAQUE.",
        "RESULTADO TÉCNICO: A PALAVRA-PASSE RESISTIU AO ATAQUE.",
        "TECHNICAL RESULT: THE PASSWORD SURVIVED THE ATTACK.",
        "TECHNICAL RESULT: THE PASSWORD SURVIVED THE ATTACK.",
        "RESULTADO TÉCNICO: LA CONTRASEÑA RESISTIÓ EL ATAQUE.",
        "技术结果：密码经受住了攻击。",
        "RÉSULTAT TECHNIQUE : LE MOT DE PASSE A RÉSISTÉ À L'ATTAQUE." },
    [M_ATK_FOUND] = {
        "\n>>> Senha encontrada: \"%s\"\n", "\n>>> Palavra-passe encontrada: \"%s\"\n",
        "\n>>> Password found: \"%s\"\n", "\n>>> Password found: \"%s\"\n",
        "\n>>> Contraseña encontrada: \"%s\"\n", "\n>>> 已找到密码：\"%s\"\n",
        "\n>>> Mot de passe trouvé : \"%s\"\n" },
    [M_ATK_STATS] = {
        ">>> Tentativas: %lu | Tempo real: %.4f segundos\n",
        ">>> Tentativas: %lu | Tempo real: %.4f segundos\n",
        ">>> Attempts: %lu | Real time: %.4f seconds\n",
        ">>> Attempts: %lu | Real time: %.4f seconds\n",
        ">>> Intentos: %lu | Tiempo real: %.4f segundos\n",
        ">>> 尝试次数：%lu | 实际耗时：%.4f 秒\n",
        ">>> Tentatives : %lu | Temps réel : %.4f secondes\n" },
    [M_ATK_CRACKED] = {
        "RESULTADO TÉCNICO: A SENHA FOI QUEBRADA.",
        "RESULTADO TÉCNICO: A PALAVRA-PASSE FOI QUEBRADA.",
        "TECHNICAL RESULT: THE PASSWORD WAS CRACKED.",
        "TECHNICAL RESULT: THE PASSWORD WAS CRACKED.",
        "RESULTADO TÉCNICO: LA CONTRASEÑA FUE ROTA.",
        "技术结果：密码已被破解。",
        "RÉSULTAT TECHNIQUE : LE MOT DE PASSE A ÉTÉ CASSÉ." },
    [M_ATK_EXHAUSTED] = {
        "\nBusca esgotada até o tamanho máximo testado sem sucesso.\n",
        "\nPesquisa esgotada até ao tamanho máximo testado, sem sucesso.\n",
        "\nSearch exhausted up to the maximum length tested, without success.\n",
        "\nSearch exhausted up to the maximum length tested, without success.\n",
        "\nBúsqueda agotada hasta la longitud máxima probada, sin éxito.\n",
        "\n已搜索到测试的最大长度，仍未成功。\n",
        "\nRecherche épuisée jusqu'à la longueur maximale testée, sans succès.\n" },
    [M_T_SEC]  = { "segundos", "segundos", "seconds", "seconds", "segundos", "秒", "secondes" },
    [M_T_MIN]  = { "minutos", "minutos", "minutes", "minutes", "minutos", "分钟", "minutes" },
    [M_T_HOUR] = { "horas", "horas", "hours", "hours", "horas", "小时", "heures" },
    [M_T_DAY]  = { "dias", "dias", "days", "days", "días", "天", "jours" },
    [M_T_YEAR] = { "anos", "anos", "years", "years", "años", "年", "ans" },

    /* ---------------- placar e historico ---------------- */
    [M_SB_TITLE] = {
        "========== PLACAR ==========", "========== MARCADOR ==========",
        "========== SCOREBOARD ==========", "========== SCOREBOARD ==========",
        "========== MARCADOR ==========", "========== 比分 ==========",
        "========== SCORE ==========" },
    [M_SB_SIDES] = {
        "VÍTIMA (defesa) %d  x  %d CRACKER (ataque)",
        "VÍTIMA (defesa) %d  x  %d CRACKER (ataque)",
        "VICTIM (defense) %d  x  %d CRACKER (attack)",
        "VICTIM (defence) %d  x  %d CRACKER (attack)",
        "VÍCTIMA (defensa) %d  x  %d CRACKER (ataque)",
        "受害者（防守）%d  比  %d 破解者（进攻）",
        "VICTIME (défense) %d  x  %d CRACKER (attaque)" },
    [M_SB_YOU] = {
        "Você: %d vitórias / %d derrotas", "Jogador: %d vitórias / %d derrotas",
        "You: %d wins / %d losses", "You: %d wins / %d losses",
        "Tú: %d victorias / %d derrotas", "你：%d 胜 / %d 负",
        "Vous : %d victoires / %d défaites" },
    [M_SB_EMPTY] = {
        "Nenhuma partida registrada ainda.", "Ainda não há jogos registados.",
        "No games recorded yet.", "No games recorded yet.",
        "Aún no hay partidas registradas.", "暂无对局记录。",
        "Aucune partie enregistrée pour l'instant." },
    [M_HIST_TITLE] = {
        "===== HISTÓRICO (últimas %d partidas) =====",
        "===== HISTÓRICO (últimos %d jogos) =====",
        "===== HISTORY (last %d games) =====",
        "===== HISTORY (last %d games) =====",
        "===== HISTORIAL (últimas %d partidas) =====",
        "===== 对局历史（最近 %d 局）=====",
        "===== HISTORIQUE (%d dernières parties) =====" },
    [M_MODE_VICTIM] = {
        "Vítima", "Vítima", "Victim", "Victim", "Víctima", "受害者", "Victime" },
    [M_MODE_CRACKER] = {
        "Cracker", "Cracker", "Cracker", "Cracker", "Cracker", "破解者", "Cracker" },
    [M_RES_WON_SHORT] = {
        "Vitória", "Vitória", "Win", "Win", "Victoria", "胜", "Victoire" },
    [M_RES_LOST_SHORT] = {
        "Derrota", "Derrota", "Loss", "Loss", "Derrota", "负", "Défaite" },
    [M_TRIES] = {
        "tentativas", "tentativas", "attempts", "attempts", "intentos", "次尝试", "tentatives" },
};

void idioma_definir(Idioma idioma) {
    if (idioma >= 0 && idioma < IDI_TOTAL) g_idioma = idioma;
}

Idioma idioma_atual(void) { return g_idioma; }

const char *idioma_nome(Idioma idioma) {
    return (idioma >= 0 && idioma < IDI_TOTAL) ? NOMES[idioma] : "?";
}

const char *T(MsgId id) {
    if (id < 0 || id >= M_TOTAL) return "";
    const char *s = TABELA[id][g_idioma];
    return s ? s : TABELA[id][IDI_PT_BR]; /* fallback seguro */
}