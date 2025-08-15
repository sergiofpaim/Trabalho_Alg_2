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

char* formatarTempo(unsigned long long int ms) 
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

char* formatarData(time_t data)
{
    struct tm *tm_info = localtime(&data);
    char *saida = malloc(11);

    strftime(saida, 11, "%d/%m/%Y", tm_info);

    return saida;
}

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

int jogoQuery(char *identificacao) 
{
    for (int i = 0; i < jogos.tamanho; i++) 
        if (strcmp(jogos.lista[i].nome, identificacao) == 0) return i;

    return -1;
}

void userAdd()
{
    struct Usuario temp;
    int nomeExistente = 0;

    printf("Digite as informa√ß√µes do usu√°rio:\n");
    do
    {
        printf("\nApelido\n");
        printf("> ");
        scanf("%23s", temp.apelido);
    
        if ((nomeExistente = userQuery(temp.apelido)) >= 0)
            printf("\nNome j√° existe no banco de dados\n");
    }
    while (nomeExistente >= 0);
    //userQuery retorna negativo caso n√£o exista o usu√°rio

    printf("\nEmail\n");
    printf("> ");
    scanf("%31s", temp.email);
    printf("\nNascimento (dd-mm-aaaa)\n");
    printf("> ");
    scanf("%10s", temp.nascimento);
    printf("\nPa√≠s\n");
    printf("> ");
    scanf("%11s", temp.pais);

    usuarios.tamanho++;
    usuarios.lista = (struct Usuario *) realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
    if (usuarios.lista != NULL) {
        usuarios.lista[usuarios.tamanho - 1] = temp;
    } else {
        printf("Erro ao alocar mem√≥ria para novo usu√°rio.\n");
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
                printf("\nVoc√™ est√° no modo edi√ß√£o\n");
                printf("\nDigite o campo que gostaria de editar (1. Apelido, 2. Email, 3. Data de Nascimento, 4. Pa√≠s, 999. Sair da edi√ß√£o)\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                    case 1:
                        printf("\nDigite o nome:\n");
                        printf("> ");
                        char temp[24];
                        scanf("%23s", temp);

                        // Confere se o nome j√° existe no banco de dados
                        if (userQuery(temp) >= 0) printf("\nNome j√° existe\n");
                        else
                        {
                            strcpy(usuarios.lista[posicao].apelido, temp);
                            printf("\nNome editado com sucesso\n");
                        }

                        break;
                    case 2:
                        printf("\nDigite o email:\n");
                        printf("> ");
                        scanf("%31s", usuarios.lista[posicao].email);
                        printf("\nEmail editado com sucesso\n");
                        break;
                    case 3:
                        printf("\nDigite a data de nascimento:\n");
                        printf("> ");
                        scanf("%10s", usuarios.lista[posicao].nascimento);
                        printf("\nNascimento editado com sucesso\n");
                        break;
                    case 4:
                        printf("\nDigite o pa√≠s:\n");
                        printf("> ");
                        scanf("%11s", usuarios.lista[posicao].pais);
                        printf("\nPa√≠s editado com sucesso\n");
                        break;
                    case 999:
                        break;
                    default:
                        printf("\nComando n√£o encontrado\n");
                        break;
                }
            }
    }
    else printf("\nNome n√£o encontrado\n");
}

void userDelete()
{
    int posicao;

    printf("Digite o apelido do usuario:\n");
    char temp[24];
    printf("> ");
    scanf("%23s", temp);
    
    //TODO: Verificar se existe recorde antes de apagar
    if ((posicao = userQuery(temp)) >= 0)
    {
        usuarios.lista[posicao] = usuarios.lista[usuarios.tamanho - 1]; 
        usuarios.lista = (struct Usuario*) realloc(usuarios.lista, --usuarios.tamanho * sizeof(struct Usuario));
        //TODO: Pegar todos os recordes do usuario da ultima posicao para a posicao atual
        printf("Usu·rio deletado com sucesso!\n");
    }
    else printf("\nNome n√£o encontrado\n");
}

void jogoAdd()
{
    struct Jogo temp;
    int jogoExistente = 0;

    printf("Digite as informaÁıes do jogo:\n");

    do 
    {
        printf("\nNome\n");
        printf("> ");
        scanf("%23s", temp.nome);

        if ((jogoExistente = jogoQuery(temp.nome)) >= 0) 
            printf("\nO jogo j· existe no banco de dados\n");
    } 
    while (jogoExistente >= 0);

    printf("\nDesenvolvedores\n");
    printf("> ");
    scanf("%31s", temp.desenvolvedora);
    printf("\nLan√ßamento (dd-mm-aaaa)\n");
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
        printf("Erro ao alocar mem√≥ria para novo jogo");
        jogos.tamanho--;
    }
}

