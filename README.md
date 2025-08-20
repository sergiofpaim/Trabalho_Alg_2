### Trabalho Algoritmos e Programação 2 - Speedrunners

#### Resumo
Programa em C para gerenciar usuários, jogos e recordes (runs) de speedrunning.  
O sistema permite adicionar, editar, remover e consultar dados, além de exportar consultas para um arquivo texto.

#### Compilação (Windows com MinGW)
Abra um terminal (cmd ou PowerShell) no diretório do projeto e execute:

```bash
cd src/
gcc *.c -o ../speedrunners.exe
cd ..
```

ou, se preferir produzir sem extensão no Linux/MinGW:

```bash
cd src/
gcc *.c -o ../speedrunners
cd ..
```

#### Execução
No mesmo diretório que o executável:

Windows:
```bash
./speedrunners.exe
```

Linux/Mac:
```bash
./speedrunners
```

#### Arquivos de armazenamento
O programa utiliza a pasta "armazenamento" com os arquivos:
- armazenamento/usuarios.txt
- armazenamento/jogos.txt
- armazenamento/recordes.txt

Ao encerrar o programa com a opção 999 os dados são serializados para esses arquivos.  
O comando de exportação de consulta cria:
- armazenamento/tabela_recordes.txt

#### Formato das entradas
- Datas: dd-mm-aaaa (ex: 05-08-2025)  
- Tempos: hh:mm:ss:msms (ex: 00:12:34:256)  
- Para consultas é possível usar '*' como coringa (todos)

#### Mapa rápido de comandos (menu)
- 0  - Ajuda
- 1  - Adicionar Jogador
- 2  - Editar Jogador
- 3  - Deletar Jogador
- 4  - Adicionar Jogo
- 5  - Editar Jogo
- 6  - Deletar Jogo
- 7  - Adicionar Recorde
- 8  - Editar Recorde
- 9  - Remover Recorde
- 10 - Mostrar Usuarios
- 11 - Mostrar Jogos
- 12 - Consultar
- 999- Finalizar programa (salva alterações)