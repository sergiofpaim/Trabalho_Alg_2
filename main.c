#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

struct Usuario
{
    char apelido[24];
    char email[32];
    char nascimento[11];
    char pais[12];
};

struct Jogo
{
    char nome[24];
    char desenvolvedora[32];
    char data_lancamento[24];
    char plataforma[12];
};

struct Recorde
{
    struct Usuario *usuario;
    struct Jogo *jogo;
    char data_lancamento[24];
    char plataforma[12];
    unsigned long long int tempo;
    time_t data_registro;
    char identificacao[32];
};

struct Usuarios
{
    int tamanho;
    struct Usuario *lista;
};

struct Jogos
{
    int tamanho;
    struct Jogo *lista;
};

struct Recordes
{
    int tamanho;
    struct Recorde *lista;
};

struct Usuarios usuarios;
struct Jogos jogos;
struct Recordes recordes;

int userQuery(char *identificacao)
{
    for (int i = 0; i < usuarios.tamanho; i++)
        if (strcmp(usuarios.lista[i].apelido, identificacao) == 0) return i;
    
    return -1;
}

int recordeQuery(char *identificacao)
{
    for (int i = 0; i < recordes.tamanho; i++)
        if (strcmp(recordes.lista[i].identificacao, identificacao) == 0) return i;
    
    return -1;
}

void userAdd()
{
    struct Usuario temp;
    int nomeExistente = 0;

    printf("Digite as informações do usuário:\n");
    do
    {
        printf("\nApelido\n");
        printf("> ");
        scanf("%23s", temp.apelido);
    
        if ((nomeExistente = userQuery(temp.apelido)) >= 0)
            printf("\nNome já existe no banco de dados\n");
    }
    while (nomeExistente >= 0);
    //userQuery retorna negativo caso não exista o usuário

    printf("\nEmail\n");
    printf("> ");
    scanf("%31s", temp.email);
    printf("\nNascimento (dd-mm-aaaa)\n");
    printf("> ");
    scanf("%10s", temp.nascimento);
    printf("\nPaís\n");
    printf("> ");
    scanf("%11s", temp.pais);

    usuarios.tamanho++;
    usuarios.lista = (struct Usuario *) realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
    if (usuarios.lista != NULL) {
        usuarios.lista[usuarios.tamanho - 1] = temp;
    } else {
        printf("Erro ao alocar memória para novo usuário.\n");
        usuarios.tamanho--;
    }
}

