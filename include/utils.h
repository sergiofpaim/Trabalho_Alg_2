#ifndef UTILS_H
#define UTILS_H

#include <time.h>

void serializarString(char *destino, char *origem);
void deserializarString(char *destino, char *origem);
int gerarId(int min, int max);
char *formatarTempo(unsigned long long int ms);
char *formatarData(time_t data);
int validarData(char *str);
int validarTempo(int horas, int minutos, int segundos, int milissegundos);

#endif
