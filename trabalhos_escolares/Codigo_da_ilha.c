#include <stdio.h>    // Inclui a biblioteca padrão de entrada e saída (para printf, scanf)
#include <string.h>   // Inclui a biblioteca para manipulação de strings (para strcpy, strcmp, strcspn)
#include <stdlib.h>   // Inclui a biblioteca padrão (para EXIT_SUCCESS, malloc, free, system)
#include <unistd.h>   // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
                      // (OBS: 'sleep' é padrão em sistemas Unix-like (Linux/macOS). Em Windows, use 'Sleep' de <windows.h>.)
#include <time.h>     // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand() e medir tempo.

// Define o número máximo de itens que a mochila pode armazenar no vetor
#define MAX_ITENS 10

// ---------------------- 1. Criação de Structs ----------------------

// Struct 'Item': representa um objeto individual no inventário do jogador
typedef struct {
    char nome[30];       // Campo para armazenar o nome do item (até 29 caracteres + '\0' para terminador nulo)
    char tipo[20];       // Campo para armazenar o tipo do item (até 19 caracteres + '\0', ex: arma, munição, cura)
    int quantidade;      // Campo para armazenar a quantidade do item
} Item;

// Struct 'No': representa um nó na lista encadeada
// Contém os dados do item e um ponteiro para o próximo nó
typedef struct No {
    Item dados;          // Armazena uma instância da struct Item (nome, tipo, quantidade)
    struct No* proximo;  // Ponteiro para o próximo nó na lista. 'struct No*' é necessário porque 'No' ainda não está totalmente definido.
} No;

// ---------------------- Variáveis Globais para Mochila (Vetor) ----------------------
// mochilaVetor: um array (vetor) de structs 'Item' para armazenar os itens na versão de vetor da mochila
Item mochilaVetor[MAX_ITENS];
// numItensVetor: contador de quantos itens estão atualmente no 'mochilaVetor'
int numItensVetor = 0;
// comparacoesBuscaSequencialVetor: contador para registrar o número de comparações em buscas sequenciais no vetor
long long comparacoesBuscaSequencialVetor = 0;
// comparacoesBuscaBinariaVetor: contador para registrar o número de comparações em buscas binárias no vetor
long long comparacoesBuscaBinariaVetor = 0;

// ---------------------- Variáveis Globais para Mochila (Lista Encadeada) ----------------------
// headLista: ponteiro para o primeiro nó da lista encadeada. Se for NULL, a lista está vazia.
No* headLista = NULL;
// numItensLista: contador de quantos itens estão atualmente na lista encadeada
int numItensLista = 0;
// comparacoesBuscaSequencialLista: contador para registrar o número de comparações em buscas sequenciais na lista
long long comparacoesBuscaSequencialLista = 0;

// ---------------------- Protótipos das Funções Comuns ----------------------
// Declaração da função para limpar o buffer de entrada (stdin)
void limparBuffer();
// Declaração da função que exibe o menu principal do programa
void exibirMenuPrincipal();
// Declaração da função que exibe o menu de operações para a mochila (vetor ou lista)
void exibirMenuMochila();

// ---------------------- Protótipos das Funções para Mochila (Vetor) ----------------------
// Declaração da função para inserir um item no vetor
void inserirItemVetor();
// Declaração da função para remover um item do vetor
void removerItemVetor();
// Declaração da função para listar todos os itens no vetor
void listarItensVetor();
// Declaração da função para ordenar os itens do vetor (usando Bubble Sort)
void ordenarVetor();
// Declaração da função para buscar um item no vetor usando busca sequencial
void buscarSequencialVetor();
// Declaração da função para buscar um item no vetor usando busca binária (requer vetor ordenado)
void buscarBinariaVetor();

