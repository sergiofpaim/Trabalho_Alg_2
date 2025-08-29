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
int validarRecordeTempo(char* str);
unsigned long long int converterTempo(char* tempo);
void separarArgumentos(char elementos[][50], int quantidade, char* valor);

#endif
