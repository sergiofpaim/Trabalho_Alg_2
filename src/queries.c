#include "../include/queries.h"
#include "../include/data.h"

#include <stdlib.h>
#include <string.h>

/* ============================================================
   Seção: Operações de localização de usuários, jogos e recordes
   ============================================================ */

int usuarioQuery(char *identificacao)
{
    for (int i = 0; i < usuarios.tamanho; i++)
        if (strcmp(usuarios.lista[i].apelido, identificacao) == 0)
            return i;

    return -1;
}

int jogoQuery(char *identificacao)
{
    for (int i = 0; i < jogos.tamanho; i++)
        if (strcmp(jogos.lista[i].nome, identificacao) == 0)
            return i;

    return -1;
}

struct Resultados recordeQuery(char *nomeJogador, char *nomeJogo, char *identificacao_recorde)
{
    struct Resultados resultados;
    resultados.tamanho = 0;
    resultados.lista = (int *)malloc(resultados.tamanho * sizeof(int));
    for (int i = 0; i <= recordes.tamanho - 1; i++)
    {
        int achouJogador = (strcmp(nomeJogador, "*") == 0) || (strcmp(nomeJogador, recordes.lista[i].usuario) == 0);
        int achouJogo = (strcmp(nomeJogo, "*") == 0) || (strcmp(nomeJogo, recordes.lista[i].jogo) == 0);
        int achouId = (strcmp(identificacao_recorde, "*") == 0) || (recordes.lista[i].identificacao == atoi(identificacao_recorde));

        if (achouId && achouJogador && achouJogo)
        {
            resultados.lista = (int *)realloc(resultados.lista, ++resultados.tamanho * sizeof(int));
            resultados.lista[resultados.tamanho - 1] = i;
        }
    }

    return resultados;
}