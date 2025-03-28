#include <stdio.h>
 
int main() {
    int i = 1;
    int num;
    int opcao;
   
    /*do { // esta função vão rodar pelomenos uma vez sendo verdadeir ou falso a condição 
        printf("%d\n", i);
        i++; // lembrar de sempre incrementar para não gerar o loope
    } while (i <= 5);
   

   /* do {
        printf("Digite um número (negativo para sair): ");
        scanf("%d", &num);
       
        if (num >= 0) {
            printf("Você digitou: %d\n", num);
        }
    } while (num >= 0);
   
    printf("Número negativo detectado. Saindo...\n");*/

    do {
        printf("Menu:\n");
        printf("1. Opção 1\n");
        printf("2. Opção 2\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: \n");
        scanf("%d", &opcao);
       
        switch(opcao) {
            case 1:
                printf("Você escolheu a opção 1\n");
                break;
            case 2:
                printf("Você escolheu a opção 2\n");
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);
 
    
    return 0;
}