# Compilador e flags
CC = gcc
CFLAGS = -Wall

# Detecta SO
ifeq ($(OS),Windows_NT)
    EXE := .exe
    RM := del /Q
    NULL := 2>nul || true
else
    EXE :=
    RM := rm -f
    NULL :=
endif

# Diretórios e fontes
SRC_DIR = src
SOURCES := $(wildcard $(SRC_DIR)/*.c)
TARGET = speedrunners$(EXE)

# Alvo padrão
all: $(TARGET)

# Compilação direta (sem .o)
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

# Limpeza
clean:
	$(RM) $(TARGET) $(NULL)

# Rebuild
rebuild: clean all

# (Opcional) Executar
run: $(TARGET)
	./$(TARGET)