void userEdit()
{
    int posicao;

        printf("Digite o apelido do usuario:\n");
        char temp[24];
        printf("> ");
        scanf("%23s", temp);
    
    if ((posicao = userQuery(temp)) >= 0)
    {
            int escolha = 0;
            while (escolha != 999)
                {
                printf("\nVocê está no modo edição\n");
                printf("\nDigite o campo que gostaria de editar (1. Apelido, 2. Email, 3. Data de Nascimento, 4. País, 999. Sair da edição)\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 1:
                    char temp[24];
                    printf("\nDigite o nome:\n");
                    printf("> ");
                    scanf("%23s", temp);

                    // Confere se o nome já existe no banco de dados
                    if (userQuery(temp) >= 0) printf("\nNome já existe\n");
                    else
                    {
                        strcpy(usuarios.lista[posicao].apelido, temp);
                        printf("\nNome editado com sucesso\n");
                    }

                    break;
                case 2:
                    printf("\nDigite o email:\n");
                    printf("> ");
                    scanf("%23s", usuarios.lista[posicao].email);
                    printf("\nEmail editado com sucesso\n");
                    break;
                case 3:
                    printf("\nDigite a data de nascimento:\n");
                    printf("> ");
                    scanf("%23s", usuarios.lista[posicao].nascimento);
                    printf("\nNascimento editado com sucesso\n");
                    break;
                case 4:
                    printf("\nDigite o país:\n");
                    printf("> ");
                    scanf("%23s", usuarios.lista[posicao].pais);
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
    else printf("\nNome não encontrado\n");
}

void userDelete()
{
    int posicao;

    printf("Digite o apelido do usuario:\n");
    char temp[24];
    printf("> ");
    scanf("%23s", temp);
    
    if ((posicao = userQuery(temp)) >= 0)
    {
        usuarios.lista[posicao] = usuarios.lista[usuarios.tamanho - 1]; 
        usuarios.lista = (struct Usuario*) realloc(usuarios.lista, --usuarios.tamanho * sizeof(struct Usuario));
        //TODO: Pegar todos os recordes do usuario da ultima posicao para a posicao atual
    }
    else printf("\nNome não encontrado\n");
}

void jogoAdd()
{
    struct Jogo temp;

    printf("Digite as informações do jogo:\n");
    printf("\nNome\n");
    printf("> ");
    scanf("%23s", temp.nome);
    printf("\nDesenvolvedores\n");
    printf("> ");
    scanf("%31s", temp.desenvolvedora);
    printf("\nLançamento (dd-mm-aaaa)\n");
    printf("> ");
    scanf("%10s", temp.data_lancamento);
    printf("\nPlatafoma\n");
    printf("> ");
    scanf("%11s", temp.plataforma);

    jogos.tamanho++;
    jogos.lista = (struct Jogo *) realloc(jogos.lista, jogos.tamanho * sizeof(struct Jogo));
    if (jogos.lista != NULL) jogos.lista[jogos.tamanho - 1] = temp;
    else
    {
        printf("Erro ao alocar memória para novo jogo");
        jogos.tamanho--;
    }
}

void dump() // Temporario para debugar com mais facilidade comando 123
{
    if (jogos.tamanho != 0)
    {
        printf("\nJogos\n");
        for (int i = 0; i < jogos.tamanho; i++){
            printf("---\n");
            printf("Nome: %s\n", jogos.lista[i].nome);
            printf("desenvolvedora: %s\n", jogos.lista[i].desenvolvedora);
            printf("data_lancamento: %s\n", jogos.lista[i].data_lancamento);
            printf("plataforma: %s\n", jogos.lista[i].plataforma);
            printf("---\n");
        }
    }
    if (usuarios.tamanho != 0)
    {
        printf("\nUsuarios\n");
        for (int i = 0; i < usuarios.tamanho; i++){
            printf("---\n");
            printf("Apelido: %s\n", usuarios.lista[i].apelido);
            printf("Email: %s\n", usuarios.lista[i].email);
            printf("nascimento: %s\n", usuarios.lista[i].nascimento);
            printf("pais: %s\n", usuarios.lista[i].pais);
            printf("---\n");
        }
    }
}

void consulta()
{
    char nomeJogador[24];
    char nomeJogo[24];
    char identificacao_recorde[32];

    struct Recordes consulta;
    consulta.tamanho = 0;
    consulta.lista = (struct Recorde*) malloc(consulta.tamanho * sizeof(struct Recorde));

    printf("\nDigite a sua consulta neste formato <id_jogador nome_jogo identificacao_recorde> ou '*' para todos daquela posicao (no caso de busca de recorde, colocar '*' em jogador e jogo)\n");
    scanf("%s %s %s", nomeJogador, nomeJogo, identificacao_recorde);

    if (identificacao_recorde[0] != '*')
    {
        for (int i = 0; i < recordes.tamanho; i++)
            if (((strcmp(recordes.lista[i].usuario->apelido, nomeJogador) == 0) || (strcmp(nomeJogador, "*") == 0)) &&
                ((strcmp(recordes.lista[i].jogo->nome, nomeJogo) == 0) || (strcmp(nomeJogo, "*") == 0)))
            {
                consulta.lista = (struct Recorde*) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
                consulta.lista[consulta.tamanho - 1] = recordes.lista[i];
            }
            else printf("\nRecorde não encontrado\n");
    }
    else if (nomeJogador[0] != '*' && nomeJogo[0] != '*' && identificacao_recorde[0] == '*')
    {
        int posicao;
        if ((posicao = recordeQuery(identificacao_recorde)) >= 0)
            {
                consulta.lista = (struct Recorde*) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
                consulta.lista[consulta.tamanho - 1] = recordes.lista[posicao];
            }
            else printf("\nRecorde não encontrado\n");
    }
    else
        dump();
}

void interpretador(int prompt)
{
    char ajuda[] = "[0] = Ajuda\n[1] = Adicionar Jogador\n[2] = Editar Jogador\n[3] = Deletar Jogador\n[4] = Adicionar Jogo\n[5] = Editar Jogo\n[6] = Deletar Jogo\n[7] = Adicionar Recorde\n[8] = Editar Recorde\n[9] = Remover Recorde\n[10] = Consultar\n[999] = Finalizar Programa\n";

    switch (prompt){
        case 0:
            printf("%s", ajuda);
            break;
        case 1:
            userAdd();
            break;
        case 2:
            userEdit();
            break;
        case 3:
            userDelete();
            break;
        case 4:
            jogoAdd();
            break;
        case 10:
            consulta();
            break;
        case 123:
            dump();
            break;
        case 999:
            printf("Adeus!\n");
            break;
        default:
            printf("Comando não encontrado\n");
            break;
    }
}

void main()
{
    usuarios.tamanho = 0;
    usuarios.lista = (struct Usuario*)malloc(usuarios.tamanho * sizeof(struct Usuario));

    jogos.tamanho = 0;
    jogos.lista = (struct Jogo*)malloc(jogos.tamanho * sizeof(struct Jogo));

    recordes.tamanho = 0;
    recordes.lista = (struct Recorde*)malloc(recordes.tamanho * sizeof(struct Recorde));


    int prompt = 0;

    printf("Bem vindo ao Speed Runners!\n");

    while (prompt != 999)
    {
        printf("Digite o comando, ou 0 para ajuda\n");
        printf("> ");
        scanf("%d", &prompt);

        interpretador(prompt);
    }
}
