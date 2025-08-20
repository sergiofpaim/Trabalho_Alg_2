#include "../include/utils.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

/* ============================================================
   Sessão com utilidades para o resto do código principal
   ============================================================ */

void serializarString(char *destino, char *origem)
{
    int i;
    for (i = 0; origem[i] != '\0'; i++)
        destino[i] = (origem[i] == ' ') ? '_' : origem[i];
    destino[i] = '\0';
}

void deserializarString(char *destino, char *origem)
{
    int i;
    for (i = 0; origem[i] != '\0'; i++)
        destino[i] = (origem[i] == '_') ? ' ' : origem[i];
    destino[i] = '\0';
}

int gerarId(int min, int max)
{
    return min + rand() % (max - min + 1);
}

char *formatarTempo(unsigned long long int ms)
{
    unsigned long long horas, minutos, segundos, milisecundos;

    horas = ms / 3600000ULL;
    minutos = (ms % 3600000ULL) / 60000ULL;
    segundos = (ms % 60000ULL) / 1000ULL;
    milisecundos = ms % 1000ULL;

    char *saida = malloc(13);
    snprintf(saida, 13, "%02llu:%02llu:%02llu:%03llu",
             horas, minutos, segundos, milisecundos);

    return saida;
}

char *formatarData(time_t data)
{
    struct tm *tm_info = localtime(&data);
    char *saida = malloc(11);
    strftime(saida, 11, "%d/%m/%Y", tm_info);
    return saida;
}

int validarData(char *str)
{
    if (!str || strlen(str) != 10)
        return 0;

    if (str[2] != '-' || str[5] != '-')
        return 0;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    int dia = atoi(str);
    int mes = atoi(str + 3);
    int ano = atoi(str + 6);

    if (mes < 1 || mes > 12)
        return 0;

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        diasNoMes[1] = 29;

    if (dia < 1 || dia > diasNoMes[mes - 1])
        return 0;

    return 1;
}

int validarTempo(int horas, int minutos, int segundos, int milissegundos)
{
    if (horas < 0)
        return 0;
    if (minutos < 0 || minutos > 59)
        return 0;
    if (segundos < 0 || segundos > 59)
        return 0;
    if (milissegundos < 0 || milissegundos > 999)
        return 0;
    return 1;
}