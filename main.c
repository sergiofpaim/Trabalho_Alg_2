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

void UserAdd()
{
    struct Usuario temp;

    printf("Digite as informações do usuário:\n");
    printf("\nApelido\n");
    printf("> ");
    scanf("%23s", temp.apelido);
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

void JogoAdd()
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
            printf("---\n")
            printf("Nome: %s\n", jogos.lista[i].nome);
            printf("desenvolvedora: %s\n", jogos.lista[i].desenvolvedora);
            printf("data_lancamento: %s\n", jogos.lista[i].data_lancamento);
            printf("plataforma: %s\n", jogos.lista[i].plataforma);
            printf("---\n")
        }
    }
    if (usuarios.tamanho != 0)
    {
        printf("\nUsuarios\n");
        for (int i = 0; i < usuarios.tamanho; i++){
            printf("---\n")
            printf("Apelido: %s\n", usuarios.lista[i].apelido);
            printf("Email: %s\n", usuarios.lista[i].email);
            printf("nascimento: %s\n", usuarios.lista[i].nascimento);
            printf("pais: %s\n", usuarios.lista[i].pais);
            printf("---\n")
        }
    }
}

void interpretador(int prompt)
{
    char ajuda[] = "[0] = Ajuda\n[1] = Adicionar Jogador\n[2] = Editar Jogador\n[3] = Deletar Jogador\n[4] = Adicionar Jogo\n[5] = Editar Jogo\n[6] = Deletar Jogo\n[7] = Adicionar Recorde\n[8] = Editar Recorde\n[9] = Remover Recorde\n[999] = Finalizar Programa\n";

    switch (prompt){
        case 0:
            printf("%s", ajuda);
            break;
        case 1:
            UserAdd();
            break;
        case 4:
            JogoAdd();
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
