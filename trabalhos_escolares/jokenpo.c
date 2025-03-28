#include <stdio.h> // Biblioteca padrão de entrada e saída (printf, scanf)
#include <stdlib.h> // Biblioteca padrão de funções gerais (rand, srand)
#include <time.h> // Biblioteca para funções de tempo (time)
// funçoes que capacitam a leitura da string como rsposta 
#include <stdarg.h>//paa lidar com a manipulação de argumentos variaveis
#include <string.h> // Adicionado: Inclui a biblioteca string.h para usar strcmp()

int main(){
    int escolhajogador, escolhacomputador; // Declara variáveis para escolha do jogador e do computador
    // escolhajogador armazena a escolha do jogador (1, 2 ou 3)
    // escolhacomputador armazena a escolha do computador (gerada aleatoriamente)
    char repetir[4];
    srand(time(0)); // Inicializa a semente do gerador de números aleatórios com o tempo atual
    // A função srand() configura o gerador de números aleatórios com uma semente inicial.
    // time(0) retorna o tempo atual em segundos, garantindo que a semente seja diferente a cada execução.

    
    do
    {
        printf("*** jogo de jokenpô ***\n\n"); // Imprime o título do jogo
        // printf() é uma função que envia uma string formatada para a saída padrão (tela).
        printf("Escolha a opção\n"); // Pede ao jogador para escolher uma opção
        printf("1. pedra\n");
        printf("2. papel\n");
        printf("3. tesoura\n");
        printf("escolha: ");
        scanf("%d", &escolhajogador); // Lê a escolha do jogador
        // A função scanf() lê um número inteiro do teclado e armazena em escolhajogador.
        // %d especifica que um número inteiro deve ser lido.
        // &escolhajogador fornece o endereço de memória onde o valor lido deve ser armazenado.
    
        escolhacomputador = rand()%3+1; // Gera um número aleatório entre 1 e 3 para a escolha do computador
        // A função rand() gera um número aleatório.
        // O operador % (módulo) retorna o resto da divisão por 3, resultando em 0, 1 ou 2.
        // Adicionando 1, obtemos um número aleatório entre 1 e 3.
    
        switch (escolhajogador) // Inicia um switch para verificar a escolha do jogador
        {
        case 1: // Caso escolhajogador seja 1 (pedra)
            printf("O jogador escolheu pedra\n");
            break; // Sai do switch após executar este caso
            // O comando break é essencial para evitar que a execução continue para os próximos casos.
        case 2: // Caso escolhajogador seja 2 (papel)
            printf("O jogador escolheu papel\n");
            break; // Sai do switch após executar este caso
        case 3: // Caso escolhajogador seja 3 (tesoura)
            printf("O jogador escolheu tesoura\n");
            break; // Sai do switch após executar este caso
    
        default: // Caso escolhajogador não corresponda a nenhum dos casos anteriores
        printf("Opção invalida\n"); // Informa que a opção é inválida
            break; // Sai do switch após executar este caso
        }
        // O switch compara escolhajogador com cada caso.
        // Se houver correspondência, o código dentro do caso é executado.
        // O break sai do switch após a execução do caso.
        // O default é executado se nenhum caso corresponder.
    
        switch (escolhacomputador) // Inicia um switch para verificar a escolha do computador
        {
        case 1: // Caso escolhacomputador seja 1 (pedra)
            printf("O computador escolheu pedra\n");
            break; // Sai do switch após executar este caso
        case 2: // Caso escolhacomputador seja 2 (papel)
            printf("O computador escolheu papel\n");
            break; // Sai do switch após executar este caso
        case 3: // Caso escolhacomputador seja 3 (tesoura)
            printf("O computador escolheu tesoura\n");
            break; // Sai do switch após executar este caso
        }
        // Similar ao switch anterior, mas para a escolha do computador.
    
        if (escolhacomputador == escolhajogador) // Verifica se houve empate
        {
            printf("### O jogo empatou ###\n");
        }else if ((escolhajogador == 1)&&(escolhacomputador == 3) // Verifica se o jogador ganhou (pedra vs. tesoura)
                ||(escolhajogador == 2)&&(escolhacomputador == 1) // Verifica se o jogador ganhou (papel vs. pedra)
                ||(escolhajogador == 3)&&(escolhacomputador == 2)) // Verifica se o jogador ganhou (tesoura vs. papel)
        {
            printf("### Parabens voçê ganhou! ####\n"); // caso o jogador ganhe
        }else{
            printf("### Que pena, voçê perdeu! ###\n"); // Caso o jogador perca

            printf("Deseja jogar novamente?...\n");
            getchar();
            scanf("%3s",&repetir);
            for (int i = 0; repetir[i]; i++){
                repetir[i] = tolower(repetir[i]);
            }
        }
        
    } while (strcmp(repetir, "sim")==0);

    printf("\n Ok... ate a porxima...\n");

    
    // Uma série de if e else if verifica as combinações de escolhas para determinar o vencedor ou se houve empate.
    // && representa o operador lógico "e".
    // || representa o operador lógico "ou".

    return 0; // Indica que o programa terminou com sucesso
    // A função main() retorna 0 para indicar que o programa foi executado sem erros.
}