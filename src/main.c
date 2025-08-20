#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/data.h"
#include "../include/operations.h"
#include "../include/storage.h"

int main()
{
    /* Inicializa o gerador com a hora atual */
    srand((unsigned int)time(NULL));

    /* Carrega dados do armazenamento */
    desserializarAlteracoes();
        
    int prompt = 0;

    printf("\nBem vindo ao Speed Runners!\n");

    while (prompt != 999)
    {
        printf("\nDigite o comando, ou 0 para ajuda\n");
        printf("\n> ");
        if (scanf("%d", &prompt) != 1) {
            while(getchar() != '\n');
            printf("\nEntrada inválida\n");
            continue;
        }
        while(getchar() != '\n');

        interpretador(prompt);
    }

    return 0;
}
