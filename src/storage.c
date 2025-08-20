#include "../include/storage.h"
#include "../include/data.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

void exportarResultado(struct Recordes *consulta)
{
    FILE* out = fopen("armazenamento/tabela_recordes.txt", "w");
    if (!out) {
        printf("\nFalha ao salvar o arquivo\n");
        return;
    }

    fprintf(out, "--------------------------\n");
    for (int i = 0; i < consulta->tamanho; i++)
    {
        char *tempo_str = formatarTempo(consulta->lista[i].tempo);
        char *data_str = formatarData(consulta->lista[i].data_registro);

        fprintf(out, "Usuário: %s\n", consulta->lista[i].usuario);
        fprintf(out, "Jogo: %s\n", consulta->lista[i].jogo);
        fprintf(out, "Plataforma: %s\n", consulta->lista[i].plataforma);
        fprintf(out, "Tempo: %s\n", tempo_str);
        fprintf(out, "Data de Registro: %s\n", data_str);
        fprintf(out, "Identificador: %d\n", consulta->lista[i].identificacao);
        fprintf(out, "--------------------------\n");

        free(tempo_str);
        free(data_str);
    }

    fclose(out);
    printf("\nArquivo tabela_recordes.txt salvo com a consulta!\n");
}

void desserializarAlteracoes()
{
    FILE *fileUsuarios = fopen("armazenamento/usuarios.txt", "r");
    if (!fileUsuarios) {
        // sem arquivo mantém coleções vazias
    } else {
        fscanf(fileUsuarios, "%d", &usuarios.tamanho);
        usuarios.lista = (struct Usuario*) realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
        for (int i = 0; i < usuarios.tamanho; i++)
        {
            struct Usuario temp;
            fscanf(fileUsuarios, "%s %s %s %s", temp.apelido,
                                                temp.email,
                                                temp.nascimento,
                                                temp.pais);

            deserializarString(usuarios.lista[i].apelido, temp.apelido);
            deserializarString(usuarios.lista[i].email, temp.email);
            deserializarString(usuarios.lista[i].nascimento, temp.nascimento);
            deserializarString(usuarios.lista[i].pais, temp.pais);
        }
        fclose(fileUsuarios);
    }

    FILE *fileJogos = fopen("armazenamento/jogos.txt", "r");
    if (!fileJogos) {
        // sem arquivo
    } else {
        fscanf(fileJogos, "%d", &jogos.tamanho);
        jogos.lista = (struct Jogo*) realloc(jogos.lista, jogos.tamanho * sizeof(struct Jogo));
        for (int i = 0; i < jogos.tamanho; i++)
        {
            struct Jogo temp;
            fscanf(fileJogos, "%s %s %s %s", temp.nome,
                                             temp.genero,
                                             temp.desenvolvedora,
                                             temp.data_lancamento);

            deserializarString(jogos.lista[i].nome, temp.nome);
            deserializarString(jogos.lista[i].genero, temp.genero);
            deserializarString(jogos.lista[i].desenvolvedora, temp.desenvolvedora);
            deserializarString(jogos.lista[i].data_lancamento, temp.data_lancamento);
        }
        fclose(fileJogos);
    }

    FILE *fileRecordes = fopen("armazenamento/recordes.txt", "r");
    if (!fileRecordes) {
        // sem arquivo
    } else {
        fscanf(fileRecordes, "%d", &recordes.tamanho);
        recordes.lista = (struct Recorde*) realloc(recordes.lista, recordes.tamanho * sizeof(struct Recorde));
        for (int i = 0; i < recordes.tamanho; i++)
        {
            struct Recorde temp;
            fscanf(fileRecordes, "%s %s %s %llu %lld %d", temp.usuario,
                                                             temp.jogo,
                                                             temp.plataforma,
                                                             &temp.tempo,
                                                             &temp.data_registro,
                                                             &temp.identificacao);

            deserializarString(recordes.lista[i].usuario, temp.usuario);
            deserializarString(recordes.lista[i].jogo, temp.jogo);
            deserializarString(recordes.lista[i].plataforma, temp.plataforma);
            recordes.lista[i].tempo = temp.tempo;
            recordes.lista[i].data_registro = temp.data_registro;
            recordes.lista[i].identificacao = temp.identificacao;
        }
        fclose(fileRecordes);
    }
}

void serializarAlteracoes()
{
    FILE *fileUsuarios = fopen("armazenamento/usuarios.txt", "w");
    if (!fileUsuarios) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(fileUsuarios, "%d\n", usuarios.tamanho);
    for (int i = 0; i < usuarios.tamanho; i++)
    {
        char buffer[100];

        serializarString(buffer, usuarios.lista[i].apelido);
        fprintf(fileUsuarios, "%s ", buffer);
        serializarString(buffer, usuarios.lista[i].email);
        fprintf(fileUsuarios, "%s ", buffer);
        serializarString(buffer, usuarios.lista[i].nascimento);
        fprintf(fileUsuarios, "%s ", buffer);
        serializarString(buffer, usuarios.lista[i].pais);
        fprintf(fileUsuarios, "%s\n", buffer);
    }

    fclose(fileUsuarios);

    FILE *fileJogos = fopen("armazenamento/jogos.txt", "w");
    if (!fileJogos) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(fileJogos, "%d\n", jogos.tamanho);
    for (int i = 0; i < jogos.tamanho; i++)
    {
        char buffer[100];

        serializarString(buffer, jogos.lista[i].nome);
        fprintf(fileJogos, "%s ", buffer);
        serializarString(buffer, jogos.lista[i].desenvolvedora);
        fprintf(fileJogos, "%s ", buffer);
        serializarString(buffer, jogos.lista[i].data_lancamento);
        fprintf(fileJogos, "%s ", buffer);
        serializarString(buffer, jogos.lista[i].genero);
        fprintf(fileJogos, "%s\n", buffer);
    }

    fclose(fileJogos);

    FILE *fileRecordes = fopen("armazenamento/recordes.txt", "w");
    if (!fileRecordes) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(fileRecordes, "%d\n", recordes.tamanho);
    for (int i = 0; i < recordes.tamanho; i++)
    {
        char buffer[100];
        
        serializarString(buffer, recordes.lista[i].usuario);
        fprintf(fileRecordes, "%s ", buffer);
        serializarString(buffer, recordes.lista[i].jogo);
        fprintf(fileRecordes, "%s ", buffer);
        serializarString(buffer, recordes.lista[i].plataforma);
        fprintf(fileRecordes, "%s ", buffer);

        fprintf(fileRecordes, "%llu %lld %d\n", recordes.lista[i].tempo,
                                                            recordes.lista[i].data_registro,
                                                            recordes.lista[i].identificacao);
    }

    fclose(fileRecordes);

    printf("Alterações salvas com sucesso!\n");
}
