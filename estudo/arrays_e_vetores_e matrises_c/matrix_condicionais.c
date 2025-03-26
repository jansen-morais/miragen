#include <stdio.h>
 
int main() {
    // Todas as matrizes precisam ser iguais 
    int matriz1[2][2] = {{1, 2}, {3, 4}};
    int matriz2[2][2] = {{5, 6}, {7, 8}};
    int matrizSoma[2][2];

    // Inicialização da matriz usando estruturas de repetição for aninhadas
    // Somando as duas matrizes
    for (int i = 0; i < 2; i++) { // Este loop so rodara apoz que o segundo for fizer uma volta completa
        for (int j = 0; j < 2; j++) { // Este loop lera a coluna, enquanto o antesseçoer lera a linha 
            matrizSoma[i][j] = matriz1[i][j] + matriz2[i][j]; // Atribuindo valores à matriz
        }
    }
 
    // Exibindo a matriz resultante
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("matrizSoma[%d][%d] = %d\n", i, j, matrizSoma[i][j]);
        }
    }
 
    return 0;
}