// ---------------------- Protótipos das Funções para Mochila (Lista Encadeada) ----------------------
// Declaração da função para inserir um item na lista encadeada
void inserirItemLista();
// Declaração da função para remover um item da lista encadeada
void removerItemLista();
// Declaração da função para listar todos os itens na lista encadeada
void listarItensLista();
// Declaração da função para buscar um item na lista encadeada usando busca sequencial
void buscarSequencialLista();
// Declaração da função para liberar toda a memória alocada dinamicamente para a lista encadeada
void liberarLista();

// ---------------------- Função Principal (main) ----------------------
int main() {
    int opcaoPrincipal; // Variável para armazenar a opção de escolha da estrutura de dados
    int opcaoMochila;   // Variável para armazenar a opção da operação na mochila selecionada

    // Inicializa o gerador de números aleatórios com a hora atual.
    // É uma boa prática para funções que utilizam números aleatórios (rand()),
    // garantindo que sequências diferentes sejam geradas a cada execução do programa.
    srand(time(NULL));

    // Loop principal do programa que exibe o menu de escolha da estrutura de dados
    do {
        system("clear || cls"); // Limpa a tela do console.
                                // "clear" para sistemas Unix-like (Linux/macOS), "cls" para Windows.
        exibirMenuPrincipal();  // Chama a função para exibir o menu de escolha da estrutura
        printf("Escolha uma opcao: ");
        scanf("%d", &opcaoPrincipal); // Lê a opção do usuário
        limparBuffer(); // Limpa o buffer de entrada após a leitura de um inteiro

        // Estrutura switch para processar a opção principal escolhida pelo usuário
        switch (opcaoPrincipal) {
            case 1: // Opção 1: Mochila com Vetor
                do {
                    system("clear || cls"); // Limpa a tela para o menu da mochila de vetor
                    printf("==========================================\n");
                    printf("MOCHILA DE SOBREVIVENCIA - VERSAO VETOR\n");
                    printf("==========================================\n");
                    // Exibe a capacidade atual do inventário do vetor
                    printf("Capacidade do inventario (%d/%d)\n", numItensVetor, MAX_ITENS);
                    exibirMenuMochila(); // Chama a função para exibir o menu de operações do inventário
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcaoMochila); // Lê a opção de operação para o vetor
                    limparBuffer(); // Limpa o buffer

                    // Reinicia os contadores de comparações antes de cada nova busca
                    comparacoesBuscaSequencialVetor = 0;
                    comparacoesBuscaBinariaVetor = 0;

                    // Estrutura switch para executar a operação escolhida na mochila de vetor
                    switch (opcaoMochila) {
                        case 1: inserirItemVetor(); break;       // Chama a função para inserir item
                        case 2: removerItemVetor(); break;       // Chama a função para remover item
                        case 3: listarItensVetor(); break;       // Chama a função para listar itens
                        case 4: buscarSequencialVetor(); break;  // Chama a função para busca sequencial
                        case 5: ordenarVetor(); break;           // Chama a função para ordenar o vetor
                        case 6: buscarBinariaVetor(); break;     // Chama a função para busca binária
                        case 0: printf("Voltando ao menu principal...\n"); break; // Volta ao menu principal
                        default: printf("Opcao invalida. Tente novamente.\n"); break; // Mensagem de erro para opção inválida
                    }
                    // Se o usuário não escolheu "Voltar ao Menu Principal", pausa e aguarda ENTER
                    if (opcaoMochila != 0) {
                        printf("\nPressione ENTER para continuar...");
                        getchar(); // Aguarda o usuário pressionar ENTER para continuar
                    }
                } while (opcaoMochila != 0); // O loop continua até que o usuário escolha '0' para voltar
                break;

            case 2: // Opção 2: Mochila com Lista Encadeada
                do {
                    system("clear || cls"); // Limpa a tela para o menu da mochila de lista encadeada
                    printf("==========================================\n");
                    printf("MOCHILA DE SOBREVIVENCIA - VERSAO LISTA ENCADEADA\n");
                    printf("==========================================\n");
                    // Exibe a quantidade atual de itens na lista encadeada
                    printf("Itens no inventario: %d\n", numItensLista);
                    exibirMenuMochila(); // Chama a função para exibir o menu de operações do inventário
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcaoMochila); // Lê a opção de operação para a lista encadeada
                    limparBuffer(); // Limpa o buffer

                    // Reinicia o contador de comparações antes de cada nova busca
                    comparacoesBuscaSequencialLista = 0;

                    // Estrutura switch para executar a operação escolhida na mochila de lista encadeada
                    switch (opcaoMochila) {
                        case 1: inserirItemLista(); break;      // Chama a função para inserir item
                        case 2: removerItemLista(); break;      // Chama a função para remover item
                        case 3: listarItensLista(); break;      // Chama a função para listar itens
                        case 4: buscarSequencialLista(); break; // Chama a função para busca sequencial
                        case 5: // Ordenação direta não é padrão ou eficiente para lista encadeada sem reestruturação complexa
                                printf("Operacao de ordenacao nao disponivel diretamente para lista encadeada (requer algoritmos especificos ou conversao para vetor).\n");
                                break;
                        case 6: // Busca binária não é eficiente ou padrão para lista encadeada (requer acesso aleatório por índice)
                                printf("Busca binaria nao e eficiente ou padrao para lista encadeada (requer acesso por indice).\n");
                                break;
                        case 0: printf("Voltando ao menu principal...\n"); break; // Volta ao menu principal
                        default: printf("Opcao invalida. Tente novamente.\n"); break; // Mensagem de erro
                    }
                    // Se o usuário não escolheu "Voltar ao Menu Principal", pausa e aguarda ENTER
                     if (opcaoMochila != 0) {
                        printf("\nPressione ENTER para continuar...");
                        getchar(); // Aguarda o usuário pressionar ENTER para continuar
                    }
                } while (opcaoMochila != 0); // O loop continua até que o usuário escolha '0' para voltar
                break;

            case 0: // Opção 0: Sair do Programa
                printf("Saindo do programa. Ate mais!\n"); // Mensagem de despedida
                liberarLista(); // Chama a função para liberar toda a memória alocada para a lista encadeada
                break;

            default: // Caso a opção principal seja inválida
                printf("Opcao invalida. Por favor, tente novamente.\n"); // Mensagem de erro
                sleep(2); // Pausa por 2 segundos para o usuário ler a mensagem
                break;
        }
    } while (opcaoPrincipal != 0); // O loop continua até que o usuário escolha '0' para sair do programa

    return EXIT_SUCCESS; // Retorna 0 para indicar que o programa terminou com sucesso
}

