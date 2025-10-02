#include <stdio.h> // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <string.h> // Inclui a biblioteca para manipulacao de strings (strcspn).
#include <unistd.h> //biblioteca para manipulaçao do tempo(sleep), simula tempo de espera

// --- Constante de Configuracao ---
#define NUM_TERRITORIOS 5 // Define uma constante para o numero de territorios a serem cadastrados.

// --- Definicao da Struct ---
// 'typedef struct' cria um novo tipo de dado chamado 'Territorio'.
typedef struct {
    char nome[30]; // Array de caracteres para armazenar o nome do territorio (max 29 + '\0').
    char cor[10];  // Array de caracteres para armazenar a cor do exercito (max 9 + '\0').
    int tropas;    // Variavel inteira para armazenar a quantidade de tropas.
} Territorio;


// --- Funcao de Limpeza de Buffer ---
// Funcao auxiliar para garantir que nao hajam caracteres residuais no buffer de entrada (stdin).
void limpar_buffer() {
    int c;
    // Le e descarta caracteres um por um ate encontrar o caractere de nova linha ('\n') ou o Fim do Arquivo (EOF).
    while ((c = getchar()) != '\n' && c != EOF);
}


// --- Funcao Principal ---
int main() {
    // Declaracao de um vetor (array) que armazena 5 estruturas do tipo 'Territorio'.
    Territorio territorios[NUM_TERRITORIOS];
    int i; // Variavel de controle para os lacos 'for'.

    // Mensagens iniciais para orientar o usuario.
    printf("==========================================\n");
    printf("  SISTEMA DE CADASTRO DE TERRITORIOS \n");
    printf("==========================================\n");
    sleep(1.5);
    printf("Iniciando o cadastro de %d territorios.\n\n", NUM_TERRITORIOS);

    // --- Laco de Cadastro (Entrada de Dados) ---
    // O laco 'for' se repete 5 vezes (de i=0 ate i<5) para cadastrar todos os territorios.
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Cadastro do Territorio %d de %d ---\n", i + 1, NUM_TERRITORIOS);

        // 1. Leitura do NOME
        printf("Digite o nome do territorio (max 29 caracteres): ");
        // fgets le uma linha inteira do stdin (entrada padrao), evitando buffer overflow.
        if (fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin) == NULL) return 1;
        // strcspn busca a posicao do '\n' (nova linha) dentro da string.
        // Ao encontrar o '\n', ele e substituido por '\0' (caractere de terminacao de string),
        // garantindo que a string nao tenha uma quebra de linha indesejada.
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; 

        // 2. Leitura da COR
        printf("Digite a cor do exercito (ex: Vermelho, Azul - max 9 caracteres): ");
        // fgets e usado novamente para leitura segura de string.
        if (fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin) == NULL) return 1;
        // Remove o '\n' da string da cor.
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0'; 

        // 3. Leitura da QUANTIDADE DE TROPAS
        printf("Digite a quantidade de tropas: ");
        // scanf le um inteiro e o armazena no endereco de memoria de 'territorios[i].tropas' (&).
        if (scanf("%d", &territorios[i].tropas) != 1) {
             printf("Erro: Entrada invalida para tropas. Encerrando.\n");
             return 1;
        }

        // --- Limpeza de Buffer ---
        // Funcao chamada para limpar o '\n' que o scanf("%d") deixou no buffer,
        // garantindo que o proximo 'fgets' na proxima iteracao nao seja ignorado.
        limpar_buffer();

        printf("\n"); // Adiciona uma linha em branco para melhor separacao visual.
    }

    // --- Laco de Exibicao de Dados ---
    printf("==========================================\n");
    printf("  DADOS DOS TERRITORIOS CADASTRADOS \n");
    printf("==========================================\n");

    // O laco 'for' se repete 5 vezes para exibir os dados de cada territorio.
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        // printf exibe as informacoes formatadas na tela.
        printf("Territorio %d:\n", i + 1);
        // %s (string), %d (inteiro) sao especificadores de formato.
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exercito: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("---\n");
    }

    return 0; // Indica que o programa terminou com sucesso.
}