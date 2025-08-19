#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <ctype.h>

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
    char genero[32];
};

struct Recorde
{
    char usuario[24];
    char jogo[24];
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

void serializarString(char* destino, char* origem)
{
    int i;
    for (i = 0; origem[i] != '\0'; i++)
        destino[i] = (origem[i] == ' ') ? '_' : origem[i];
    destino[i] = '\0';
}

void deserializarString(char* destino, char* origem)
{
    int i;
    for (i = 0; origem[i] != '\0'; i++)
        destino[i] = (origem[i] == '_') ? ' ' : origem[i];
    destino[i] = '\0';
}

int gerarId(int min, int max) {
    return min + rand() % (max - min + 1);
}

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

int validarData(char *str) {
    if (!str || strlen(str) != 10)
        return 0;

    if (str[2] != '-' || str[5] != '-')
        return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(str[i])) return 0;
    }

    int dia = atoi(str);
    int mes = atoi(str + 3);
    int ano = atoi(str + 6);

    if (mes < 1 || mes > 12) return 0;

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        diasNoMes[1] = 29; // ano bissexto

    if (dia < 1 || dia > diasNoMes[mes - 1]) return 0;

    return 1; 
}

int validarTempo(int horas, int minutos, int segundos, int milissegundos) {
    if (horas < 0) return 0;                     
    if (minutos < 0 || minutos > 59) return 0;   
    if (segundos < 0 || segundos > 59) return 0; 
    if (milissegundos < 0 || milissegundos > 999) return 0; 
    return 1;
}

void exportarResultado(struct Recordes *consulta)
{
    FILE* out = fopen("armazenamento/tabela_recordes.txt", "w");
    if (!out) {
        printf("\nFalha ao salvar o arquivo\n");
        return;
    }

    fprintf(out, "--------------------------\n");
    // Salva os dados da struct no arquivo
    for (int i = 0; i < consulta->tamanho; i++)
    {
        fprintf(out, "Usuário: %s\n", consulta->lista[i].usuario);
        fprintf(out, "Jogo: %s\n", consulta->lista[i].jogo);
        fprintf(out, "Plataforma: %s\n", consulta->lista[i].plataforma);
        fprintf(out, "Tempo: %s\n", formatarTempo(consulta->lista[i].tempo));
        fprintf(out, "Data de Registro: %s\n", formatarData(consulta->lista[i].data_registro));
        fprintf(out, "Identificador: %d\n", consulta->lista[i].identificacao);
        fprintf(out, "--------------------------\n");
    }

    fclose(out);
    printf("\nArquivo tabela_recordes.txt salvo com a consulta!\n");
}

void desserializarAlteracoes()
{
    FILE *fileUsuarios = fopen("armazenamento/usuarios.txt", "r");
    if (!fileUsuarios) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

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

    FILE *fileJogos = fopen("armazenamento/jogos.txt", "r");
    if (!fileJogos) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

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

    FILE *fileRecordes = fopen("armazenamento/recordes.txt", "r");
    if (!fileRecordes) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

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
}


// Confere se o usuário já existe no banco de dados
int usuarioQuery(char *identificacao)
{
    for (int i = 0; i < usuarios.tamanho; i++)
        if (strcmp(usuarios.lista[i].apelido, identificacao) == 0) return i;
    
    return -1;
}

// Confere se o jogo já existe no banco de dados
int jogoQuery(char *identificacao) 
{
    for (int i = 0; i < jogos.tamanho; i++) 
        if (strcmp(jogos.lista[i].nome, identificacao) == 0) return i;

    return -1;
}