// ---------------------- Implementação das Funções Comuns ----------------------

// Função para limpar o buffer de entrada (stdin)
void limparBuffer() {
    int c; // Variável para armazenar cada caractere lido
    // Loop para ler e descartar caracteres do buffer até encontrar um '\n' (nova linha) ou EOF (fim do arquivo)
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para exibir o menu principal, onde o jogador escolhe a estrutura de dados
void exibirMenuPrincipal() {
    printf("==========================================\n");
    printf("MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
    printf("==========================================\n");
    printf("\n--- Escolha a Estrutura de Dados ---\n");
    printf("1. Mochila com Vetor (Lista Sequencial)\n");
    printf("2. Mochila com Lista Encadeada (Estrutura Dinamica)\n");
    printf("0. Sair do Programa\n");
    printf("------------------------------------\n");
}

// Função para exibir o menu de operações que podem ser realizadas em qualquer tipo de mochila
void exibirMenuMochila() {
    printf("\n--- Operacoes na Mochila ---\n");
    printf("1. Cadastrar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Buscar Item (Busca Sequencial)\n");
    printf("5. Ordenar Itens (Apenas Vetor)\n");
    printf("6. Buscar Item (Busca Binaria - Apenas Vetor Ordenado)\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("----------------------------\n");
}

// ---------------------- Implementação das Funções para Mochila (Vetor) ----------------------

// Função para inserir um novo item na mochila que utiliza um vetor (lista sequencial)
void inserirItemVetor() {
    // Verifica se o vetor está cheio. Se sim, não é possível adicionar mais itens.
    if (numItensVetor >= MAX_ITENS) {
        printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return; // Sai da função
    }

    printf("\n--- Cadastrar Novo Item (Vetor) ---\n");
    printf("Nome do Item: ");
    // Usa fgets para ler o nome do item. É mais seguro que scanf("%s") pois previne buffer overflow.
    fgets(mochilaVetor[numItensVetor].nome, sizeof(mochilaVetor[numItensVetor].nome), stdin);
    // Remove o caractere de nova linha ('\n') que fgets pode incluir no final da string.
    mochilaVetor[numItensVetor].nome[strcspn(mochilaVetor[numItensVetor].nome, "\n")] = 0;

    printf("Tipo do Item (ex: arma, municao, cura, ferramenta): ");
    fgets(mochilaVetor[numItensVetor].tipo, sizeof(mochilaVetor[numItensVetor].tipo), stdin);
    mochilaVetor[numItensVetor].tipo[strcspn(mochilaVetor[numItensVetor].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &mochilaVetor[numItensVetor].quantidade); // Lê a quantidade do item
    limparBuffer(); // Limpa o buffer após a leitura de um inteiro

    numItensVetor++; // Incrementa o contador de itens no vetor
    printf("Item '%s' cadastrado com sucesso na mochila (Vetor)!\n", mochilaVetor[numItensVetor - 1].nome);
}

// Função para remover um item da mochila (vetor) com base no seu nome
void removerItemVetor() {
    char nomeRemover[30]; // Buffer para armazenar o nome do item que será removido
    int i, j;             // Variáveis de índice para os loops
    int encontrado = 0;   // Flag para indicar se o item foi encontrado (0 = não, 1 = sim)

    // Verifica se a mochila está vazia. Se sim, não há o que remover.
    if (numItensVetor == 0) {
        printf("A mochila (Vetor) esta vazia. Nao ha itens para remover.\n");
        return; // Sai da função
    }

    printf("\n--- Remover Item da Mochila (Vetor) ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin); // Lê o nome do item a remover
    nomeRemover[strcspn(nomeRemover, "\n")] = 0;   // Remove o '\n'

    // Loop para percorrer o vetor em busca do item
    for (i = 0; i < numItensVetor; i++) {
        // Compara o nome do item atual no vetor com o nome a ser removido
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(mochilaVetor[i].nome, nomeRemover) == 0) {
            encontrado = 1; // Item encontrado, define a flag
            // Loop para deslocar os itens subsequentes uma posição para trás,
            // efetivamente "removendo" o item encontrado.
            for (j = i; j < numItensVetor - 1; j++) {
                mochilaVetor[j] = mochilaVetor[j + 1];
            }
            numItensVetor--; // Decrementa o contador de itens
            printf("Item '%s' removido com sucesso da mochila (Vetor)!\n", nomeRemover);
            break; // Sai do loop 'for' após remover o item
        }
    }

    // Se a flag 'encontrado' ainda for 0, o item não foi encontrado no vetor
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila (Vetor).\n", nomeRemover);
    }
}

// Função para listar todos os itens atualmente armazenados na mochila (vetor)
void listarItensVetor() {
    int i; // Variável de controle do loop

    printf("\n--- Itens Atuais na Mochila (Vetor) (%d/%d) ---\n", numItensVetor, MAX_ITENS);

    // Verifica se o vetor está vazio. Se sim, imprime uma mensagem e sai.
    if (numItensVetor == 0) {
        printf("A mochila (Vetor) esta vazia.\n");
        return; // Sai da função
    }

    // Imprime um cabeçalho formatado para a tabela de itens
    printf("----------------------------------------------------\n");
    printf("%-20s %-15s %-10s\n", "Nome", "Tipo", "Quantidade"); // Formato para colunas
    printf("----------------------------------------------------\n");

    // Loop para percorrer e imprimir os dados de cada item no vetor
    for (i = 0; i < numItensVetor; i++) {
        // Imprime os dados do item atual, usando formatação para alinhar as colunas
        printf("%-20s %-15s %-10d\n", mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
    }
    printf("----------------------------------------------------\n"); // Linha de fechamento da tabela
}

// Função para ordenar os itens da mochila (vetor) por nome em ordem alfabética (usando Bubble Sort)
void ordenarVetor() {
    int i, j;   // Variáveis de controle para os loops do Bubble Sort
    Item temp;  // Variável temporária para auxiliar na troca de itens durante a ordenação

    // Se houver 0 ou 1 item, o vetor já está "ordenado" ou não precisa de ordenação.
    if (numItensVetor <= 1) {
        printf("Nao ha itens suficientes na mochila (Vetor) para ordenar.\n");
        return; // Sai da função
    }

    printf("\n--- Ordenando Itens na Mochila (Vetor) por Nome ---\n");
    // Algoritmo Bubble Sort:
    // O loop externo controla o número de passes necessários.
    for (i = 0; i < numItensVetor - 1; i++) {
        // O loop interno percorre os elementos e 'flutua' o maior elemento para sua posição correta.
        // A cada pass, o maior elemento restante já está na sua posição final, por isso 'numItensVetor - 1 - i'.
        for (j = 0; j < numItensVetor - 1 - i; j++) {
            // Compara os nomes de dois itens adjacentes usando strcmp.
            // strcmp retorna um valor > 0 se o primeiro nome for "maior" (vem depois no alfabeto) que o segundo.
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                // Se os itens estão fora de ordem, realiza a troca de posições.
                temp = mochilaVetor[j];            // Guarda o item atual em 'temp'
                mochilaVetor[j] = mochilaVetor[j + 1]; // Move o próximo item para a posição atual
                mochilaVetor[j + 1] = temp;        // Coloca o item original (guardado em temp) na próxima posição
            }
        }
    }
    printf("Itens da mochila (Vetor) ordenados com sucesso por nome!\n");
    listarItensVetor(); // Após ordenar, lista os itens para mostrar o resultado
}

// Função para buscar um item na mochila (vetor) usando busca sequencial
void buscarSequencialVetor() {
    char nomeBuscar[30]; // Buffer para armazenar o nome do item a ser buscado
    int i;              // Variável de índice para o loop
    int encontrado = 0; // Flag para indicar se o item foi encontrado

    // Verifica se o vetor está vazio. Se sim, não há o que buscar.
    if (numItensVetor == 0) {
        printf("A mochila (Vetor) esta vazia. Nao ha itens para buscar.\n");
        return; // Sai da função
    }

    printf("\n--- Buscar Item na Mochila (Vetor) - Busca Sequencial ---\n");
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeBuscar, sizeof(nomeBuscar), stdin); // Lê o nome a ser buscado
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;   // Remove o '\n'

    comparacoesBuscaSequencialVetor = 0; // Reinicia o contador de comparações para esta busca

    // Loop para percorrer o vetor do início ao fim
    for (i = 0; i < numItensVetor; i++) {
        comparacoesBuscaSequencialVetor++; // Incrementa o contador a cada comparação realizada
        // Compara o nome do item atual no vetor com o nome a ser buscado
        if (strcmp(mochilaVetor[i].nome, nomeBuscar) == 0) {
            encontrado = 1; // Item encontrado, define a flag
            printf("\nItem Encontrado:\n");
            printf("Nome: %s\n", mochilaVetor[i].nome);       // Imprime o nome
            printf("Tipo: %s\n", mochilaVetor[i].tipo);       // Imprime o tipo
            printf("Quantidade: %d\n", mochilaVetor[i].quantidade); // Imprime a quantidade
            break; // Sai do loop 'for' assim que o item é encontrado
        }
    }

    // Se a flag 'encontrado' ainda for 0, o item não foi encontrado
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila (Vetor).\n", nomeBuscar);
    }
    // Exibe o total de comparações feitas durante a busca
    printf("Comparacoes realizadas (Busca Sequencial): %lld\n", comparacoesBuscaSequencialVetor);
}

// Função para buscar um item na mochila (vetor) usando busca binária
// PRÉ-REQUISITO: O vetor DEVE estar ordenado por nome para que a busca binária funcione corretamente.
void buscarBinariaVetor() {
    char nomeBuscar[30];    // Buffer para armazenar o nome do item a ser buscado
    int esquerda = 0;       // Índice inicial do sub-vetor a ser pesquisado
    int direita = numItensVetor - 1; // Índice final do sub-vetor a ser pesquisado
    int meio;               // Índice do meio do sub-vetor
    int encontrado = 0;     // Flag para indicar se o item foi encontrado

    // Verifica se o vetor está vazio.
    if (numItensVetor == 0) {
        printf("A mochila (Vetor) esta vazia. Nao ha itens para buscar.\n");
        return; // Sai da função
    }

    printf("\n--- Buscar Item na Mochila (Vetor) - Busca Binaria ---\n");
    printf("ATENCAO: Esta busca requer que os itens estejam ORDENADOS por nome.\n");
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeBuscar, sizeof(nomeBuscar), stdin); // Lê o nome a ser buscado
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;   // Remove o '\n'

    comparacoesBuscaBinariaVetor = 0; // Reinicia o contador de comparações para esta busca

    // O loop continua enquanto o lado esquerdo não ultrapassar o lado direito
    while (esquerda <= direita) {
        meio = esquerda + (direita - esquerda) / 2; // Calcula o índice do meio para evitar overflow em grandes arrays
        comparacoesBuscaBinariaVetor++;             // Incrementa o contador a cada comparação

        // Compara o nome do item no meio do vetor com o nome a ser buscado
        int cmp = strcmp(mochilaVetor[meio].nome, nomeBuscar);

        if (cmp == 0) { // Se cmp é 0, os nomes são iguais, item encontrado
            encontrado = 1; // Define a flag
            printf("\nItem Encontrado:\n");
            printf("Nome: %s\n", mochilaVetor[meio].nome);
            printf("Tipo: %s\n", mochilaVetor[meio].tipo);
            printf("Quantidade: %d\n", mochilaVetor[meio].quantidade);
            break; // Sai do loop 'while'
        } else if (cmp < 0) { // Se cmp é < 0, o nome no meio é "menor" (vem antes no alfabeto) que o buscado.
                               // Isso significa que o item buscado (se existir) está na metade direita.
            esquerda = meio + 1; // Ajusta o limite inferior da busca
        } else { // Se cmp é > 0, o nome no meio é "maior" (vem depois no alfabeto) que o buscado.
                 // O item buscado (se existir) está na metade esquerda.
            direita = meio - 1; // Ajusta o limite superior da busca
        }
    }

    // Se a flag 'encontrado' ainda for 0, o item não foi encontrado
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila (Vetor).\n", nomeBuscar);
    }
    // Exibe o total de comparações feitas durante a busca
    printf("Comparacoes realizadas (Busca Binaria): %lld\n", comparacoesBuscaBinariaVetor);
}


