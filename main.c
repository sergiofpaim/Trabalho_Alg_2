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
    char plataforma[32];
};

struct Recorde
{
    struct Usuario *usuario;
    struct Jogo *jogo;
    char data_lancamento[24];
    char plataforma[32];
    unsigned long long int tempo;
    time_t data_registro;
    int identificacao;
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

struct Resultados
{
    int tamanho, *lista;
};

struct Usuarios usuarios;
struct Jogos jogos;
struct Recordes recordes;
int idrecordes;

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


int jogoQuery(char *identificacao) 
{
    for (int i = 0; i < jogos.tamanho; i++) 
        if (strcmp(jogos.lista[i].nome, identificacao) == 0) return i;

    return -1;
}

struct Resultados recordeQuery(char* nomeJogador, char* nomeJogo, char* identificacao_recorde)
{
    struct Resultados resultados;
    resultados.tamanho = 0;
    resultados.lista = (int *) malloc(resultados.tamanho * sizeof(int));
    for (int i = 0; i <= recordes.tamanho - 1; i++){
        int achouId = (strcmp(identificacao_recorde, "*") == 0) || (recordes.lista[i].identificacao == atoi(identificacao_recorde));
        int achouJogador = (strcmp(nomeJogador, "*") == 0) || (strcmp(nomeJogador, recordes.lista[i].usuario->apelido) == 0);
        int achouJogo = (strcmp(nomeJogo, "*") == 0) || (strcmp(nomeJogo, recordes.lista[i].jogo->nome) == 0);

        if (achouId && achouJogador && achouJogo){
            resultados.lista = (int*) realloc(resultados.lista, ++resultados.tamanho * sizeof(int));
            resultados.lista[resultados.tamanho-1] = i;
        }
    }