// Confere se o recorde já existe no banco de dados
struct Resultados recordeQuery(char* nomeJogador, char* nomeJogo, char* identificacao_recorde)
{
    struct Resultados resultados;
    resultados.tamanho = 0;
    resultados.lista = (int *) malloc(resultados.tamanho * sizeof(int));
    for (int i = 0; i <= recordes.tamanho - 1; i++){
        int achouJogador = (strcmp(nomeJogador, "*") == 0) || (strcmp(nomeJogador, recordes.lista[i].usuario) == 0);
        int achouJogo = (strcmp(nomeJogo, "*") == 0) || (strcmp(nomeJogo, recordes.lista[i].jogo) == 0);
        int achouId = (strcmp(identificacao_recorde, "*") == 0) || (recordes.lista[i].identificacao == atoi(identificacao_recorde));

        if (achouId && achouJogador && achouJogo){
            resultados.lista = (int*) realloc(resultados.lista, ++resultados.tamanho * sizeof(int));
            resultados.lista[resultados.tamanho-1] = i;
        }
    }

    return resultados;
}

void usuarioAdd()
{
    struct Usuario temp;
    int nomeExistente = 0;

    printf("\nDigite as informações do usuário:\n");

    do
    {
        printf("\nApelido\n");
        printf("\n> ");

        fgets(temp.apelido, sizeof(temp.apelido), stdin);
        temp.apelido[strcspn(temp.apelido, "\n")] = '\0';
    
        if ((nomeExistente = usuarioQuery(temp.apelido)) >= 0)
            printf("\nNome já existe no banco de dados\n");
        else
            break;
    } 
    while (1);

    printf("\nEmail\n");
    printf("\n> ");
    fgets(temp.email, sizeof(temp.email), stdin);
    temp.email[strcspn(temp.email, "\n")] = '\0';

    do 
    {
        printf("\nNascimento (dd-mm-aaaa)\n");
        printf("\n> ");
        fgets(temp.nascimento, sizeof(temp.nascimento), stdin);
        temp.nascimento[strcspn(temp.nascimento, "\n")] = '\0';
        
        if (validarData(temp.nascimento) != 1) 
        printf("\nData em formato inválido, digite no formato (dd-mm-aaaa)\n");
        else
            break;
    } 
    while (1);

    printf("\nPa��s\n");
    printf("\n> ");
    getchar();
    fgets(temp.pais, sizeof(temp.pais), stdin);
    temp.pais[strcspn(temp.pais, "\n")] = '\0';

    usuarios.tamanho++;
    usuarios.lista = (struct Usuario *) realloc(usuarios.lista, usuarios.tamanho * sizeof(struct Usuario));
    if (usuarios.lista != NULL) {
        usuarios.lista[usuarios.tamanho - 1] = temp;
    } else {
        printf("\nErro ao alocar memória para novo usuário.\n");
        usuarios.tamanho--;
    }
}

void jogoAdd()
{
    struct Jogo temp;

    printf("\nDigite as informações do jogo:\n");

    do 
    {
        printf("\nNome\n");
        printf("\n> ");
        fgets(temp.nome, sizeof(temp.nome), stdin);
        temp.nome[strcspn(temp.nome, "\n")] = '\0';

        if ((jogoQuery(temp.nome)) >= 0) 
            printf("\nO jogo já existe no banco de dados\n");
        else
            break;
    } 
    while (1);

    printf("\nDesenvolvedores\n");
    printf("\n> ");
    fgets(temp.desenvolvedora, sizeof(temp.desenvolvedora), stdin);
    temp.desenvolvedora[strcspn(temp.desenvolvedora, "\n")] = '\0';

    do 
    {
        printf("\nLançamento (dd-mm-aaaa)\n");
        printf("\n> ");
        fgets(temp.data_lancamento, sizeof(temp.data_lancamento), stdin);
        temp.data_lancamento[strcspn(temp.data_lancamento, "\n")] = '\0';
        
        if (validarData(temp.data_lancamento) != 1) 
            printf("\nData em formato inválido, digite no formato (dd-mm-aaaa)\n");
        else
            break;
    } 
    while (1);

    printf("\nGênero\n");
    printf("\n> ");
    fgets(temp.genero, sizeof(temp.genero), stdin);
    temp.genero[strcspn(temp.genero, "\n")] = '\0';

    jogos.lista = (struct Jogo *) realloc(jogos.lista, ++jogos.tamanho * sizeof(struct Jogo));

    if (jogos.lista != NULL) jogos.lista[jogos.tamanho - 1] = temp;
    else
    {
        printf("\nErro ao alocar memória para novo jogo");
        jogos.tamanho--;
    }
}

