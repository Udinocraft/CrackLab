cmake_minimum_required(VERSION 3.10)
project(jogo_senha C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(jogo_senha
        CrackLab/Menu.c
        CrackLab/Perfil.c
        CrackLab/Perfil.h
        CrackLab/Bruteforce.c
        CrackLab/Bruteforce.h
)

# Se estiver usando a função pow(), lembre-se de adicionar também:
target_link_libraries(jogo_senha PRIVATE m)