// ---------------------- Implementação das Funções para Mochila (Lista Encadeada) ----------------------

// Função para inserir um novo item na mochila que utiliza uma lista encadeada.
// Para simplicidade, a inserção é feita no início da lista.
void inserirItemLista() {
    // Aloca dinamicamente memória para um novo nó (struct No)
    No* novoNo = (No*)malloc(sizeof(No));

    // Verifica se a alocação de memória foi bem-sucedida. Se malloc retornar NULL, houve um erro.
    if (novoNo == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o novo item.\n");
        return; // Sai da função
    }

    printf("\n--- Cadastrar Novo Item (Lista Encadeada) ---\n");
    printf("Nome do Item: ");
    fgets(novoNo->dados.nome, sizeof(novoNo->dados.nome), stdin); // Lê o nome do item
    novoNo->dados.nome[strcspn(novoNo->dados.nome, "\n")] = 0;   // Remove o '\n'

    printf("Tipo do Item (ex: arma, municao, cura, ferramenta): ");
    fgets(novoNo->dados.tipo, sizeof(novoNo->dados.tipo), stdin); // Lê o tipo do item
    novoNo->dados.tipo[strcspn(novoNo->dados.tipo, "\n")] = 0;   // Remove o '\n'

    printf("Quantidade: ");
    scanf("%d", &novoNo->dados.quantidade); // Lê a quantidade do item
    limparBuffer(); // Limpa o buffer após a leitura de um inteiro

    // Insere o novo nó no início da lista:
    novoNo->proximo = headLista; // O campo 'proximo' do novo nó aponta para o antigo início da lista
    headLista = novoNo;          // O 'headLista' agora aponta para o novo nó, tornando-o o primeiro
    numItensLista++;             // Incrementa o contador de itens na lista
    printf("Item '%s' cadastrado com sucesso na mochila (Lista Encadeada)!\n", novoNo->dados.nome);
}