void recordeAdd()
{
    struct Recorde temp;
    char player[24], jogo[24];
    int horas, minutos, segundos, milisecundos;

    printf("\nDigite as informações do recorde:\n");

    printf("\nNome do usuário: \n");
    printf("\n> ");
    fgets(player, sizeof(player), stdin);
    player[strcspn(player, "\n")] = '\0';

    if (usuarioQuery(player) >= 0) strcpy(temp.usuario, usuarios.lista[usuarioQuery(player)].apelido);
    else
    {
        printf("\nUsuário não encontrado!\n");
        return;
    }
    
    printf("\nNome do jogo:\n");
    printf("\n> ");
    fgets(jogo, sizeof(jogo), stdin);
    jogo[strcspn(jogo, "\n")] = '\0';

    if (jogoQuery(jogo) >= 0) strcpy(temp.jogo,jogos.lista[jogoQuery(jogo)].nome);
    else 
    {
        printf("\nJogo não encontrado!\n");
        return;
    }

    printf("\nPlataforma do Recorde:\n");
    printf("\n> ");
    fgets(temp.plataforma, sizeof(temp.plataforma), stdin);
    temp.plataforma[strcspn(temp.plataforma, "\n")] = '\0';

    do 
    {
        printf("\nTempo da Run (formato hh:mm:ss:msms):\n");
        printf("\n> ");
        scanf("%d:%d:%d:%d", &horas, &minutos, &segundos, &milisecundos);
        while(getchar() != '\n');
        
        if (validarTempo(horas, minutos, segundos, milisecundos) != 1) 
        printf("\nTempo em formato inválido, digite no formato (hh:mm:ss:msms)\n");
    } 
    while (validarTempo(horas, minutos, segundos, milisecundos) != 1);

    temp.tempo = horas * 3600000ULL + minutos * 60000ULL + segundos * 1000ULL + milisecundos;
    temp.data_registro = time(NULL);

    temp.identificacao = gerarId(1000, 10000); 

    recordes.lista = (struct Recorde *) realloc(recordes.lista, ++recordes.tamanho * sizeof(struct Recorde));
    if (recordes.lista != NULL) recordes.lista[recordes.tamanho - 1] = temp;
    else
    {
        printf("\nErro ao alocar memória para o recorde\n");
        recordes.tamanho--;
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
                while(getchar() != '\n');
                switch (escolha)
                {
                    case 1:
                        printf("\nDigite o nome:\n");
                        printf("\n> ");
                        char temp[24];
                        fgets(temp, sizeof(temp), stdin);
                        temp[strcspn(temp, "\n")] = '\0';

                        if (usuarioQuery(temp) >= 0) printf("\nNome já existe\n");
                        else
                        {
                            strcpy(usuarios.lista[posicao].apelido, temp);
                            printf("\nNome editado com sucesso\n");
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
                        } 
                        while (1);
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
    else printf("\nNome não encontrado\n");
}

void jogoEdit()
{
    int posicao;

    printf("\nDigite o nome do jogo:\n");
    
    char temp[23];
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
            while(getchar() != '\n'); 
            switch (escolha) 
            {
                case 1:
                    printf("\nDigite o do jogo: \n");
                    printf("\n> ");
                    char temp[24];
                    fgets(temp, sizeof(temp), stdin);
                    temp[strcspn(temp, "\n")] = '\0';
                    if (jogoQuery(temp) >= 0) printf("\nJogo com nome já registrado!\n");
                    else { 
                        strcpy(jogos.lista[posicao].nome, temp);
                        printf("\nNome registrado com sucesso!\n");
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
                    } 
                    while (1);
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
    } else printf("\nJogo não encontrado!\n");
}

void recordeEdit()
{
    printf("\nDigite o id do recorde:\n");
    char id[24];
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
            while(getchar() != '\n');
            char temp[32];

            switch (escolha) 
            {
                case 1:
                    printf("\nDigite o usuario desejado: \n");
                    printf("\n> ");
                    fgets(temp, sizeof(temp), stdin);
                    temp[strcspn(temp, "\n")] = '\0';
                    if (usuarioQuery(temp) < 0) printf("\nUsuário não encontrado\n");
                    else { 
                        strcpy(recordes.lista[posicao].usuario, usuarios.lista[usuarioQuery(temp)].apelido);
                        printf("\nUsuario editado com sucesso!\n");
                    }
                    break;
                case 2:
                    printf("\nDigite o nome do jogo: \n");
                    printf("\n> ");
                    fgets(temp, sizeof(temp), stdin);
                    temp[strcspn(temp, "\n")] = '\0';
                    if (jogoQuery(temp) < 0) printf("\nJogo não encontrado!\n");
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
                        while(getchar() != '\n');

                        if (validarTempo(horas, minutos, segundos, milisecundos) != 1) 
                            printf("\nTempo em formato inválido, digite no formato (hh:mm:ss:msms)\n");
                        else
                        {
                            recordes.lista[posicao].tempo = horas * 3600000ULL + minutos * 60000ULL + segundos * 1000ULL + milisecundos; 
                            printf("\nTempo editado com sucesso!\n");
                            break;
                        }
                    } 
                    while (1);
                    break;
                case 999:
                    break;
                default: 
                    printf("\nComando não encontrado\n");
                    break;
            }
        }
    } else printf("\nRecorde não encontrado!\n");
}

