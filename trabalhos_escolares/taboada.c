#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída

int main() { // Função principal do programa
    int opcao; // Declara uma variável inteira para armazenar a opção escolhida pelo usuário
    char continuar = 's'; // Declara uma variável char para controlar o loop e inicializa com 's'


while (continuar == 's' || continuar == 'S') // Início do loop while: executa enquanto o usuário quiser continuar
{
    printf("### Taboada ###\n\n"); // Imprime o título do programa
    printf("Qual equação deseja consultar?\n"); // Pergunta ao usuário qual equação ele deseja consultar
    printf("Opções:\n"); // Lista as opções disponíveis
    printf("1. Adição.\n2. Subtração.\n3. Multiplicação.\n4. Divisão.\n"); // Exibe as opções numeradas
    scanf("%d", &opcao); // Lê a opção escolhida pelo usuário e armazena em 'opcao'

    switch (opcao) { // Início do switch: executa o bloco de código correspondente à opção
        case 1: // Caso a opção seja 1 (Adição)
            for (int i = 1; i <= 10; i++) { // Loop externo: itera de 1 a 10
                for (int j = 1; j <= 10; j++) { // Loop interno: itera de 1 a 10
                    printf("Valor do cálculo: %d + %d = %d\n", i, j, i + j); // Imprime o resultado da adição
                }
                printf("\n\n"); // Imprime duas quebras de linha para separar as tabuadas
            }
            printf("\n\n"); // Imprime duas quebras de linha adicionais
            break; // Sai do switch
        case 2: // Caso a opção seja 2 (Subtração)
            for (int i = 1; i <= 10; i++) { // Loop externo: itera de 1 a 10
                for (int j = 1; j <= 10; j++) { // Loop interno: itera de 1 a 10
                    printf("Valor do cálculo: %d - %d = %d\n", i, j, i - j); // Imprime o resultado da subtração
                }
                printf("\n"); // Imprime uma quebra de linha
            }
            printf("\n\n"); // Imprime duas quebras de linha adicionais
            break; // Sai do switch
        case 3: // Caso a opção seja 3 (Multiplicação)
            for (int i = 1; i <= 10; i++) { // Loop externo: itera de 1 a 10
                for (int j = 1; j <= 10; j++) { // Loop interno: itera de 1 a 10
                    printf("Valor do cálculo: %d x %d = %d\n", i, j, i * j); // Imprime o resultado da multiplicação
                }
                printf("\n"); // Imprime uma quebra de linha
            }
            printf("\n\n"); // Imprime duas quebras de linha adicionais
            break; // Sai do switch
        case 4: // Caso a opção seja 4 (Divisão)
            for (int i = 1; i <= 10; i++) { // Loop externo: itera de 1 a 10
                for (int j = 1; j <= 10; j++) { // Loop interno: itera de 1 a 10
                    printf("Valor do cálculo: %d / %d = %.2f\n", i, j, (float)i / j); // Imprime o resultado da divisão com duas casas decimais
                }
                printf("\n"); // Imprime uma quebra de linha
            }
            printf("\n\n"); // Imprime duas quebras de linha adicionais
            break; // Sai do switch
        default: // Caso a opção seja inválida
            printf("Opção inválida...\n\n"); // Imprime mensagem de erro
            break; // Sai do switch
    }

printf("Deseja fazer um novo cauculo?, \n(s/n):"); // Pergunta ao usuário se ele deseja continuar
getchar();
scanf("%c", &continuar); // Lê a resposta do usuário e armazena em 'continuar'    

}printf("OK...\nFinalizando o programa...\n\n"); // Imprime na tela a mensagem finalizando o programa
    

        return 0; // Indica que o programa terminou com sucesso
}