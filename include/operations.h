#ifndef OPERATIONS_H
#define OPERATIONS_H

int usuarioAdd(char* apelido, char* email, char* nascimento, char* pais);
int jogoAdd(char* nome, char* desenvolvedora, char* data_lancamento, char* genero);
int recordeAdd(char* usuario, char* jogo, char* plataforma, char* tempo);
void usuarioEdit();
void jogoEdit();
void recordeEdit();
void usuarioDelete();
void jogoDelete();
void recordeDelete();
void mostrarUsuarios();
void mostrarJogos();
struct Recordes consultaRecordes(char* nomeJogador, char* nomeJogo, char* identificacao_recorde);
void mostrarRecordes(struct Recordes consulta);
void interpretador(int prompt);

#endif
