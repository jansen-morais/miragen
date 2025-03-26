#include <stdio.h>
// Constantes
#define LINHA 5
#define COLUNA 5
 
int main() {
    int matriz[LINHA][COLUNA];
    int target = 4;
    int found = 0;
    int soma = 1; // Vai atribuir aos indices os valores de acordo com a aplicação

    for (int i = 0; i < LINHA; i++) {     
        for (int j = 0; j < COLUNA; j++) { 
            matriz[i][j] = soma;
            soma++;
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    // Busca condicional do elemento alvo
    for (int i = 0; i < LINHA; i++) {      // Loop externo para as linhas
        for (int j = 0; j < COLUNA; j++) {  // Loop interno para as colunas
            if (matriz[i][j] == target) {
                printf("Valor %d encontrado na indice (%d, %d)\n", target, i, j);
                found = 1;
                break; // finaliza o loop interno
            }
        }
        if (found) break; // Finaliza o loop externo
    }
 
    if (!found) {
        printf("Elemento %d não encontrado na matriz\n", target);
    }

 
    return 0;
}