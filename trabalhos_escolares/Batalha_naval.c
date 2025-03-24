#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída para usar funções como printf

// Função para imprimir o tabuleiro
void imprimirTabuleiro(char tabuleiro[10][10]) {
    printf("  A B C D E F G H I J\n"); // Imprime o cabeçalho com as letras das colunas
    for (int i = 0; i < 10; ++i) { // Loop externo para percorrer as linhas do tabuleiro
        printf("%d ", i); // Imprime o número da linha
        for (int j = 0; j < 10; ++j) { // Loop interno para percorrer as colunas do tabuleiro
            printf("%c ", tabuleiro[i][j]); // Imprime o conteúdo de cada célula do tabuleiro seguido de um espaço
        }
        printf("\n"); // Imprime uma nova linha após cada linha do tabuleiro
    }
}

int main() {
    // Cria um tabuleiro 10x10 com espaços vazios ('0')
    char tabuleiro[10][10]; // Declara um array bidimensional de caracteres para representar o tabuleiro
    for (int i = 0; i < 10; ++i) { // Loop externo para percorrer as linhas do tabuleiro
        for (int j = 0; j < 10; ++j) { // Loop interno para percorrer as colunas do tabuleiro
            tabuleiro[i][j] = '0'; // Inicializa cada célula do tabuleiro com '0', representando um espaço vazio
        }
    }

    // Posiciona um navio horizontal com tamanho de 3 espaços
    int linhaHorizontal = 2; // Define a linha onde o navio horizontal será posicionado
    int colunaHorizontal = 3; // Define a coluna inicial onde o navio horizontal será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        tabuleiro[linhaHorizontal][colunaHorizontal + i] = '3'; // Coloca o caractere '3' nas células correspondentes ao navio horizontal
    }

    // Posiciona um navio vertical com tamanho de 3 espaços
    int linhaVertical = 5; // Define a linha inicial onde o navio vertical será posicionado
    int colunaVertical = 1; // Define a coluna onde o navio vertical será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        tabuleiro[linhaVertical + i][colunaVertical] = '3'; // Coloca o caractere '3' nas células correspondentes ao navio vertical
    }

    // Imprime o tabuleiro
    imprimirTabuleiro(tabuleiro); // Chama a função para imprimir o tabuleiro com os navios posicionados

        return 0; // Indica que o programa terminou com sucesso
}