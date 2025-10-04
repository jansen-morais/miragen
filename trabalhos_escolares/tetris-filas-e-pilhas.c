#include <stdio.h>   // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrao (malloc, calloc, free, rand, srand) para alocacao dinamica e numeros aleatorios.
#include <string.h>  // Inclui a biblioteca para manipulacao de strings (strcspn, strcpy, strcmp).
#include <unistd.h>  // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>    // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().


// --- Definições de Estruturas e Constantes ---

// Define a capacidade máxima da Fila de Peças (5)
#define MAX_FILA 5
// Define a capacidade máxima da Pilha de Reserva (3)
#define MAX_PILHA 3

// Variável global para gerar IDs únicos para as peças
int proximo_id = 0;

/**
 * @brief Estrutura que representa uma Peça do Tetris.
 *
 * nome: Caractere que identifica o tipo da peça ('I', 'O', 'T', 'L').
 * id: Número inteiro único para rastrear a ordem de criação.
 */
typedef struct {
    char nome;
    int id;
} Peca;

/**
 * @brief Estrutura para gerenciar a Fila Circular de Peças.
 *
 * itens: Array de peças que armazena os elementos da fila.
 * inicio: Índice do primeiro elemento da fila.
 * fim: Índice da próxima posição livre (após o último elemento).
 * tamanho_atual: Número de elementos atualmente na fila.
 */
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int tamanho_atual;
} FilaCircular;

/**
 * @brief Estrutura para gerenciar a Pilha de Reserva de Peças.
 *
 * itens: Array de peças que armazena os elementos da pilha.
 * topo: Índice da última peça inserida (o topo da pilha).
 */
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// --- Funções de Utilitário e Limpeza ---

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar erros em leituras futuras.
 * É essencial após ler um número inteiro (scanf("%d", ...)) e antes de ler
 * uma string ou outro inteiro em um loop de menu.
 */
void limpar_buffer() {
    int c;
    // Lê e descarta todos os caracteres até encontrar um newline (\n) ou EOF
    while ((c = getchar()) != '\n' && c != EOF) {
        // Bloco vazio, apenas descarta o caractere
    }
}

/**
 * @brief Limpa o array de Peças, inicializando-o com valores nulos/vazios.
 * @param array Array de Peças a ser limpo.
 * @param tamanho Tamanho do array.
 */
void limpar_array_pecas(Peca array[], int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        // Inicializa o nome com um caractere nulo (0) para indicar que a posição está vazia
        array[i].nome = '\0';
        // Inicializa o ID com um valor inválido (-1)
        array[i].id = -1;
    }
}

// --- Funções de Peças e Inicialização ---

/**
 * @brief Gera uma nova peça com um tipo aleatório e um ID único.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    // Array de tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    // Escolhe um tipo de forma pseudo-aleatória
    int indice_aleatorio = rand() % num_tipos;
    nova_peca.nome = tipos[indice_aleatorio];

    // Atribui o ID único e incrementa o contador global
    nova_peca.id = proximo_id;
    proximo_id++;

    return nova_peca;
}

/**
 * @brief Inicializa a Fila Circular.
 * @param fila Ponteiro para a estrutura da fila.
 */
void inicializar_fila(FilaCircular *fila) {
    fila->inicio = 0;          // O início aponta para a posição 0
    fila->fim = 0;             // O fim (próxima posição livre) aponta para a posição 0
    fila->tamanho_atual = 0;   // A fila começa vazia
    limpar_array_pecas(fila->itens, MAX_FILA); // Limpa o array de itens
}

/**
 * @brief Inicializa a Pilha.
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void inicializar_pilha(Pilha *pilha) {
    pilha->topo = -1; // -1 indica que a pilha está vazia (o topo não aponta para nenhum elemento)
    limpar_array_pecas(pilha->itens, MAX_PILHA); // Limpa o array de itens
}

/**
 * @brief Preenche a fila com peças iniciais até a capacidade máxima.
 * @param fila Ponteiro para a estrutura da fila.
 */
void preencher_fila_inicial(FilaCircular *fila) {
    printf("\n------------------------------------------------------\n");
    printf("\n               *****ByteBros*****\n");
    printf("\n------------------------------------------------------\n");
    sleep(2.5);
    printf("\n------------------------------------------------------\n");
    printf("\n            ##### Tetris Stack #####\n");
    printf("\n------------------------------------------------------\n");
    sleep(2);

    printf("\n--- Inicializando a Fila de Pecas ---\n");
    while (fila->tamanho_atual < MAX_FILA) {
        Peca nova = gerarPeca();
        // Simula o enqueue (inserir)
        fila->itens[fila->fim] = nova;
        fila->fim = (fila->fim + 1) % MAX_FILA; // Move o 'fim' de forma circular
        fila->tamanho_atual++;
        printf("Peca [%c %d] adicionada.\n", nova.nome, nova.id);
    }
}