    return resultados;
}

void userAdd()
{
    struct Usuario temp;;
    int nomeExistente = 0;

    printf("Digite as informaÃ§Ãµes do usuÃ¡rio:\n");
    do
    {
        printf("\nApelido\n");
        printf("> ");
        scanf("%23s", temp.apelido);
    
        if ((nomeExistente = userQuery(temp.apelido)) >= 0)
            printf("\nNome jÃ¡ existe no banco de dados\n");
    }
    while (nomeExistente >= 0);
    //userQuery retorna negativo caso nÃ£o exista o usuÃ¡rio

    printf("\nEmail\n");
    printf("> ");
    scanf("%31s", temp.email);
    printf("\nNascimento (dd-mm-aaaa)\n");
    printf("> ");
    scanf("%10s", temp.nascimento);
    printf("\nPaÃ­s\n");
    printf("> ");
    scanf("%11s", temp.pais);

    usuarios.tamanho++;
    usuarios.lista = (struct Usuario *) realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
    if (usuarios.lista != NULL) {
        usuarios.lista[usuarios.tamanho - 1] = temp;
    } else {
        printf("Erro ao alocar memÃ³ria para novo usuÃ¡rio.\n");
        usuarios.tamanho--;
    }
}

void jogoAdd()
{
    struct Jogo temp;
    int jogoExistente = 0;

    printf("Digite as informações do jogo:\n");

    do 
    {
        printf("\nNome\n");
        printf("> ");
        scanf("%23s", temp.nome);

        if ((jogoExistente = jogoQuery(temp.nome)) >= 0) 
            printf("\nO jogo já existe no banco de dados\n");
    } 
    while (jogoExistente >= 0);

    printf("\nDesenvolvedores\n");
    printf("> ");
    scanf("%31s", temp.desenvolvedora);
    printf("\nLanÃ§amento (dd-mm-aaaa)\n");
    printf("> ");
    scanf("%10s", temp.data_lancamento);
    printf("\nPlatafoma\n");
    printf("> ");
    scanf("%11s", temp.plataforma);

    jogos.lista = (struct Jogo *) realloc(jogos.lista, ++jogos.tamanho * sizeof(struct Jogo));
    if (jogos.lista != NULL) jogos.lista[jogos.tamanho - 1] = temp;
    else
    {
        printf("Erro ao alocar memÃ³ria para novo jogo");
        jogos.tamanho--;
    }
}

void recordeAdd()
{
    struct Recorde temp;
    char player[24], jogo[24];
    int horas, minutos, segundos, milisecundos;

    printf("\nDigite as informações do recorde:\n");
    printf("Nome do usuário: \n");
    printf("> ");
    scanf("%s", player);
    if (userQuery(player) >= 0) temp.usuario = &(usuarios.lista[userQuery(player)]);
    else
    {
        printf("Usuário não encontrado!\n");
        return;
    }
    printf("Nome do jogo:\n");
    printf("> ");
    scanf("%s", jogo);
    if (jogoQuery(jogo) >= 0) temp.jogo = &(jogos.lista[jogoQuery(jogo)]);
    else 
    {
        printf("Jogo não encontrado!\n");
        return;
    }
    printf("Plataforma do Recorde:\n");
    printf("> ");
    scanf("%s", temp.plataforma);
    printf("Tempo da Run (formato hh:mm:ss:msms):\n");
    printf("> ");
    scanf("%d:%d:%d:%d", &horas, &minutos, &segundos, &milisecundos);
    temp.tempo = horas * 3600000ULL + minutos * 60000ULL + segundos * 1000ULL + milisecundos;
    temp.data_registro = time(NULL);
    idrecordes++;
    temp.identificacao = idrecordes;

    recordes.lista = (struct Recorde *) realloc(recordes.lista, ++recordes.tamanho * sizeof(struct Recorde));
    if (recordes.lista != NULL) recordes.lista[recordes.tamanho - 1] = temp;
    else
    {
        printf("Erro ao alocar memória para o recorde\n");
        recordes.tamanho--;
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
                printf("\nVocÃª estÃ¡ no modo ediÃ§Ã£o\n");
                printf("\nDigite o campo que gostaria de editar (1. Apelido, 2. Email, 3. Data de Nascimento, 4. PaÃ­s, 999. Sair da ediÃ§Ã£o)\n");
                printf("> ");
                scanf("%d", &escolha);
                switch (escolha)
                {
                    case 1:
                        printf("\nDigite o nome:\n");
                        printf("> ");
                        char temp[24];
                        scanf("%23s", temp);

                        // Confere se o nome jÃ¡ existe no banco de dados
                        if (userQuery(temp) >= 0) printf("\nNome jÃ¡ existe\n");
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
                        printf("\nDigite o paÃ­s:\n");
                        printf("> ");
                        scanf("%11s", usuarios.lista[posicao].pais);
                        printf("\nPaÃ­s editado com sucesso\n");
                        break;
                    case 999:
                        break;
                    default:
                        printf("\nComando nÃ£o encontrado\n");
                        break;
                }
            }
    }
    else printf("\nNome nÃ£o encontrado\n");
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
            printf("\nVocê está no modo edição!\n");
            printf("Digite o campo que gostaria de editar: (1. Nome, 2. Desenvolvedora, 3. Data de Lançamento, 4. Plataforma, 999. Sair do modo edição)\n");
            printf("> ");
            scanf("%d", &escolha);
            switch (escolha) 
            {
                case 1:
                    printf("Digite o nome desejado: \n");
                    printf("> ");
                    char temp[24];
                    scanf("%s", &temp);
                    if (jogoQuery(temp) >= 0) printf("Nome já registrado!\n");
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
                    printf("Digite a data de lançamento: \n");
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
                    printf("\nComando não encontrado\n");
                    break;
            }
        }
    } else printf("Jogo não encontrado!\n");
}

