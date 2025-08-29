#include "../include/data.h"
#include "../include/utils.h"
#include "../include/queries.h"
#include "../include/storage.h"
#include "../include/menus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================
   Seção: Operações CRUD (Adicionar, Editar, Deletar)
   - Funções para gerenciar usuarios, jogos e recordes
   ============================================================ */

int usuarioAdd(char* apelido, char* email, char* nascimento, char* pais)
{
    struct Usuario temp;

    strncpy(temp.apelido, apelido, sizeof(temp.apelido)-1);
    strncpy(temp.email, email, sizeof(temp.email)-1);
    strncpy(temp.nascimento, nascimento, sizeof(temp.nascimento)-1);
    strncpy(temp.pais, pais, sizeof(temp.pais)-1);

    if (usuarioQuery(temp.apelido) >= 0)
        return 1;

    if (validarData(temp.nascimento) == 0)
        return 2;

    usuarios.tamanho++;
    usuarios.lista = (struct Usuario *)realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
    if (usuarios.lista != NULL)
    {
        usuarios.lista[usuarios.tamanho - 1] = temp;
        return 0;
    }
    else
    {
        usuarios.tamanho--;
        return -1;
    }
}

int jogoAdd(char* nome, char* desenvolvedora, char* data_lancamento, char* genero)
{
    struct Jogo temp;

    strncpy(temp.nome, nome, sizeof(temp.nome)-1);
    strncpy(temp.desenvolvedora, desenvolvedora, sizeof(temp.desenvolvedora)-1);
    strncpy(temp.data_lancamento, data_lancamento, sizeof(temp.data_lancamento)-1);
    strncpy(temp.genero, genero, sizeof(temp.genero)-1);

    if (jogoQuery(temp.nome) >= 0)
        return 1;

    if (validarData(temp.data_lancamento) != 1)
        return 2;

    jogos.lista = (struct Jogo *)realloc(jogos.lista, ++jogos.tamanho * sizeof(struct Jogo));

    if (jogos.lista != NULL)
    {
        jogos.lista[jogos.tamanho - 1] = temp;
        return 0;
    }
    else
    {
        jogos.tamanho--;
        return -1;
    }
}

int recordeAdd(char* usuario, char* jogo, char* plataforma, char* tempo)
{
    struct Recorde temp;

    if (usuarioQuery(usuario) >= 0)
        strcpy(temp.usuario, usuario);
    else
        return 1;

    if (jogoQuery(jogo) >= 0)
        strcpy(temp.jogo, jogo);
    else
        return 2;

    if (validarRecordeTempo(tempo) == 1)
        temp.tempo = converterTempo(tempo);
    else 
        return 3;

    strncpy(temp.plataforma, plataforma, sizeof(temp.plataforma)-1);

    temp.data_registro = time(NULL);

    temp.identificacao = gerarId(1000, 10000);

    recordes.lista = (struct Recorde *)realloc(recordes.lista, ++recordes.tamanho * sizeof(struct Recorde));
    if (recordes.lista != NULL)
    {
        recordes.lista[recordes.tamanho - 1] = temp;
        return 0;
    }
    else
    {
        recordes.tamanho--;
        return -1;
    }
}