// --- Funções de Fila Circular (Queue) ---

/**
 * @brief Verifica se a fila está cheia.
 * @param fila Ponteiro para a estrutura da fila.
 * @return int 1 se cheia, 0 caso contrário.
 */
int fila_cheia(FilaCircular *fila) {
    // A fila está cheia se o tamanho atual for igual ao tamanho máximo
    return fila->tamanho_atual == MAX_FILA;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param fila Ponteiro para a estrutura da fila.
 * @return int 1 se vazia, 0 caso contrário.
 */
int fila_vazia(FilaCircular *fila) {
    // A fila está vazia se o tamanho atual for zero
    return fila->tamanho_atual == 0;
}

/**
 * @brief Insere uma peça no final da fila (enqueue).
 * @param fila Ponteiro para a estrutura da fila.
 * @param peca A peça a ser inserida.
 * @return int 1 se a inserção foi bem-sucedida, 0 caso contrário (fila cheia).
 */
int enqueue(FilaCircular *fila, Peca peca) {
    if (fila_cheia(fila)) {
        // Não deve ocorrer neste programa, pois a fila será sempre mantida cheia,
        // mas é um bom princípio de programação.
        // printf("ERRO: Fila cheia. Nao e possivel inserir a peca [%c %d].\n", peca.nome, peca.id);
        return 0;
    }

    // Insere a peça na posição de 'fim'
    fila->itens[fila->fim] = peca;
    // Move o 'fim' para a próxima posição livre de forma circular
    fila->fim = (fila->fim + 1) % MAX_FILA;
    // Incrementa o tamanho da fila
    fila->tamanho_atual++;
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * @param fila Ponteiro para a estrutura da fila.
 * @param peca_removida Ponteiro para onde a peça removida será copiada.
 * @return int 1 se a remoção foi bem-sucedida, 0 caso contrário (fila vazia).
 */
int dequeue(FilaCircular *fila, Peca *peca_removida) {
    if (fila_vazia(fila)) {
        printf("ERRO: Fila vazia. Nao e possivel remover pecas.\n");
        return 0;
    }

    // Copia a peça da frente da fila
    *peca_removida = fila->itens[fila->inicio];

    // "Limpa" a posição de início (opcional, mas boa prática para visualização)
    fila->itens[fila->inicio].nome = '\0';
    fila->itens[fila->inicio].id = -1;

    // Move o 'inicio' para a próxima peça de forma circular
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    // Decrementa o tamanho da fila
    fila->tamanho_atual--;
    return 1;
}

/**
 * @brief Retorna a peça da frente da fila sem removê-la (peek).
 * @param fila Ponteiro para a estrutura da fila.
 * @return Peca A peça da frente. Retorna uma peça com nome '\0' se a fila estiver vazia.
 */
Peca frente_da_fila(FilaCircular *fila) {
    if (fila_vazia(fila)) {
        // Retorna uma peça vazia
        Peca vazia = {'\0', -1};
        return vazia;
    }
    // Retorna a peça no índice de 'inicio'
    return fila->itens[fila->inicio];
}

// --- Funções de Pilha (Stack) ---

/**
 * @brief Verifica se a pilha está cheia.
 * @param pilha Ponteiro para a estrutura da pilha.
 * @return int 1 se cheia, 0 caso contrário.
 */
int pilha_cheia(Pilha *pilha) {
    // A pilha está cheia se o topo for igual ao índice máximo (MAX_PILHA - 1)
    return pilha->topo == MAX_PILHA - 1;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha Ponteiro para a estrutura da pilha.
 * @return int 1 se vazia, 0 caso contrário.
 */
int pilha_vazia(Pilha *pilha) {
    // A pilha está vazia se o topo for -1
    return pilha->topo == -1;
}

/**
 * @brief Insere uma peça no topo da pilha (push).
 * @param pilha Ponteiro para a estrutura da pilha.
 * @param peca A peça a ser inserida.
 * @return int 1 se a inserção foi bem-sucedida, 0 caso contrário (pilha cheia).
 */
int push(Pilha *pilha, Peca peca) {
    if (pilha_cheia(pilha)) {
        printf("AVISO: Pilha de reserva cheia. Nao e possivel reservar a peca [%c %d].\n", peca.nome, peca.id);
        return 0;
    }

    // Incrementa o topo
    pilha->topo++;
    // Insere a peça na nova posição do topo
    pilha->itens[pilha->topo] = peca;
    return 1;
}

/**
 * @brief Remove a peça do topo da pilha (pop).
 * @param pilha Ponteiro para a estrutura da pilha.
 * @param peca_removida Ponteiro para onde a peça removida será copiada.
 * @return int 1 se a remoção foi bem-sucedida, 0 caso contrário (pilha vazia).
 */
int pop(Pilha *pilha, Peca *peca_removida) {
    if (pilha_vazia(pilha)) {
        printf("AVISO: Pilha de reserva vazia. Nao ha pecas para usar.\n");
        return 0;
    }

    // Copia a peça do topo da pilha
    *peca_removida = pilha->itens[pilha->topo];

    // "Limpa" a posição do topo (opcional, mas boa prática)
    pilha->itens[pilha->topo].nome = '\0';
    pilha->itens[pilha->topo].id = -1;

    // Decrementa o topo
    pilha->topo--;
    return 1;
}

/**
 * @brief Retorna a peça do topo da pilha sem removê-la (peek).
 * @param pilha Ponteiro para a estrutura da pilha.
 * @return Peca A peça do topo. Retorna uma peça com nome '\0' se a pilha estiver vazia.
 */
Peca topo_da_pilha(Pilha *pilha) {
    if (pilha_vazia(pilha)) {
        // Retorna uma peça vazia
        Peca vazia = {'\0', -1};
        return vazia;
    }
    // Retorna a peça no índice de 'topo'
    return pilha->itens[pilha->topo];
}

// --- Funções de Visualização ---

/**
 * @brief Exibe o estado atual da Fila Circular e da Pilha.
 * @param fila Ponteiro para a estrutura da fila.
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void exibir_estado_atual(FilaCircular *fila, Pilha *pilha) {
    int i;

    printf("\n------------------------------------------------------\n");
    printf("Estado Atual:\n");

    // --- Visualização da Fila ---
    printf("Fila de pecas (%d/%d): ", fila->tamanho_atual, MAX_FILA);

    if (fila_vazia(fila)) {
        printf("Vazia\n");
    } else {
        // Percorre o array da fila do início ao fim (índice a índice)
        for (i = 0; i < MAX_FILA; i++) {
            // Verifica se a posição atual (i) é a de início da fila
            if (i == fila->inicio && fila->tamanho_atual > 0) {
                printf(" -> "); // Indicador do "início" (frente)
            }

            // A forma mais correta de percorrer os elementos ativos da fila circular
            // seria: (fila->inicio + k) % MAX_FILA, onde k vai de 0 a tamanho_atual - 1.
            // Para simplificar a visualização do array subjacente (como no exemplo),
            // percorreremos o array completo e mostraremos apenas as posições válidas
            // (e a posição no array).

            // Se o elemento na posição for válido (nome não é nulo)
            if (fila->itens[i].nome != '\0') {
                printf("[%c %d] ", fila->itens[i].nome, fila->itens[i].id);
            } else {
                // Mostra um marcador de posição vazia (se houver, o que só deve ocorrer
                // em casos extremos ou após uma limpeza que não move o array, como
                // ocorre aqui para visualizar a estrutura circular).
                printf("[-- -] ");
            }

            // Verifica se a posição atual (i) é a anterior ao 'fim' da fila
            if (i == (fila->fim - 1 + MAX_FILA) % MAX_FILA && fila->tamanho_atual > 0) {
                printf(" <- "); // Indicador do "fim" (traseira)
            }
        }
        printf("\n");
    }

    // --- Visualização da Pilha ---
    printf("Pilha de reserva (Topo -> Base) (%d/%d): ", pilha->topo + 1, MAX_PILHA);

    if (pilha_vazia(pilha)) {
        printf("Vazia\n");
    } else {
        // Percorre a pilha do topo (maior índice) até a base (índice 0)
        for (i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
        printf("\n");
    }

    printf("------------------------------------------------------\n");
}

/**
 * @brief Exibe o menu de opções disponíveis.
 */
void exibir_menu() {
    printf("\nOpcoes disponiveis:\n");
    printf("Codigo | Acao\n");
    printf("-------|----------------------------------------------\n");
    printf("  1    | Jogar peca da frente da fila (Dequeue)\n");
    printf("  2    | Enviar peca da fila para a pilha de reserva\n");
    printf("  3    | Usar peca da pilha de reserva (Pop)\n");
    printf("  4    | Trocar peca da frente da fila com o topo da pilha\n");
    printf("  5    | Trocar os 3 primeiros da fila com as 3 da pilha\n");
    printf("  0    | Sair do programa\n");
    printf("------------------------------------------------------\n");
    printf("Opcao: ");
}

// --- Funções de Ação e Manipulação ---

/**
 * @brief Executa a ação 1: Jogar uma peça (dequeue da fila).
 * @param fila Ponteiro para a estrutura da fila.
 */
void acao_jogar_peca(FilaCircular *fila) {
    Peca peca_jogada;
    // Tenta remover a peça da frente da fila
    if (dequeue(fila, &peca_jogada)) {
        printf("\nACAO: Peca [%c %d] jogada (removida da fila).\n", peca_jogada.nome, peca_jogada.id);

        // Gera uma nova peça para preencher a vaga, mantendo a fila cheia (se possível)
        Peca nova_peca = gerarPeca();
        if (enqueue(fila, nova_peca)) {
            printf("PECA AUTOMATICA: Nova peca [%c %d] inserida no final da fila.\n", nova_peca.nome, nova_peca.id);
        }
    }
}

/**
 * @brief Executa a ação 2: Reservar uma peça (dequeue da fila, push na pilha).
 * @param fila Ponteiro para a estrutura da fila.
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void acao_reservar_peca(FilaCircular *fila, Pilha *pilha) {
    Peca peca_removida;

    // 1. Tenta remover a peça da frente da fila
    if (dequeue(fila, &peca_removida)) {
        // 2. Tenta inserir a peça removida no topo da pilha
        if (push(pilha, peca_removida)) {
            printf("\nACAO: Peca [%c %d] movida da fila para a pilha de reserva.\n", peca_removida.nome, peca_removida.id);

            // 3. Gera uma nova peça para preencher a vaga na fila
            Peca nova_peca = gerarPeca();
            if (enqueue(fila, nova_peca)) {
                printf("PECA AUTOMATICA: Nova peca [%c %d] inserida no final da fila.\n", nova_peca.nome, nova_peca.id);
            }
        } else {
            // Se o push falhou (pilha cheia), a peça precisa voltar para a fila.
            // Isso complica a lógica. No cenário do Tetris, a peça simplesmente não
            // é movida e permanece na fila, e a nova peça não é gerada.
            // Para simplificar (e manter o requisito de 'manter a fila cheia' após ação),
            // assumimos que a peça que falhou em ir para a pilha 'foi jogada'
            // (ou seja, descartada) para permitir a entrada da nova peça na fila.
            // No entanto, para ser estritamente correto:
            printf("AVISO: Pilha de reserva cheia. Peca [%c %d] nao reservada e descartada para manter a fila completa.\n", peca_removida.nome, peca_removida.id);
            // Poderia-se reinserir a peça, mas o requisito diz que removidas não voltam.
            // O requisito 'a cada ação, uma nova peça é gerada' sugere que a peça
            // removida deve ser tratada como 'jogada' ou 'descartada' se a pilha falhar.
            Peca nova_peca = gerarPeca();
            if (enqueue(fila, nova_peca)) {
                printf("PECA AUTOMATICA: Nova peca [%c %d] inserida no final da fila.\n", nova_peca.nome, nova_peca.id);
            }
        }
    }
}

/**
 * @brief Executa a ação 3: Usar uma peça reservada (pop da pilha).
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void acao_usar_peca_reservada(Pilha *pilha) {
    Peca peca_usada;

    // Tenta remover a peça do topo da pilha
    if (pop(pilha, &peca_usada)) {
        printf("\nACAO: Peca [%c %d] usada (removida do topo da pilha).\n", peca_usada.nome, peca_usada.id);
    }
    // Obs: Esta ação não gera uma nova peça na fila, pois não houve remoção da fila.
}

/**
 * @brief Executa a ação 4: Trocar a peça da frente da fila com o topo da pilha.
 * @param fila Ponteiro para a estrutura da fila.
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void acao_troca_simples(FilaCircular *fila, Pilha *pilha) {
    // 1. Verifica se ambas as estruturas estão prontas para a troca
    if (fila_vazia(fila)) {
        printf("\nAVISO: Fila vazia. Nao e possivel realizar a troca.\n");
        return;
    }
    if (pilha_vazia(pilha)) {
        printf("\nAVISO: Pilha vazia. Nao e possivel realizar a troca.\n");
        return;
    }

    Peca peca_fila = frente_da_fila(fila);
    Peca peca_pilha = topo_da_pilha(pilha);

    // 2. Realiza a troca (substituição direta nos arrays)

    // A peça da pilha vai para o início da fila
    fila->itens[fila->inicio] = peca_pilha;
    // A peça da fila vai para o topo da pilha
    pilha->itens[pilha->topo] = peca_fila;

    printf("\nACAO: Troca simples realizada entre o topo da pilha ([%c %d]) e a frente da fila ([%c %d]).\n",
           peca_pilha.nome, peca_pilha.id, peca_fila.nome, peca_fila.id);
}

/**
 * @brief Executa a ação 5: Trocar as 3 primeiras peças da fila com as 3 peças da pilha.
 * @param fila Ponteiro para a estrutura da fila.
 * @param pilha Ponteiro para a estrutura da pilha.
 */
void acao_troca_multipla(FilaCircular *fila, Pilha *pilha) {
    const int num_trocas = 3;
    int i;

    // 1. Verifica se ambas as estruturas têm capacidade mínima para a troca
    if (fila->tamanho_atual < num_trocas) {
        printf("\nAVISO: Fila tem menos de %d pecas. Nao e possivel realizar a troca multipla.\n", num_trocas);
        return;
    }
    // A Pilha tem capacidade MAX_PILHA=3, então verificamos se está cheia
    if (pilha->topo + 1 < num_trocas) {
        printf("\nAVISO: Pilha tem menos de %d pecas. Nao e possivel realizar a troca multipla.\n", num_trocas);
        return;
    }

    printf("\nACAO: Iniciando a troca das %d primeiras pecas da fila com as %d pecas da pilha.\n", num_trocas, num_trocas);

    // 2. Realiza a troca
    for (i = 0; i < num_trocas; i++) {
        // Calcula o índice circular na fila: inicio + i
        int indice_fila = (fila->inicio + i) % MAX_FILA;
        // O índice da pilha é Pilha.topo - i, o que corresponde aos 3 elementos
        // Pilha.topo, Pilha.topo-1, Pilha.topo-2 (ou 2, 1, 0, se cheia)
        int indice_pilha = pilha->topo - i;

        // Armazena temporariamente a peça da fila
        Peca temp_peca_fila = fila->itens[indice_fila];

        // Troca 1: A peça da pilha vai para a fila
        fila->itens[indice_fila] = pilha->itens[indice_pilha];

        // Troca 2: A peça original da fila vai para a pilha
        pilha->itens[indice_pilha] = temp_peca_fila;
    }

    printf("Troca realizada com sucesso!\n");
}

// --- Função Principal ---

int main() {
    // Inicializa o gerador de números pseudo-aleatórios (para gerarPeca)
    srand(time(NULL));

    // Declaração das estruturas de dados
    FilaCircular fila;
    Pilha pilha;
    int opcao = -1;

    // Inicializa as estruturas
    inicializar_fila(&fila);
    inicializar_pilha(&pilha);

    // Preenche a fila com 5 peças iniciais, como requerido
    preencher_fila_inicial(&fila);

    // Loop principal do programa
    while (opcao != 0) {
        // 1. Exibe o estado atual antes de qualquer ação
        exibir_estado_atual(&fila, &pilha);

        // 2. Exibe o menu e solicita a opção
        exibir_menu();
        // Leitura da opção do usuário
        // O resultado do scanf é o número de itens lidos com sucesso
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Entrada invalida. Por favor, digite um numero.\n");
            // Limpa o buffer de entrada para evitar loops infinitos de erro
            limpar_buffer();
            opcao = -1; // Reinicia a opção para garantir que o loop continue
            continue;
        }

        // Limpa o buffer após a leitura do inteiro
        limpar_buffer();

        // 3. Processa a opção escolhida
        switch (opcao) {
            case 1:
                acao_jogar_peca(&fila); // Dequeue e gera nova peça
                break;
            case 2:
                acao_reservar_peca(&fila, &pilha); // Dequeue, Push, e gera nova peça
                break;
            case 3:
                acao_usar_peca_reservada(&pilha); // Pop da pilha
                break;
            case 4:
                acao_troca_simples(&fila, &pilha); // Troca frente da fila com topo da pilha
                break;
            case 5:
                acao_troca_multipla(&fila, &pilha); // Troca 3 da fila com 3 da pilha
                break;
            case 0:
                printf("\nEncerrando o Gerenciador de Pecas. Ate logo!\n");
                break;
            default:
                printf("\nAVISO: Opcao invalida. Por favor, escolha um numero entre 0 e 5.\n");
                break;
        }
    }

    return 0; // Finaliza o programa
}