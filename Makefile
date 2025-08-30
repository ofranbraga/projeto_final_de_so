# Makefile para o projeto "Simulador de Restaurante Caótico" em C (versão modular)

# Compilador
CC = gcc

# Diretórios
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Flags de compilação
# -I$(INCDIR) diz ao compilador para procurar arquivos de cabeçalho na pasta 'include'
# -pthread é essencial para linkar a biblioteca POSIX Threads
CFLAGS = -Wall -pthread -I$(INCDIR)

# Encontra todos os arquivos .c na pasta src
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Gera os nomes dos arquivos objeto (.o) a partir dos arquivos fonte
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Nome do executável final
TARGET = restaurante_caotico

# --- REGRAS ---

# Regra padrão: compila o projeto inteiro
all: $(TARGET)

# Regra para linkar os arquivos objeto e criar o executável final
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar cada arquivo .c em um arquivo .o
# $< é o nome do primeiro pré-requisito (o arquivo .c)
# $@ é o nome do alvo (o arquivo .o)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) # Cria o diretório de objetos se não existir
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra "run": compila (se necessário) e executa o programa
run: all
	./$(TARGET)

# Regra "clean": remove o executável e a pasta de objetos
clean:
	rm -f $(TARGET)
	rm -rf $(OBJDIR)