void usuarioEdit()
{
    int posicao;

    printf("\nDigite o apelido do usuario:\n");
    char temp[24];
    printf("\n> ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';

    if ((posicao = usuarioQuery(temp)) >= 0)
    {
        int escolha = 0;
        while (escolha != 999)
        {
            printf("\nVocê está no modo edição\n");
            printf("\nDigite o campo que gostaria de editar (1. Apelido, 2. Email, 3. Data de Nascimento, 4. País, 999. Sair da edição)\n");
            printf("\n> ");
            scanf("%d", &escolha);
            while (getchar() != '\n')
                ;
            switch (escolha)
            {
            case 1:
                printf("\nDigite o nome:\n");
                printf("\n> ");
                {
                    char tempNome[24];
                    fgets(tempNome, sizeof(tempNome), stdin);
                    tempNome[strcspn(tempNome, "\n")] = '\0';

                    if (usuarioQuery(tempNome) >= 0)
                        printf("\nNome já existe\n");
                    else
                    {
                        strcpy(usuarios.lista[posicao].apelido, tempNome);
                        printf("\nNome editado com sucesso\n");
                    }
                }
                break;
            case 2:
                printf("\nDigite o email:\n");
                printf("\n> ");
                fgets(usuarios.lista[posicao].email, sizeof(usuarios.lista[posicao].email), stdin);
                usuarios.lista[posicao].email[strcspn(usuarios.lista[posicao].email, "\n")] = '\0';
                printf("\nEmail editado com sucesso\n");
                break;
            case 3:
                do
                {
                    struct Usuario temp;

                    printf("\nNascimento (dd-mm-aaaa)\n");
                    printf("\n> ");
                    fgets(temp.nascimento, sizeof(temp.nascimento), stdin);
                    temp.nascimento[strcspn(temp.nascimento, "\n")] = '\0';

                    if (validarData(temp.nascimento) != 1)
                        printf("\nData em formato inválido, digite no formato (dd-mm-aaaa)\n");

                    else
                    {
                        strcpy(usuarios.lista[posicao].nascimento, temp.nascimento);
                        break;
                    }
                } while (1);
                break;
            case 4:
                printf("\nDigite o país:\n");
                printf("\n> ");
                fgets(usuarios.lista[posicao].pais, sizeof(usuarios.lista[posicao].pais), stdin);
                usuarios.lista[posicao].pais[strcspn(usuarios.lista[posicao].pais, "\n")] = '\0';
                printf("\nPaís editado com sucesso\n");
                break;
            case 999:
                break;
            default:
                printf("\nComando não encontrado\n");
                break;
            }
        }
    }
    else
        printf("\nNome não encontrado\n");
}

void jogoEdit()
{
    int posicao;
    char temp[23];

    printf("\nDigite o nome do jogo:\n");
    printf("\n> ");

    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';

    if ((posicao = jogoQuery(temp)) >= 0)
    {
        int escolha = 0;
        while (escolha != 999)
        {
            printf("\nVocê está no modo edição!\n");
            printf("\nDigite o campo que gostaria de editar: (1. Nome, 2. Desenvolvedora, 3. Data de Lançamento, 4. Genero, 999. Sair do modo edição)\n");
            printf("\n> ");
            scanf("%d", &escolha);
            while (getchar() != '\n')
                ;
            switch (escolha)
            {
            case 1:
                printf("\nDigite o do jogo: \n");
                printf("\n> ");
                {
                    char tempNome[24];
                    fgets(tempNome, sizeof(tempNome), stdin);
                    tempNome[strcspn(tempNome, "\n")] = '\0';
                    if (jogoQuery(tempNome) >= 0)
                        printf("\nJogo com nome já registrado!\n");
                    else
                    {
                        strcpy(jogos.lista[posicao].nome, tempNome);
                        printf("\nNome registrado com sucesso!\n");
                    }
                }
                break;
            case 2:
                printf("\nDigite o nome da desenvolvedora: \n");
                printf("\n> ");
                fgets(jogos.lista[posicao].desenvolvedora, sizeof(jogos.lista[posicao].desenvolvedora), stdin);
                jogos.lista[posicao].desenvolvedora[strcspn(jogos.lista[posicao].desenvolvedora, "\n")] = '\0';
                printf("\nDesenvolvedora editada com sucesso\n");
                break;
            case 3:
                do
                {
                    struct Jogo temp;
                    printf("\nLançamento (dd-mm-aaaa)\n");
                    printf("\n> ");
                    fgets(temp.data_lancamento, sizeof(temp.data_lancamento), stdin);
                    temp.data_lancamento[strcspn(temp.data_lancamento, "\n")] = '\0';

                    if (validarData(temp.data_lancamento) != 1)
                        printf("\nData em formato inválido, digite no formato (dd-mm-aaaa)\n");
                    else
                    {
                        strcpy(jogos.lista[posicao].data_lancamento, temp.data_lancamento);
                        printf("\nLançamento editado com sucesso\n");
                        break;
                    }
                } while (1);
                break;
            case 4:
                printf("\nDigite o genero do jogo: \n");
                printf("\n> ");
                fgets(jogos.lista[posicao].genero, sizeof(jogos.lista[posicao].genero), stdin);
                jogos.lista[posicao].genero[strcspn(jogos.lista[posicao].genero, "\n")] = '\0';
                printf("\nGenero editado com sucesso\n");
                break;
            case 999:
                break;
            default:
                printf("\nComando não encontrado\n");
                break;
            }
        }
    }
    else
        printf("\nJogo não encontrado!\n");
}

void recordeEdit()
{
    char id[24];

    printf("\nDigite o id do recorde:\n");
    printf("\n> ");

    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    struct Resultados resultado = recordeQuery("*", "*", id);

    if (resultado.tamanho > 0)
    {
        int escolha = 0;
        int posicao = resultado.lista[0];
        while (escolha != 999)
        {
            printf("\nVocê está no modo edição!\n");
            printf("\nDigite o campo que gostaria de editar: (1. Usuário, 2. Jogo, 3. Plataforma, 4. Tempo, 999. Sair do modo edição)\n");
            printf("\n> ");
            scanf("%d", &escolha);
            while (getchar() != '\n')
                ;
            char temp[32];

            switch (escolha)
            {
            case 1:
                printf("\nDigite o usuario desejado: \n");
                printf("\n> ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                if (usuarioQuery(temp) < 0)
                    printf("\nUsuário não encontrado\n");
                else
                {
                    strcpy(recordes.lista[posicao].usuario, usuarios.lista[usuarioQuery(temp)].apelido);
                    printf("\nUsuario editado com sucesso!\n");
                }
                break;
            case 2:
                printf("\nDigite o nome do jogo: \n");
                printf("\n> ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                if (jogoQuery(temp) < 0)
                    printf("\nJogo não encontrado!\n");
                else
                {
                    strcpy(recordes.lista[posicao].jogo, jogos.lista[jogoQuery(temp)].nome);
                    printf("\nNome editado com sucesso!\n");
                }
                break;
            case 3:
                printf("\nDigite a plataforma: \n");
                printf("\n> ");
                fgets(recordes.lista[posicao].plataforma, sizeof(recordes.lista[posicao].plataforma), stdin);
                recordes.lista[posicao].plataforma[strcspn(recordes.lista[posicao].plataforma, "\n")] = '\0';
                break;
            case 4:
                do
                {
                    int horas, minutos, segundos, milisecundos;
                    printf("\nTempo da Run (formato hh:mm:ss:msms):\n");
                    printf("\n> ");
                    scanf("%d:%d:%d:%d", &horas, &minutos, &segundos, &milisecundos);
                    while (getchar() != '\n')
                        ;

                    if (validarTempo(horas, minutos, segundos, milisecundos) != 1)
                        printf("\nTempo em formato inválido, digite no formato (hh:mm:ss:msms)\n");
                    else
                    {
                        recordes.lista[posicao].tempo = horas * 3600000ULL + minutos * 60000ULL + segundos * 1000ULL + milisecundos;
                        printf("\nTempo editado com sucesso!\n");
                        break;
                    }
                } while (1);
                break;
            case 999:
                break;
            default:
                printf("\nComando não encontrado\n");
                break;
            }
        }
    }
    else
        printf("\nRecorde não encontrado!\n");
}

void usuarioDelete()
{
    int posicao;
    char apelido[24];

    printf("\nDigite o apelido do usuario:\n");
    printf("\n> ");

    fgets(apelido, sizeof(apelido), stdin);
    apelido[strcspn(apelido, "\n")] = '\0';

    if ((posicao = usuarioQuery(apelido)) >= 0)
    {
        struct Resultados resultados = recordeQuery(apelido, "*", "*");
        if (resultados.tamanho == 0)
        {
            usuarios.lista[posicao] = usuarios.lista[usuarios.tamanho - 1];
            usuarios.lista = (struct Usuario *)realloc(usuarios.lista, --usuarios.tamanho * sizeof(struct Usuario));
            printf("\nUsuário deletado com sucesso!\n");
        }
        else
            printf("\nNão foi possível deletar este usuário, pois há recordes registrados com ele no banco de dados, delete-os primeiro");
    }
    else
        printf("\nApelido não encontrado\n");
}

void jogoDelete()
{
    int posicao;
    char nome[24];

    printf("\nDigite o nome do jogo: \n");
    printf("\n> ");

    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if ((posicao = jogoQuery(nome)) >= 0)
    {
        struct Resultados resultados = recordeQuery("*", nome, "*");
        if (resultados.tamanho == 0)
        {
            jogos.lista[posicao] = jogos.lista[jogos.tamanho - 1];
            jogos.lista = (struct Jogo *)realloc(jogos.lista, --jogos.tamanho * sizeof(struct Jogo));
            resultados = recordeQuery("*", jogos.lista[posicao].nome, "*");
            printf("\nJogo deletado com sucesso!\n");
        }
        else
            printf("\nNão foi possível deletar este jogo, pois há recordes registrados com ele no banco de dados, delete-os primeiro");
    }
    else
        printf("\nJogo nao encontrado!\n");
}

void recordeDelete()
{
    char id[24];

    printf("\nDigite identificador do recorde: \n");
    printf("\n> ");

    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    struct Resultados resultado = recordeQuery("*", "*", id);

    if (resultado.tamanho > 0)
    {
        recordes.lista[resultado.lista[0]] = recordes.lista[recordes.tamanho - 1];
        recordes.lista = (struct Recorde *)realloc(recordes.lista, --recordes.tamanho * sizeof(struct Recorde));
        printf("\nRecorde deletado com sucesso!\n");
    }
    else
        printf("\nRecorde não encontrado!\n");
}

void mostrarUsuarios()
{
    if (usuarios.tamanho != 0)
    {
        printf("\nUsuarios\n\n");
        printf("--------------------------\n");
        for (int i = 0; i < usuarios.tamanho; i++)
        {
            printf("\nApelido: %s\n", usuarios.lista[i].apelido);
            printf("\nEmail: %s\n", usuarios.lista[i].email);
            printf("\nNascimento: %s\n", usuarios.lista[i].nascimento);
            printf("\nPais: %s\n", usuarios.lista[i].pais);
            printf("--------------------------\n");
        }
    }
    else
        printf("\nNão há usuários adicionados\n");
}

void mostrarJogos()
{
    if (jogos.tamanho != 0)
    {
        printf("\nJogos\n\n");
        printf("\n---\n");
        for (int i = 0; i < jogos.tamanho; i++)
        {
            printf("\nNome: %s\n", jogos.lista[i].nome);
            printf("\nDesenvolvedora: %s\n", jogos.lista[i].desenvolvedora);
            printf("\nData_lancamento: %s\n", jogos.lista[i].data_lancamento);
            printf("\nGênero: %s\n", jogos.lista[i].genero);
            printf("--------------------------\n");
        }
    }
    else
        printf("\nNão há jogos adicionados\n");
}

struct Recordes consultaRecordes(char* nomeJogador, char* nomeJogo, char* identificacao_recorde)
{
    struct Recordes consulta;
    consulta.tamanho = 0;
    consulta.lista = NULL;

    struct Resultados resultados = recordeQuery(nomeJogador, nomeJogo, identificacao_recorde);

    for (int i = 0; i < resultados.tamanho; i++)
    {
        struct Recorde candidato = recordes.lista[resultados.lista[i]];

        int jaExiste = 0;
        for (int k = 0; k < consulta.tamanho; k++)
        {
            if (consulta.lista[k].identificacao == candidato.identificacao)
            {
                jaExiste = 1;
                break;
            }
        }

        if (!jaExiste)
        {
            consulta.lista = realloc(consulta.lista, (consulta.tamanho + 1) * sizeof(struct Recorde));
            consulta.lista[consulta.tamanho] = candidato;
            consulta.tamanho++;
        }
    }

    for (int i = 0; i < consulta.tamanho - 1; i++)
        for (int j = 0; j < consulta.tamanho - 1; j++)
            if (consulta.lista[j].tempo > consulta.lista[j + 1].tempo)
            {
                struct Recorde temp = consulta.lista[j];
                consulta.lista[j] = consulta.lista[j + 1];
                consulta.lista[j + 1] = temp;
            }
    
    return consulta;
}

void mostrarRecordes(struct Recordes consulta)
{
    char linha[128];

    for (int i = 0; i < consulta.tamanho; i++)
    {
        char *tempo_str = formatarTempo(consulta.lista[i].tempo);
        char *data_str = formatarData(consulta.lista[i].data_registro);

        snprintf(linha, sizeof(linha),
                 "%-4d| %-20s | %-15s | %-15s | %-9s | %-10s\n",
                 consulta.lista[i].identificacao,
                 consulta.lista[i].usuario,
                 consulta.lista[i].plataforma,
                 consulta.lista[i].jogo,
                 tempo_str,
                 data_str);

        printf("\n%s", linha);

        free(tempo_str);
        free(data_str);
    }
}

void interpretador(int prompt)
{
    char ajuda[] = "[0] = Ajuda\n[1] = Adicionar elementos\n[2] = Editar elementos\n[3] = Deletar elementos\n[4] = Consultar\n[999] = Finalizar Programa\n";

    switch (prompt)
    {
    case 0:
        printf("\n%s", ajuda);
        break;
    case 1:
        adicionar();
        break;
    case 2:
        editar();
        break;
    case 3:
        deletar();
        break;
    case 4:
        consultar();
        break;
    case 999:
        serializarAlteracoes();
        printf("\nAdeus!\n");
        break;
    default:
        printf("\nComando não encontrado\n");
        break;
    }
}