// Função para remover um item da mochila (lista encadeada) com base no seu nome
void removerItemLista() {
    char nomeRemover[30];      // Buffer para armazenar o nome do item a ser removido
    No* atual = headLista;     // Ponteiro que percorre a lista, apontando para o nó atual
    No* anterior = NULL;       // Ponteiro que aponta para o nó anterior ao 'atual'
    int encontrado = 0;        // Flag para indicar se o item foi encontrado

    // Verifica se a lista está vazia.
    if (headLista == NULL) {
        printf("A mochila (Lista Encadeada) esta vazia. Nao ha itens para remover.\n");
        return; // Sai da função
    }

    printf("\n--- Remover Item da Mochila (Lista Encadeada) ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin); // Lê o nome do item a remover
    nomeRemover[strcspn(nomeRemover, "\n")] = 0;   // Remove o '\n'

    // Loop para percorrer a lista encadeada
    while (atual != NULL) {
        // Compara o nome do item no nó atual com o nome a ser removido
        if (strcmp(atual->dados.nome, nomeRemover) == 0) {
            encontrado = 1; // Item encontrado, define a flag

            // Caso especial: o nó a ser removido é o primeiro da lista (head)
            if (anterior == NULL) {
                headLista = atual->proximo; // O novo 'head' será o próximo nó
            } else {
                // O nó anterior agora aponta para o nó que vem depois do nó atual (pulando o nó atual)
                anterior->proximo = atual->proximo;
            }
            free(atual); // Libera a memória alocada para o nó removido
            numItensLista--; // Decrementa o contador de itens na lista
            printf("Item '%s' removido com sucesso da mochila (Lista Encadeada)!\n", nomeRemover);
            break; // Sai do loop 'while' após remover o item
        }
        anterior = atual;          // Atualiza 'anterior' para ser o 'atual'
        atual = atual->proximo;    // Move 'atual' para o próximo nó da lista
    }

    // Se a flag 'encontrado' ainda for 0, o item não foi encontrado
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila (Lista Encadeada).\n", nomeRemover);
    }
}