void usuarioDelete()
{
    int posicao;

    printf("\nDigite o apelido do usuario:\n");
    char apelido[24];
    printf("\n> ");
    fgets(apelido, sizeof(apelido), stdin);
    apelido[strcspn(apelido, "\n")] = '\0';
    
    if ((posicao = usuarioQuery(apelido)) >= 0)
    {
        struct Resultados resultados = recordeQuery(apelido, "*", "*");
        if(resultados.tamanho == 0)
        {
            usuarios.lista[posicao] = usuarios.lista[usuarios.tamanho - 1]; 
            usuarios.lista = (struct Usuario*) realloc(usuarios.lista, --usuarios.tamanho * sizeof(struct Usuario));
            printf("\nUsuário deletado com sucesso!\n");
        }
        else
            printf("\nNão foi possível deletar este usuário, pois há recordes registrados com ele no banco de dados, delete-os primeiro");
    }
    else printf("\nApelido não encontrado\n");
}

void jogoDelete() 
{
    int posicao;

    printf("\nDigite o nome do jogo: \n");
    char nome[24];
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if((posicao = jogoQuery(nome)))
    {
        struct Resultados resultados = recordeQuery("*", nome, "*");
        if(resultados.tamanho == 0)
        {
        jogos.lista[posicao] = jogos.lista[jogos.tamanho - 1];
        jogos.lista = (struct Jogo*) realloc (jogos.lista, --jogos.tamanho * sizeof(struct Jogo));
        resultados = recordeQuery("*", jogos.lista[posicao].nome, "*");
        printf("\nJogo deletado com sucesso!\n");
        }
        else
            printf("\nNão foi possível deletar este jogo, pois há recordes registrados com ele no banco de dados, delete-os primeiro");

    }
    else printf("\nJogo nao encontrado!\n");
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
        recordes.lista = (struct Recorde *) realloc(recordes.lista, --recordes.tamanho * sizeof(struct Recorde));
        printf("\nRecorde deletado com sucesso!\n");
    }
    else printf("\nRecorde não encontrado!\n");
}

void mostrarUsuarios()
{
    if (usuarios.tamanho != 0)
    {
        printf("\nUsuarios\n");
        for (int i = 0; i < usuarios.tamanho; i++){
            printf("\n---\n");
            printf("\nApelido: %s\n", usuarios.lista[i].apelido);
            printf("\nEmail: %s\n", usuarios.lista[i].email);
            printf("\nNascimento: %s\n", usuarios.lista[i].nascimento);
            printf("\nPais: %s\n", usuarios.lista[i].pais);
            printf("\n---\n");
        }
    }
    else
        printf("\nNão há usuários adicionados\n");
}

