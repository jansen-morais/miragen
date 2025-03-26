#include <stdio.h>
#define LINHAS 5 // Aqui eu defino o tamanho final da matriz
#define COLUNAS 5 // Deixando assim o codigo mais limpo 
// E não precisa sempre digitar o valor
int main() {
    // Todas as matrizes precisam ser iguais 
    int matriz[LINHAS][COLUNAS];
    int soma = 0;
    // Inicialização da matriz usando estruturas de repetição for aninhadas
    // Somando as duas matrizes
    for (int i = 0; i < LINHAS; i++) // Este loop so rodara apoz que o segundo for fizer uma volta completa
    {
        for (int j = 0; j < COLUNAS; j++)  // Este loop lera a coluna, enquanto o antesseçoer lera a linha
        {
            soma++;
            matriz[i][j]=soma; // Atribuindo valores à matriz
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
    
 

 
        return 0;
}