// Função para listar todos os itens atualmente armazenados na mochila (lista encadeada)
void listarItensLista() {
    No* atual = headLista; // Ponteiro que começa no início da lista (head)
    // int i = 0; // Variável de controle (não essencial para listagem, mas pode ser usada para numerar)

    printf("\n--- Itens Atuais na Mochila (Lista Encadeada) (%d itens) ---\n", numItensLista);

    // Verifica se a lista está vazia.
    if (headLista == NULL) {
        printf("A mochila (Lista Encadeada) esta vazia.\n");
        return; // Sai da função
    }

    // Imprime um cabeçalho formatado para a tabela de itens
    printf("----------------------------------------------------\n");
    printf("%-20s %-15s %-10s\n", "Nome", "Tipo", "Quantidade");
    printf("----------------------------------------------------\n");

    // Loop para percorrer a lista encadeada e imprimir os dados de cada nó
    while (atual != NULL) {
        // Imprime os dados do item no nó atual
        printf("%-20s %-15s %-10d\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo; // Move o ponteiro 'atual' para o próximo nó da lista
        // i++; // Incrementa o contador, se estiver sendo usado para numerar itens
    }
    printf("----------------------------------------------------\n"); // Linha de fechamento da tabela
}

// Função para buscar um item na mochila (lista encadeada) usando busca sequencial
void buscarSequencialLista() {
    char nomeBuscar[30]; // Buffer para armazenar o nome do item a ser buscado
    No* atual = headLista; // Ponteiro que percorre a lista, começando pelo head
    int encontrado = 0;    // Flag para indicar se o item foi encontrado

    // Verifica se a lista está vazia.
    if (headLista == NULL) {
        printf("A mochila (Lista Encadeada) esta vazia. Nao ha itens para buscar.\n");
        return; // Sai da função
    }

    printf("\n--- Buscar Item na Mochila (Lista Encadeada) - Busca Sequencial ---\n");
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeBuscar, sizeof(nomeBuscar), stdin); // Lê o nome a ser buscado
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;   // Remove o '\n'

    comparacoesBuscaSequencialLista = 0; // Reinicia o contador de comparações para esta busca

    // Loop para percorrer a lista encadeada sequencialmente
    while (atual != NULL) {
        comparacoesBuscaSequencialLista++; // Incrementa o contador a cada comparação
        // Compara o nome do item no nó atual com o nome a ser buscado
        if (strcmp(atual->dados.nome, nomeBuscar) == 0) {
            encontrado = 1; // Item encontrado, define a flag
            printf("\nItem Encontrado:\n");
            printf("Nome: %s\n", atual->dados.nome);       // Imprime o nome
            printf("Tipo: %s\n", atual->dados.tipo);       // Imprime o tipo
            printf("Quantidade: %d\n", atual->dados.quantidade); // Imprime a quantidade
            break; // Sai do loop 'while' assim que o item é encontrado
        }
        atual = atual->proximo; // Move o ponteiro 'atual' para o próximo nó
    }

    // Se a flag 'encontrado' ainda for 0, o item não foi encontrado
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila (Lista Encadeada).\n", nomeBuscar);
    }
    // Exibe o total de comparações feitas durante a busca
    printf("Comparacoes realizadas (Busca Sequencial): %lld\n", comparacoesBuscaSequencialLista);
}

// Função para liberar toda a memória alocada dinamicamente para os nós da lista encadeada
void liberarLista() {
    No* atual = headLista; // Ponteiro que começa no início da lista (head)
    No* proximoNo;         // Ponteiro temporário para guardar o próximo nó

    // Loop que percorre a lista. Continua enquanto houver nós.
    while (atual != NULL) {
        proximoNo = atual->proximo; // Guarda o endereço do próximo nó antes de liberar o nó atual
        free(atual);                // Libera a memória do nó atual
        atual = proximoNo;          // Move o ponteiro 'atual' para o próximo nó guardado
    }
    headLista = NULL; // Após liberar todos os nós, define headLista como NULL para indicar que a lista está vazia
    numItensLista = 0; // Zera o contador de itens na lista
    printf("Memoria da lista encadeada liberada com sucesso.\n"); // Mensagem de confirmação

            return 0;

}