void mostrarJogos()
{
     if (jogos.tamanho != 0)
    {
        printf("\nJogos\n");
        for (int i = 0; i < jogos.tamanho; i++){
            printf("\n---\n");
            printf("\nNome: %s\n", jogos.lista[i].nome);
            printf("\nDesenvolvedora: %s\n", jogos.lista[i].desenvolvedora);
            printf("\nData_lancamento: %s\n", jogos.lista[i].data_lancamento);
            printf("\nGênero: %s\n", jogos.lista[i].genero);
            printf("\n---\n");
        }
    }
    else
        printf("\nNão há jogos adicionados\n");
}

void consulta()
{
    char nomeJogador[24];
    char nomeJogo[24];
    char identificacao_recorde[32];
    
    struct Recordes consulta;
    consulta.tamanho = 0;
    consulta.lista = (struct Recorde*) malloc(consulta.tamanho * sizeof(struct Recorde));

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

    struct Resultados resultados = recordeQuery(nomeJogador, nomeJogo, identificacao_recorde);

    printf("Resultados encontrados: %d\n", resultados.tamanho);
    
    if (resultados.tamanho == 0)
    {
        printf("\nNenhum recorde encontrado!\n");
        return;
    }
 
    for (int i = 0; i < resultados.tamanho; i++)
    {
        consulta.lista = (struct Recorde *) realloc(consulta.lista, ++consulta.tamanho * sizeof(struct Recorde));
        consulta.lista[consulta.tamanho - 1] = recordes.lista[resultados.lista[i]];
    }

    // "Recordes:\n" 11
    // "1. [identificador] [nomeJogador] [pais] [jogo] [tempo] [data_registro]\n" 11 + 24 + 12 + 24 + 12 + 10 = 93 * consulta.tamanho 

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

    char escolha;

    printf("\nGostaria de exportar os resultados da sua consulta? (s/n)\n");
    printf("> ");
    scanf("%c", &escolha);

    if (escolha == 's')
        exportarResultado(&consulta);
    else if (escolha != 'n')
        printf("Escolha inválida");
}

void interpretador(int prompt)
{
    char ajuda[] = "[0] = Ajuda\n[1] = Adicionar Jogador\n[2] = Editar Jogador\n[3] = Deletar Jogador\n[4] = Adicionar Jogo\n[5] = Editar Jogo\n[6] = Deletar Jogo\n[7] = Adicionar Recorde\n[8] = Editar Recorde\n[9] = Remover Recorde\n[10] = Consultar\n[999] = Finalizar Programa\n";

    switch (prompt){
        case 0:
            printf("\n%s", ajuda);
            break;
        case 1:
            usuarioAdd();
            break;
        case 2:
            usuarioEdit();
            break;
        case 3:
            usuarioDelete();
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
            mostrarUsuarios();
            break;
        case 11:
            mostrarJogos();
            break;
        case 12:
            consulta();
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

int main()
{
    // Inicializa o gerador com a hora atual
    srand(time(NULL)); 

    usuarios.tamanho = 0;
    usuarios.lista = (struct Usuario*)malloc(usuarios.tamanho * sizeof(struct Usuario));

    jogos.tamanho = 0;
    jogos.lista = (struct Jogo*)malloc(jogos.tamanho * sizeof(struct Jogo));

    recordes.tamanho = 0;
    recordes.lista = (struct Recorde*)malloc(recordes.tamanho * sizeof(struct Recorde));

    desserializarAlteracoes();
        
    int prompt = 0;

    printf("\nBem vindo ao Speed Runners!\n");

    while (prompt != 999)
    {
        printf("\nDigite o comando, ou 0 para ajuda\n");
        printf("\n> ");
        scanf("%d", &prompt);
        while(getchar() != '\n');

        interpretador(prompt);
    }

    return 0;
}
