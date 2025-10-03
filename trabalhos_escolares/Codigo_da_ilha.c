#include <stdio.h>   // Inclui a biblioteca padrão de entrada e saída (para printf, scanf)
#include <string.h>  // Inclui a biblioteca para manipulação de strings (para strcpy, strcmp)
#include <stdlib.h>  // Inclui a biblioteca padrão (para EXIT_SUCCESS, entre outros, embora não tão usado aqui)
#include <unistd.h>  // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>    // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().


// Define o número máximo de itens que a mochila pode armazenar
#define MAX_ITENS 10

// ---------------------- Definição da Struct ----------------------
// Cria uma estrutura chamada 'Item' para armazenar os dados de cada objeto
typedef struct {
    char nome[30];       // Campo para armazenar o nome do item (até 29 caracteres + '\0')
    char tipo[20];       // Campo para armazenar o tipo do item (até 19 caracteres + '\0', ex: arma, munição, cura)
    int quantidade;      // Campo para armazenar a quantidade do item
} Item;

// ---------------------- Variáveis Globais ----------------------
Item mochila[MAX_ITENS]; // Declara um vetor de structs 'Item' para representar a mochila
int numItens = 0;        // Variável para controlar o número atual de itens na mochila

// ---------------------- Prototipos das Funções ----------------------
void limparBuffer();                           // Função para limpar o buffer de entrada
void inserirItem();                            // Função para adicionar um novo item à mochila
void removerItem();                            // Função para remover um item da mochila
void listarItens();                            // Função para listar todos os itens na mochila
void buscarItem();                             // Função para buscar um item específico pelo nome

