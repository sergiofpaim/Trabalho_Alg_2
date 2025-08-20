#ifndef TRABALHO_QUERIES_H
#define TRABALHO_QUERIES_H

#include "data.h"

int usuarioQuery(char *identificacao);
int jogoQuery(char *identificacao);
struct Resultados recordeQuery(char *nomeJogador, char *nomeJogo, char *identificacao_recorde);

#endif
