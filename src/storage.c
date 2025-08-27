#include "../include/storage.h"
#include "../include/data.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   Seção: Serialização e Desserialização
   ============================================================ */

void desserializarAlteracoes()
{
    FILE *fileUsuarios = fopen("armazenamento/usuarios.bin", "rb");
    if (!fileUsuarios)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    else
    {
        fread(&usuarios.tamanho, sizeof(int), 1, fileUsuarios);
        usuarios.lista = (struct Usuario *)realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
        for (int i = 0; i < usuarios.tamanho; i++)
        {
            fread(&usuarios.lista[i], sizeof(struct Usuario), 1, fileUsuarios);
        }
        fclose(fileUsuarios);
    }

    FILE *fileJogos = fopen("armazenamento/jogos.bin", "rb");
    if (!fileJogos)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    else
    {
        fread(&jogos.tamanho, sizeof(int), 1, fileJogos);
        jogos.lista = (struct Jogo *)realloc(jogos.lista, jogos.tamanho * sizeof(struct Jogo));
        for (int i = 0; i < jogos.tamanho; i++)
        {
            fread(&jogos.lista[i], sizeof(struct Jogo), 1, fileJogos);
        }
        fclose(fileJogos);
    }

    FILE *fileRecordes = fopen("armazenamento/recordes.bin", "rb");
    if (!fileRecordes)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    else
    {
        fread(&recordes.tamanho, sizeof(int), 1, fileRecordes);
        recordes.lista = (struct Recorde *)realloc(recordes.lista, recordes.tamanho * sizeof(struct Recorde));
        for (int i = 0; i < recordes.tamanho; i++)
        {
            fread(&recordes.lista[i], sizeof(struct Recorde), 1, fileRecordes);
        }
        fclose(fileRecordes);
    }
}

void serializarAlteracoes()
{
    FILE *fileUsuarios = fopen("armazenamento/usuarios.bin", "wb");
    if (!fileUsuarios)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fwrite(&usuarios.tamanho, sizeof(int), 1, fileUsuarios);
    for (int i = 0; i < usuarios.tamanho; i++)
    {
        fwrite(&usuarios.lista[i], sizeof(struct Usuario), 1, fileUsuarios);
    }
    fclose(fileUsuarios);

    FILE *fileJogos = fopen("armazenamento/jogos.bin", "wb");
    if (!fileJogos)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fwrite(&jogos.tamanho, sizeof(int), 1, fileJogos);
    for (int i = 0; i < jogos.tamanho; i++)
    {
        fwrite(&jogos.lista[i], sizeof(struct Jogo), 1, fileJogos);
    }
    fclose(fileJogos);

    FILE *fileRecordes = fopen("armazenamento/recordes.bin", "wb");
    if (!fileRecordes)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fwrite(&recordes.tamanho, sizeof(int), 1, fileRecordes);
    for (int i = 0; i < recordes.tamanho; i++)
    {
        fwrite(&recordes.lista[i], sizeof(struct Recorde), 1, fileRecordes);
    }
    fclose(fileRecordes);

    printf("Alterações salvas com sucesso!\n");
}

void exportarResultado(struct Recordes *consulta)
{
    FILE *fileTabela = fopen("armazenamento/tabela_recordes.txt", "w");
    if (!fileTabela)
    {
        printf("\nFalha ao salvar o arquivo\n");
        return;
    }

    fprintf(fileTabela, "--------------------------\n");
    for (int i = 0; i < consulta->tamanho; i++)
    {
        char *tempo_str = formatarTempo(consulta->lista[i].tempo);
        char *data_str = formatarData(consulta->lista[i].data_registro);

        fprintf(fileTabela, "Usuário: %s\n", consulta->lista[i].usuario);
        fprintf(fileTabela, "Jogo: %s\n", consulta->lista[i].jogo);
        fprintf(fileTabela, "Plataforma: %s\n", consulta->lista[i].plataforma);
        fprintf(fileTabela, "Tempo: %s\n", tempo_str);
        fprintf(fileTabela, "Data de Registro: %s\n", data_str);
        fprintf(fileTabela, "Identificador: %d\n", consulta->lista[i].identificacao);
        fprintf(fileTabela, "--------------------------\n");

        free(tempo_str);
        free(data_str);
    }

    fclose(fileTabela);
    printf("\nArquivo tabela_recordes.txt salvo com a consulta!\n");
}