// ---------------------- Função Principal (main) ----------------------
int main() {
    int opcao; // Variável para armazenar a opção escolhida pelo usuário

    // Loop principal do menu do sistema
    do {
        // Exibe o menu de opções para o jogador
        printf("==========================================\n");
        printf("MOCHILA DE SOBREVIVVENCIA - CODIGO DA ILHA\n");
        printf("==========================================\n");
        sleep(2); // Da uma pausa no programa pelo tempo estipulado

        // Interfasse de adção dos itens inicial 
        printf("\n--- Sistema de Inventario da Mochila ---\n");
        sleep(1); // Da uma pausa no programa pelo tempo estipulado
        printf("     Capacidade do inventario (0/10)\n");
        printf("1. Cadastrar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao); // Lê a opção do usuário

        // Limpa o buffer após a leitura de um inteiro para evitar problemas com leituras futuras de string
        limparBuffer();

        // Estrutura switch para executar a ação correspondente à opção escolhida
        switch (opcao) {
            case 1:
                inserirItem(); // Chama a função para cadastrar um item
                break;
            case 2:
                removerItem(); // Chama a função para remover um item
                break;
            case 3:
                listarItens(); // Chama a função para listar os itens
                break;
            case 4:
                buscarItem();  // Chama a função para buscar um item
                break;
            case 0:
                printf("Saindo do sistema. Ate mais!\n"); // Mensagem de saída
                break;
            default:
                printf("Opcao invalida. Por favor, tente novamente.\n"); // Mensagem para opção inválida
                break;
        }
        listarItens(); // Lista os itens após cada operação para manter o usuário atualizado
    } while (opcao != 0); // O loop continua até que o usuário escolha a opção '0' (Sair)

    return EXIT_SUCCESS; // Retorna 0 indicando que o programa terminou com sucesso
}

// ---------------------- Implementação das Funções ----------------------

        // Função para limpar o buffer de entrada
        void limparBuffer() {
            int c;
            // Loop para ler e descartar caracteres do buffer até encontrar um '\n' ou EOF
            while ((c = getchar()) != '\n' && c != EOF);
        }

    // Função para inserir um novo item na mochila
            void inserirItem() {
                // Verifica se a mochila está cheia (atingiu o limite de MAX_ITENS)
                if (numItens >= MAX_ITENS) {
                    printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
                    return; // Sai da função se a mochila estiver cheia
                }

                printf("\n--- Cadastrar Novo Item ---\n");

                // Solicita e lê o nome do item
                printf("Nome do Item: ");
                // Usa fgets para ler strings com espaços, garantindo segurança contra buffer overflow
                fgets(mochila[numItens].nome, sizeof(mochila[numItens].nome), stdin);
                // Remove o '\n' que fgets adiciona no final da string, se houver
                mochila[numItens].nome[strcspn(mochila[numItens].nome, "\n")] = 0;

                // Solicita e lê o tipo do item
                printf("Tipo do Item (ex: arma, municao, cura, ferramenta): ");
                fgets(mochila[numItens].tipo, sizeof(mochila[numItens].tipo), stdin);
                mochila[numItens].tipo[strcspn(mochila[numItens].tipo, "\n")] = 0;

                // Solicita e lê a quantidade do item
                printf("Quantidade: ");
                scanf("%d", &mochila[numItens].quantidade);
                limparBuffer(); // Limpa o buffer após a leitura de um inteiro

                numItens++; // Incrementa o contador de itens na mochila
                printf("Item '%s' cadastrado com sucesso!\n", mochila[numItens - 1].nome);
            }

            // Função para remover um item da mochila pelo nome
            void removerItem() {
                char nomeRemover[30]; // Variável para armazenar o nome do item a ser removido
                int i, j;             // Variáveis de loop
                int encontrado = 0;   // Flag para indicar se o item foi encontrado

                // Verifica se a mochila está vazia
                if (numItens == 0) {
                    printf("A mochila esta vazia. Nao ha itens para remover.\n");
                    return; // Sai da função se a mochila estiver vazia
                }

        printf("\n--- Remover Item da Mochila ---\n");
        printf("Digite o nome do item a ser removido: ");
        fgets(nomeRemover, sizeof(nomeRemover), stdin);
        nomeRemover[strcspn(nomeRemover, "\n")] = 0; // Remove o '\n'

        // Percorre o vetor de itens para encontrar o item a ser removido
        for (i = 0; i < numItens; i++) {
            // Compara o nome do item na mochila com o nome a ser removido (ignorando maiúsculas/minúsculas)
            if (strcmp(mochila[i].nome, nomeRemover) == 0) {
                encontrado = 1; // Define a flag como 1 (encontrado)

                // Desloca os itens subsequentes uma posição para trás para "remover" o item
                for (j = i; j < numItens - 1; j++) {
                    mochila[j] = mochila[j + 1];
                }
                numItens--; // Decrementa o contador de itens
                printf("Item '%s' removido com sucesso!\n", nomeRemover);
                break; // Sai do loop após remover o item
            }
        }

        // Se o item não foi encontrado após percorrer todo o vetor
        if (!encontrado) {
            printf("Item '%s' nao encontrado na mochila.\n", nomeRemover);
        }
    }

    // Função para listar todos os itens atualmente na mochila
    void listarItens() {
        int i; // Variável de loop

        printf("\n--- Itens Atuais na Mochila (%d/%d) ---\n", numItens, MAX_ITENS);

        // Verifica se a mochila está vazia
        if (numItens == 0) {
            printf("A mochila esta vazia.\n");
            return; // Sai da função se a mochila estiver vazia
        }

        // Cabeçalho da tabela de itens
        printf("----------------------------------------------------\n");
        printf("%-20s %-15s %-10s\n", "Nome", "Tipo", "Quantidade");
        printf("----------------------------------------------------\n");

        // Percorre e imprime os dados de cada item na mochila
        for (i = 0; i < numItens; i++) {
            printf("%-20s %-15s %-10d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
        printf("----------------------------------------------------\n");
    }

    // Função para buscar um item na mochila pelo nome (busca sequencial)
    void buscarItem() {
        char nomeBuscar[30]; // Variável para armazenar o nome do item a ser buscado
        int i;              // Variável de loop
        int encontrado = 0; // Flag para indicar se o item foi encontrado

        // Verifica se a mochila está vazia
        if (numItens == 0) {
            printf("A mochila esta vazia. Nao ha itens para buscar.\n");
            return; // Sai da função se a mochila estiver vazia
        }

        printf("\n--- Buscar Item na Mochila ---\n");
        printf("Digite o nome do item a ser buscado: ");
        fgets(nomeBuscar, sizeof(nomeBuscar), stdin);
        nomeBuscar[strcspn(nomeBuscar, "\n")] = 0; // Remove o '\n'

        // Percorre o vetor de itens para encontrar o item
        for (i = 0; i < numItens; i++) {
            // Compara o nome do item na mochila com o nome a ser buscado
            if (strcmp(mochila[i].nome, nomeBuscar) == 0) {
                encontrado = 1; // Define a flag como 1 (encontrado)
                printf("\nItem Encontrado:\n");
                printf("Nome: %s\n", mochila[i].nome);
                printf("Tipo: %s\n", mochila[i].tipo);
                printf("Quantidade: %d\n", mochila[i].quantidade);
                break; // Sai do loop assim que o item é encontrado
            }
        }

        // Se o item não foi encontrado após percorrer todo o vetor
        if (!encontrado) {
            printf("Item '%s' nao encontrado na mochila.\n", nomeBuscar);
        }


                return 0;

}