void jogoEdit()
{
    int posicao;

    printf("Digite o nome do jogo:\n");
    char temp[23];
    printf("> ");
    scanf("%23s", temp);

    if ((posicao = jogoQuery(temp)) >= 0) 
    {
        int escolha = 0;
        while (escolha != 999) 
        {
            printf("\nVocÍ est· no modo ediÁ„o!\n");
            printf("Digite o campo que gostaria de editar: (1. Nome, 2. Desenvolvedora, 3. Data de LanÁamento, 4. Plataforma, 999. Sair do modo ediÁ„o)\n");
            scanf("%d", &escolha);

            switch (escolha) 
            {
                case 1:
                    printf("Digite o nome desejado: \n");
                    printf("> ");
                    char temp[24];
                    scanf("%s", &temp);

                    if (jogoQuery(temp) >= 0) printf("Nome j· registrado!\n");
                    else { 
                        strcpy(jogos.lista[posicao].nome, temp);
                        printf("Nome registrado com sucesso!\n");
                    }
                    break;

                case 2:
                    printf("Digite o nome da desenvolvedora: \n");
                    printf("> ");
                    scanf("%31s", jogos.lista[posicao].desenvolvedora);
                    break;

                case 3:
                    printf("Digite a data de lanÁamento: \n");
                    printf("> ");
                    scanf("%10s", jogos.lista[posicao].data_lancamento);
                    break;

                case 4:
                    printf("Digite o nome da plataforma: \n");
                    printf("> ");
                    scanf("%11s", jogos.lista[posicao].plataforma);
                    break;

                case 999:
                    break;

                default: 
                    printf("\nComando n„o encontrado\n");
                    break;
            }
        }
    } else printf("Jogo n„o encontrado!\n");
}

void jogoDelete() 
{
    int posicao;

    printf("\nDigite o nome do jogo: \n");
    char nome[24];
    scanf("%23s", nome);

    //TODO: Verificar se existe recorde antes de apagar
    if ((posicao = jogoQuery(nome)) >= 0) 
    {
        jogos.lista[posicao] = jogos.lista[jogos.tamanho - 1];
        jogos.lista = (struct Jogo*) realloc (jogos.lista, --jogos.tamanho * sizeof(struct Jogo));
        //TODO: Pegar todos os recordes do usuario da ultima posicao para a posicao atual
        printf("Jogo deletado com sucesso!\n");
    }
    else printf("Jogo n„o encontrado!\n");
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
    scanf("%23s %23s %31s", nomeJogador, nomeJogo, identificacao_recorde);

    if (identificacao_recorde[0] != '*')
    {
        for (int i = 0; i < recordes.tamanho; i++)
            if (((strcmp(recordes.lista[i].usuario->apelido, nomeJogador) == 0) || (strcmp(nomeJogador, "*") == 0)) &&
                ((strcmp(recordes.lista[i].jogo->nome, nomeJogo) == 0) || (strcmp(nomeJogo, "*") == 0)))
            {
                consulta.lista = (struct Recorde*) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
                consulta.lista[consulta.tamanho - 1] = recordes.lista[i];
            }

        if (consulta.tamanho == 0) printf("\nRecorde n√£o encontrado\n");
    }
    else if (nomeJogador[0] != '*' && nomeJogo[0] != '*' && identificacao_recorde[0] == '*')
    {
        int posicao;
        if ((posicao = recordeQuery(identificacao_recorde)) >= 0) 
            //recordeQuery retorna negativo caso n√£o exista o recorde
            {
                consulta.lista = (struct Recorde*) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
                consulta.lista[consulta.tamanho - 1] = recordes.lista[posicao];
            }
            else printf("\nRecorde n√£o encontrado\n");
    }
    else dump();

    // "Recordes:\n" 11
    // "1. [identificador] [nomeJogador] [pais] [jogo] [tempo] [data_registro]\n" 11 + 24 + 12 + 24 + 12 + 10 = 93 * consulta.tamanho 

    const int TAMANHO_LINHA_RECORDE = 128;
    int tamanho_buffer = 11 + TAMANHO_LINHA_RECORDE * consulta.tamanho; 
    char *buffer = malloc(tamanho_buffer * sizeof(char));
    char tmp[TAMANHO_LINHA_RECORDE];

    for (int i = 0; i < consulta.tamanho; i++)
    {
        char *tempo_str = formatarTempo(consulta.lista[i].tempo);
        char *data_str = formatarData(consulta.lista[i].data_registro);
        
        snprintf(tmp, sizeof(tmp), 
                 "%d. %s %s %s %s %s %s\n", 
                 consulta.lista[i].identificacao, 
                 consulta.lista[i].usuario->apelido, 
                 consulta.lista[i].usuario->pais, 
                 consulta.lista[i].jogo->nome, 
                 tempo_str, 
                 data_str);
        
        strncat(buffer, tmp, sizeof(buffer) - strlen(buffer) - 1);
        
        free(tempo_str);
        free(data_str);
    }

    printf("%s", buffer);
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
        case 5:
            jogoEdit();
            break;
        case 6:
            jogoDelete();
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
            printf("Comando n√£o encontrado\n");
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
