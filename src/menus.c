#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/menus.h"
#include "../include/operations.h"
#include "../include/storage.h"

void adicionar()
{
    printf("Modo de Adição\n");
    while (1)
    {
        int controle;
        char campos[4][50];

        printf("Modo de uso: <numero> <valor_campos> / 999 para voltar\n");
        printf("1. Usuario: apelido email nascimento pais\n");
        printf("2. Jogo: nome desenvolvedora data_lancamento genero\n");
        printf("3. Recorde: usuario jogo plataforma tempo\n");
        printf("\n> ");
        scanf("%d", &controle);
        
        getchar();

        if (controle == 999) break;
        for (int i = 0; i < 4; i++) 
        {
            printf("\n> ");
            fgets(campos[i], sizeof(campos), stdin);
            campos[i][strcspn(campos[i], "\n")] = '\0';
        }
        
        switch(controle)
        {
            case 1:
                printf("Usuario\n");
                break;
            case 2:
                printf("Jogo\n");
                break;
            case 3:
                printf("Recorde\n");
                break;
            default:
                printf("Numero invalido!\n");
                break;
        }
    }
}

void editar()
{
    printf("Modo de Edição\n");
    while (1)
    {
        int controle;
        char campo[30], valor[100], id[50];
        printf("\nModo de uso: <numero> <identificador> <campo> <valor> / 999 para voltar\n");
        printf("1. Usuario: *Apelido, Email, Nascimento, Pais\n");
        printf("2. Jogos: *Nome, Desenvolvedora, Data_Lancamento, genero\n");
        printf("3. Recordes: *identificação Usuario, Jogo, Plataforma, Tempo\n");

        printf("\n> ");
        scanf("%d", &controle);
        getchar();
        if (controle == 999) break;
        fgets(id, sizeof id, stdin);
        scanf("%s", campo);
        getchar();
        fgets(valor, sizeof(valor), stdin);

        for (int i = 0; campo[i] != '\0'; i++)
            campo[i] = tolower(campo[i]);

        switch (controle) {
            case 1:
                if (strcmp(campo, "apelido") == 0)
                {
                    printf("apelido\n");
                    break;
                }
                if (strcmp(campo, "email") == 0)
                {
                    printf("email\n");
                    break;
                }
                if (strcmp(campo, "nascimento") == 0)
                {
                    printf("nascimento\n");
                    break;
                }
                if (strcmp(campo, "pais") == 0)
                {
                    printf("pais\n");
                    break;
                }
                else 
                {
                    printf("Campo de usuário invalido\n");
                    break;
                }
            case 2:
                if (strcmp(campo, "nome") == 0)
                { 
                    printf("nome\n");
                    break;
                }
                if (strcmp(campo, "desenvolvedora") == 0)
                {
                    printf("desenvolvedora\n");
                    break;
                }
                if (strcmp(campo, "data_lancamento") == 0)
                {
                    printf("data_lancamento\n");
                    break;
                }
                if (strcmp(campo, "genero") == 0)
                {
                    printf("genero\n");
                    break;
                }
                else 
                {
                    printf("Campo de jogo invalido\n");
                    break;
                }
            case 3:
                if (strcmp(campo, "usuario") == 0)
                { 
                    printf("usuario\n");
                    break;
                }
                if (strcmp(campo, "jogo") == 0)
                {
                    printf("jogo\n");
                    break;
                }
                if (strcmp(campo, "plataforma") == 0)
                {
                    printf("plataforma\n");
                    break;
                }
                if (strcmp(campo, "tempo") == 0)
                {
                    printf("tempo\n");
                    break;
                }
                else 
                {
                    printf("Campo de recorde invalido\n");
                    break;
                }
            default:
                printf("Numero Invalido\n");
                break;
        }
    }
}

void deletar()
{
    printf("Modo de Deleção\n");
    while (1)
    {
        int controle;

        printf("Modo de uso: <numero> <identificador>\n");
        printf("1. Usuario: Apelido\n");
        printf("2. Jogo: Nome\n");
        printf("3. Recorde: identificador\n");

        scanf("%d", &controle);
        if (controle == 999) break;

        switch (controle) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                printf("Numero invalido\n");
                break;
        }
    }
}

void consultar()
{
    printf("Modo de Consulta\n");
    while(1)
    {
        int controle;

        printf("Modo de uso: \n1. Consulta de Recordes\n2. Listar Usuarios\n3. Listar Jogos\n");

        printf("\n> ");
        scanf("%d", &controle);
        getchar();
        if (controle == 999) break;

        switch(controle)
        {
            case 1:
                char nomeJogador[24];
                char nomeJogo[24];
                char identificacao_recorde[32];

                printf("\nDigite a sua consulta neste formato \n\napelido_jogador\nnome_jogo\nidentificação_recorde\n\nOu '*' para todos daquela posicao\n");
                printf("\nApelido Jogador> ");

                fgets(nomeJogador, sizeof(nomeJogador), stdin);
                nomeJogador[strcspn(nomeJogador, "\n")] = '\0';

                printf("\nNome jogo> ");

                fgets(nomeJogo, sizeof(nomeJogo), stdin);
                nomeJogo[strcspn(nomeJogo, "\n")] = '\0';

                printf("\nIdentificacão Recorde> ");

                fgets(identificacao_recorde, sizeof(identificacao_recorde), stdin);
                identificacao_recorde[strcspn(identificacao_recorde, "\n")] = '\0';

                struct Recordes consulta;
                consulta = consultaRecordes(nomeJogador, nomeJogo, identificacao_recorde);
                if (consulta.tamanho == 0) 
                {
                    printf("Nenhum recorde encontrado\n");
                } else 
                {
                    mostrarRecordes(consulta);
                    printf("Deseja exportar essa tabela? (0/1)\n");

                    int exportar;
                    scanf("%d", &exportar);
                    if (exportar) exportarResultado(&consulta);
                }
                break;
            case 2:
                mostrarUsuarios();
                break;
            case 3:
                mostrarJogos();
                break;
        }
    }
}
