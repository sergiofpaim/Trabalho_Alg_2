#ifndef DATA_H
#define DATA_H

#include <time.h>

struct Usuario
{
    char apelido[24];
    char email[32];
    char nascimento[11];
    char pais[12];
};

struct Jogo
{
    char nome[24];
    char desenvolvedora[32];
    char data_lancamento[24];
    char genero[32];
};

struct Recorde
{
    char usuario[24];
    char jogo[24];
    char plataforma[32];
    unsigned long long int tempo;
    time_t data_registro;
    int identificacao;
};

struct Usuarios
{
    int tamanho;
    struct Usuario *lista;
};

struct Jogos
{
    int tamanho;
    struct Jogo *lista;
};

struct Recordes
{
    int tamanho;
    struct Recorde *lista;
};

struct Resultados
{
    int tamanho;
    int *lista;
};

extern struct Usuarios usuarios;
extern struct Jogos jogos;
extern struct Recordes recordes;

#endif
