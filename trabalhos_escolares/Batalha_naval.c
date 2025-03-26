/* Optei por continuar com o esquelto do codigo que comecei o desafio, e so adcionar as novas formas.
tentando colocalas o mais legiveis possivel, dentro do tabuleiro de 10x10 */

#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída para usar funções como printf

// Função para calcular o valor absoluto de um inteiro
int valorAbsoluto(int num) {
    if (num < 0) { // Se o número for negativo
        return -num; // Retorna o número multiplicado por -1 (torna positivo)
    }
    return num; // Se o número for positivo ou zero, retorna o número
}

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
    int linhaHorizontal = 9; // Define a linha onde o navio horizontal será posicionado
    int colunaHorizontal = 0; // Define a coluna inicial onde o navio horizontal será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        tabuleiro[linhaHorizontal][colunaHorizontal + i] = '3'; // Coloca o caractere '3' nas células correspondentes ao navio horizontal
    }

    // Posiciona um navio vertical com tamanho de 3 espaços
    int linhaVertical = 7; // Define a linha inicial onde o navio vertical será posicionado
    int colunaVertical = 9; // Define a coluna onde o navio vertical será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        tabuleiro[linhaVertical + i][colunaVertical] = '3'; // Coloca o caractere '3' nas células correspondentes ao navio vertical
    }

    // Posiciona o navio diagonal de 3 espaços usando loops aninhados
    int linhaDiag1 = 0; // Define a linha onde o navio diagonal será posicionado
    int colunaDiag1 = 7; // Define a coluna onde o navio diagonal será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        for (int j = 0; j < 1; ++j) { // Loop interno que roda apenas uma vez
            tabuleiro[linhaDiag1 + i][colunaDiag1 - i] = '3'; // faz com que a coluna retroceda a cada iteração do loop externo.
        }
    }

    // Posiciona o navio diagonal de 3 espaços começando na linha 5 usando loops aninhados
    int linhaDiag2 = 5; // Define a linha onde o navio diagonal será posicionado
    int colunaDiag2 = 0; // Define a coluna onde o navio diagonal será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para posicionar as 3 partes do navio
        for (int j = 0; j < 1; ++j) { // executar a lógica de posicionamento do elemento atual da diagonal.
            tabuleiro[linhaDiag2 + i][colunaDiag2 + i] = '3';  // faz com que a coluna retroceda a cada iteração do loop externo.
        }
    }

    // Desenha a cruz 
    int linhaCruz = 3;  // Define a linha onde a cruz será posicionado
    int colunaCruz = 2; // Define a coluna onde a cruz será posicionado
    int tamanhoCruz = 2; // Metade do tamanho de cada braço da cruz

    for (int i = -tamanhoCruz; i <= tamanhoCruz; ++i) { // Loop para posicionar as partes da cruz
        for (int j = -tamanhoCruz; j <= tamanhoCruz; ++j) { // Loop para posicionar as partes da cruz
            if (i == 0 || j == 0) { // Verifica se está na linha ou coluna central
                tabuleiro[linhaCruz + i][colunaCruz + j] = '3'; // Preenche a parte corespondente da cruz
            }
        }
    }

    // Desenha o losango
    int linhaLosango = 5; // Define a linha onde o losango será posicionado
    int colunaLosango = 4; // Define a coluna onde o losango será posicionado
    for (int i = 0; i < 5; ++i) { // Loop para percorrer as linhas do losango
        for (int j = 0; j < 3 - valorAbsoluto(2 - i); ++j) { // Loop para preencher as colunas do losango
            tabuleiro[linhaLosango + i][colunaLosango + 1 - j] = '3'; // Preenche a parte esquerda do losango
            tabuleiro[linhaLosango + i][colunaLosango + 1 + j] = '3'; // Preenche a parte direita do losango
        }
    }

    // Desenha o cone 
    int linhaCone = 2; // Define a linha onde o cone será posicionado
    int colunaCone = 7; // Define a coluna onde o cone será posicionado
    for (int i = 0; i < 3; ++i) { // Loop para percorrer as linhas do cone
        for (int j = -i; j <= i; ++j) { // Loop para preencher as colunas do cone
            tabuleiro[linhaCone + i][colunaCone + j] = '3'; // Preenche as células do cone
        }
    }

    // Imprime o tabuleiro
    imprimirTabuleiro(tabuleiro); // Chama a função para imprimir o tabuleiro com os navios posicionados

    return 0; // Indica que o programa terminou com sucesso
}