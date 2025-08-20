#ifndef OPERATIONS_H
#define OPERATIONS_H

void usuarioAdd();
void jogoAdd();
void recordeAdd();
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