void recordeEdit()
{
    printf("Digite o id do recorde:\n");
    char id[24];
    printf("> ");
    scanf("%s", id);
    struct Resultados resultado = recordeQuery("*", "*", id);

    if (resultado.tamanho > 0) 
    {
        int escolha = 0;
        int posicao = resultado.lista[0];
        while (escolha != 999) 
        {
            printf("\nVocê está no modo edição!\n");
            printf("Digite o campo que gostaria de editar: (1. Usuário, 2. Jogo, 3. plataforma, 4. Tempo, 999. Sair do modo edição)\n");
            printf("> ");
            scanf("%d", &escolha);
            char temp[32];

            switch (escolha) 
            {
                case 1:
                    printf("Digite o usuario desejado: \n");
                    printf("> ");
                    scanf("%s", &temp);
                    if (userQuery(temp) < 0) printf("Usuário não encontrado\n");
                    else { 
                        recordes.lista[posicao].usuario = &(usuarios.lista[userQuery(temp)]);
                        printf("Recorde editado com sucesso!\n");
                    }
                    break;
                case 2:
                    printf("Digite o nome do jogo: \n");
                    printf("> ");
                    scanf("%31s", temp);
                    if (jogoQuery(temp) < 0) printf("Jogo não encontrado!\n");
                    else
                    {
                        recordes.lista[posicao].jogo = &(jogos.lista[jogoQuery(temp)]);
                        printf("Recorde editado com sucesso!\n");
                    }
                    break;
                case 3:
                    printf("Digite a plataforma: \n");
                    printf("> ");
                    scanf("%10s", recordes.lista[posicao].plataforma);
                    break;
                case 4:
                    int horas, minutos, segundos, milisecundos;
                    printf("Digite o tempo (HH:MM:SS:mmm): \n");
                    printf("> ");
                    scanf("%d:%d:%d:%d", &horas, &minutos, &segundos, &milisecundos);
                    recordes.lista[posicao].tempo = horas * 3600000ULL + minutos * 60000ULL + segundos * 1000ULL + milisecundos; 
                    break;
                case 999:
                    break;
                default: 
                    printf("\nComando não encontrado\n");
                    break;
            }
        }
    } else printf("Jogo não encontrado!\n");
}

//TODO: Arrumar a atualização dos ponteiros pós exclusão
void userDelete()
{
    int posicao;

    printf("Digite o apelido do usuario:\n");
    char temp[24];
    printf("> ");
    scanf("%23s", temp);
    
    struct Resultados resultados = recordeQuery(temp, "*", "*");
    if (resultados.tamanho > 0)
        if ((posicao = userQuery(temp)) >= 0)
        {
            struct Resultados resultados = recordeQuery(usuarios.lista[usuarios.tamanho - 1].apelido, "*", "*");
            usuarios.lista[posicao] = usuarios.lista[usuarios.tamanho - 1]; 
            usuarios.lista = (struct Usuario*) realloc(usuarios.lista, --usuarios.tamanho * sizeof(struct Usuario));
            
            for (int i = 0; i < resultados.tamanho; i++) // Atualizando o ponteiro dos recordes
                recordes.lista[resultados.lista[i]].usuario = &(usuarios.lista[posicao]);
            
            printf("Usuário deletado com sucesso!\n");
        }
        else printf("\nNome nÃ£o encontrado\n");
    else printf("Usuário não pode ser deletado pois tem um recorde registrado com o id %d\n", recordes.lista[resultados.lista[0]].identificacao);
}

//TODO: Arrumar a atualização dos ponteiros pós exclusão
void jogoDelete() 
{
    int posicao;

    printf("\nDigite o nome do jogo: \n");
    char nome[24];
    scanf("%23s", nome);
    struct Resultados resultados = recordeQuery("*", nome, "*");
    
    if (resultados.tamanho > 0)
    {
        if((posicao = jogoQuery(nome)))
        {
            jogos.lista[posicao] = jogos.lista[jogos.tamanho - 1];
            jogos.lista = (struct Jogo*) realloc (jogos.lista, --jogos.tamanho * sizeof(struct Jogo));
            resultados = recordeQuery("*", jogos.lista[posicao].nome, "*");

            for (int i = 0; i < resultados.tamanho; i++) // Atualizando o ponteiro dos recordes
                recordes.lista[resultados.lista[i]].jogo = &(jogos.lista[posicao]);

            printf("Jogo deletado com sucesso!\n");
        }
        else printf("Jogo não encontrado!\n");
    }
    else printf("Usuário não pode ser deletado pois tem um recorde registrado com o id %d\n", recordes.lista[resultados.lista[0]].identificacao);
}

void recordeDelete() 
{
    char id[24];
    printf("\nDigite identificador do recorde: \n");
    printf("> ");
    scanf("%s", id);
    struct Resultados resultado = recordeQuery("*", "*", id);

    if (resultado.tamanho > 0) 
    {
        recordes.lista[resultado.lista[0]] = recordes.lista[recordes.tamanho - 1];
        recordes.lista = (struct Recorde *) realloc(recordes.lista, --recordes.tamanho * sizeof(struct Recorde));
        printf("Recorde deletado com sucesso!\n");
    }
    else printf("Recorde não encontrado!\n");
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
    if (recordes.tamanho != 0)
    {
        printf("\nrecordes\n");
        for (int i = 0; i < recordes.tamanho; i++){
            printf("---\n");
            printf("identificador: %d\n", recordes.lista[i].identificacao);
            printf("Usuário: %s\n", recordes.lista[i].usuario->apelido);
            printf("Jogo: %s\n", recordes.lista[i].jogo->nome);
            printf("Plataforma: %s\n", recordes.lista[i].plataforma);
            printf("Tempo: %s\n", formatarTempo(recordes.lista[i].tempo));
            printf("Data de Registro: %s\n", formatarData(recordes.lista[i].tempo));
            printf("---\n");
        }
    }
}

void consulta()
{
    char nomeJogador[24];
    char nomeJogo[24];
    char identificacao_recorde[32];
    int identificador, achou = 0;
    struct Recordes consulta;
    consulta.tamanho = 0;
    consulta.lista = (struct Recorde*) malloc(consulta.tamanho * sizeof(struct Recorde));

    printf("\nDigite a sua consulta neste formato <id_jogador nome_jogo identificacao_recorde> ou '*' para todos daquela posicao\n");
    printf("> ");
    scanf("%23s %23s %31s", nomeJogador, nomeJogo, identificacao_recorde);
    struct Resultados resultados = recordeQuery(nomeJogador, nomeJogo, identificacao_recorde);
    
    if (resultados.tamanho == 0)
    {
        printf("Nenhum recorde encontrado!\n");
        return;
    }
 
    for (int i = 0; i < resultados.tamanho; i++)
    {
        consulta.lista = (struct Recorde *) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
        consulta.lista[consulta.tamanho - 1] = recordes.lista[resultados.lista[i]];
    }

    // "Recordes:\n" 11
    // "1. [identificador] [nomeJogador] [pais] [jogo] [tempo] [data_registro]\n" 11 + 24 + 12 + 24 + 12 + 10 = 93 * consulta.tamanho 

    //Bubble Sort
    for (int i = 0; i < consulta.tamanho - 1; i++)
        for (int j = 0; j < consulta.tamanho -1; j++)
            if (consulta.lista[j].tempo > consulta.lista[j+1].tempo)
            {
                struct Recorde temp = consulta.lista[j];
                consulta.lista[j] = consulta.lista[j+1];
                consulta.lista[j+1] = temp;
            }

    char linha[128];

    for (int i = 0; i < consulta.tamanho; i++)
    {
        char *tempo_str = formatarTempo(consulta.lista[i].tempo);
        char *data_str = formatarData(consulta.lista[i].data_registro);
        
        snprintf(linha, sizeof(linha), 
                 "%d. %s %s %s %s %s\n", 
                 consulta.lista[i].identificacao, 
                 consulta.lista[i].usuario->apelido, 
                 consulta.lista[i].usuario->pais, 
                 consulta.lista[i].jogo->nome, 
                 tempo_str, 
                 data_str);

        printf("%s", linha);

        free(tempo_str);
        free(data_str);
    }
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
        case 7:
            recordeAdd();
            break;
        case 8:
            recordeEdit();
            break;
        case 9:
            recordeDelete();
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
            printf("Comando nÃ£o encontrado\n");
            break;
    }
}

void main()
{
    idrecordes = 0;

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
