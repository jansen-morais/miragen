#include <stdio.h>   // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrao (malloc, calloc, free, rand, srand) para alocacao dinamica e numeros aleatorios.
#include <string.h>  // Inclui a biblioteca para manipulacao de strings (strcspn, strcpy, strcmp).
#include <unistd.h>  // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>    // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().
#include <ctype.h>   // Para a função tolower

// Definição do tamanho máximo para strings
#define MAX_STR 100
// Tamanho da tabela hash de suspeitos
#define HASH_SIZE 10

// --- Estruturas de Dados ---

// 1. Estrutura para a Tabela Hash (Associação Pista -> Suspeito)
// Cada nó da lista ligada na tabela hash (para lidar com colisões)
typedef struct HashNode {
    char pista[MAX_STR];       // A chave (Pista)
    char suspeito[MAX_STR];    // O valor (Suspeito associado)
    struct HashNode *proximo;  // Ponteiro para o próximo nó em caso de colisão
} HashNode;

// 2. Estrutura para os NÓS da ÁRVORE DE PISTAS (BST)
// Armazena as pistas coletadas em ordem alfabética
typedef struct PistaNode {
    char pista[MAX_STR];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// 3. Estrutura para os CÔMODOS da MANSÃO (Árvore Binária)
// Mapa da mansão
typedef struct Sala {
    char nome[MAX_STR];
    char pista_estatica[MAX_STR]; // Pista que a sala "contém" (fixa)
    struct Sala *esquerda;        // Caminho para a sala à esquerda
    struct Sala *direita;         // Caminho para a sala à direita
} Sala;

// --- Protótipos das Funções ---

// Funções Auxiliares
void limparBuffer();

// Funções da Árvore de Salas (Mapa)
Sala* criarSala(const char *nome, const char *pista);
void liberarMapa(Sala *raiz);

// Funções da Tabela Hash (Pista -> Suspeito)
int hash(const char *chave);
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito);
const char* encontrarSuspeito(HashNode *tabela[], const char *pista);
void liberarHash(HashNode *tabela[]);

// Funções da Árvore de Pistas Coletadas (BST)
PistaNode* criarPistaNode(const char *pista);
PistaNode* inserirPista(PistaNode *raiz, const char *pista);
void exibirPistas(PistaNode *raiz);
void liberarPistas(PistaNode *raiz);

// Funções de Exploração e Julgamento
PistaNode* explorarSalas(Sala *raiz, PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]);
void contarProvas(PistaNode *raiz, const char *acusacao, int *contador, HashNode *tabela_suspeitos[]); // FUNÇÃO CORRIGIDA (GLOBAL)
void verificarSuspeitoFinal(PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]);

// --- Implementação das Funções Auxiliares ---

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar que caracteres indesejados
 * (como o '\n' gerado pelo Enter) interfiram em leituras subsequentes.
 */
void limparBuffer() {
    int c;
    // Lê e descarta todos os caracteres até encontrar um '\n' ou o fim do arquivo (EOF)
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Implementação das Funções da Árvore de Salas (Mapa) ---

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) para o mapa.
 *
 * @param nome O nome da sala.
 * @param pista O conteúdo da pista estática nesta sala.
 * @return Ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    // Aloca memória para a nova sala usando malloc
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));

    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        perror("Erro ao alocar memoria para Sala");
        exit(EXIT_FAILURE); // Encerra o programa em caso de falha grave
    }

    // Copia o nome e a pista para a estrutura
    strncpy(novaSala->nome, nome, MAX_STR - 1);
    novaSala->nome[MAX_STR - 1] = '\0'; // Garante terminação nula

    strncpy(novaSala->pista_estatica, pista, MAX_STR - 1);
    novaSala->pista_estatica[MAX_STR - 1] = '\0'; // Garante terminação nula

    // Inicializa os ponteiros dos caminhos como nulos
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Libera recursivamente a memória alocada para o mapa da mansão (Árvore de Salas).
 * @param raiz O nó raiz da subárvore a ser liberada.
 */
void liberarMapa(Sala *raiz) {
    // Caso base da recursão: se o nó for nulo, retorna
    if (raiz == NULL) {
        return;
    }

    // Chama recursivamente para a subárvore esquerda
    liberarMapa(raiz->esquerda);
    // Chama recursivamente para a subárvore direita
    liberarMapa(raiz->direita);

    // Libera a memória do nó atual (pós-ordem)
    free(raiz);
}

// --- Implementação das Funções da Tabela Hash (Pista -> Suspeito) ---

/**
 * @brief Função de hash simples que gera um índice para a tabela a partir da primeira letra.
 *
 * @param chave A string (pista) a ser hasheada.
 * @return Um índice válido dentro do tamanho da tabela (0 a HASH_SIZE - 1).
 */
int hash(const char *chave) {
    // Usa o valor ASCII da primeira letra (em minúsculo)
    if (chave == NULL || chave[0] == '\0') {
        return 0;
    }
    // Garante um valor positivo e dentro do limite
    return (tolower(chave[0]) - 'a') % HASH_SIZE;
}

/**
 * @brief Insere uma nova associação pista-suspeito na tabela hash.
 *
 * @param tabela O array de ponteiros da tabela hash.
 * @param pista A chave (nome da pista).
 * @param suspeito O valor (nome do suspeito).
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    // Calcula o índice (bucket) na tabela
    int index = hash(pista);

    // Aloca memória para o novo nó da hash
    HashNode *novoNo = (HashNode *)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para HashNode");
        exit(EXIT_FAILURE);
    }

    // Copia os dados
    strncpy(novoNo->pista, pista, MAX_STR - 1);
    novoNo->pista[MAX_STR - 1] = '\0';
    
    strncpy(novoNo->suspeito, suspeito, MAX_STR - 1);
    novoNo->suspeito[MAX_STR - 1] = '\0';

    // Insere o novo nó no início da lista ligada no índice calculado (encadeamento)
    novoNo->proximo = tabela[index];
    tabela[index] = novoNo;
}

/**
 * @brief Busca o suspeito associado a uma pista na tabela hash.
 *
 * @param tabela O array de ponteiros da tabela hash.
 * @param pista A chave (nome da pista) a ser buscada.
 * @return O nome do suspeito (string constante) ou NULL se a pista não for encontrada.
 */
const char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    // Calcula o índice (bucket)
    int index = hash(pista);
    
    // Percorre a lista ligada (encadeamento) neste índice
    HashNode *atual = tabela[index];
    while (atual != NULL) {
        // Compara a pista do nó atual com a pista buscada
        if (strcmp(atual->pista, pista) == 0) {
            // Pista encontrada, retorna o suspeito
            return atual->suspeito;
        }
        atual = atual->proximo; // Avança para o próximo nó da lista
    }

    // Se o loop terminar, a pista não foi encontrada
    return NULL;
}

/**
 * @brief Libera toda a memória alocada para a tabela hash.
 * @param tabela O array de ponteiros da tabela hash.
 */
void liberarHash(HashNode *tabela[]) {
    // Itera sobre todos os índices da tabela
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *atual = tabela[i];
        // Percorre a lista ligada em cada índice e libera os nós
        while (atual != NULL) {
            HashNode *temp = atual; // Guarda o ponteiro atual para liberar
            atual = atual->proximo; // Avança antes de liberar
            free(temp);             // Libera a memória do nó
        }
        tabela[i] = NULL; // Garante que o ponteiro no array seja nulo
    }
}

// --- Implementação das Funções da Árvore de Pistas Coletadas (BST) ---

/**
 * @brief Cria dinamicamente um novo nó para armazenar uma pista.
 *
 * @param pista O conteúdo da pista.
 * @return Ponteiro para o novo PistaNode criado.
 */
PistaNode* criarPistaNode(const char *pista) {
    // Aloca memória para o novo nó de pista
    PistaNode *novoNo = (PistaNode *)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para PistaNode");
        exit(EXIT_FAILURE);
    }

    // Copia a pista para a estrutura
    strncpy(novoNo->pista, pista, MAX_STR - 1);
    novoNo->pista[MAX_STR - 1] = '\0';

    // Inicializa os ponteiros dos filhos como nulos
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;

    return novoNo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma recursiva.
 * Garante que as pistas sejam armazenadas em ordem alfabética (chave é o nome da pista).
 *
 * @param raiz O nó raiz da subárvore atual.
 * @param pista O conteúdo da pista a ser inserida.
 * @return O nó raiz da subárvore atualizada.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    // Caso base da recursão: Se a subárvore for nula, cria o novo nó e o retorna
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    // Compara a nova pista com a pista do nó atual
    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        // A nova pista é 'menor', insere na esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // A nova pista é 'maior', insere na direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe, não insere duplicatas

    // Retorna o ponteiro raiz (imutável)
    return raiz;
}

/**
 * @brief Exibe todas as pistas armazenadas na BST em ordem alfabética (percurso In-ordem).
 * @param raiz O nó raiz da BST.
 */
void exibirPistas(PistaNode *raiz) {
    // Caso base da recursão
    if (raiz != NULL) {
        // 1. Visita a subárvore esquerda
        exibirPistas(raiz->esquerda);
        
        // 2. Processa o nó atual (imprime a pista)
        printf("  - %s\n", raiz->pista);
        
        // 3. Visita a subárvore direita
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Libera recursivamente a memória alocada para a BST de pistas.
 * @param raiz O nó raiz da BST a ser liberada.
 */
void liberarPistas(PistaNode *raiz) {
    // A liberação é feita em pós-ordem
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// --- Implementação das Funções de Exploração e Julgamento ---

/**
 * @brief Permite a navegação interativa do jogador pela árvore de salas e coleta de pistas.
 *
 * @param raiz O nó atual (inicialmente, o Hall de Entrada).
 * @param pistas_coletadas A raiz da BST de pistas coletadas.
 * @param tabela_suspeitos A tabela hash para identificar o suspeito da pista.
 * @return A raiz atualizada da BST de pistas coletadas.
 */
PistaNode* explorarSalas(Sala *raiz, PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]) {
    Sala *atual = raiz; // Começa no Hall
    char escolha;       // Opção do jogador

    printf("\n--- INICIO DA EXPLORACAO DA MANSAO ---\n");
    printf("Pressione 's' para sair a qualquer momento e iniciar o julgamento.\n");

    // Loop principal de exploração
    while (atual != NULL) {
        printf("\nVoce esta em: **%s**\n", atual->nome);

        // Verifica se há pista na sala
        if (atual->pista_estatica[0] != '\0') {
            const char* suspeito_da_pista = encontrarSuspeito(tabela_suspeitos, atual->pista_estatica);
            
            sleep(1.5); // Da uma breve pausa no programa
            printf("----------------------------------------\n");
            printf("!! PISTA ENCONTRADA !!\n");
            printf("Detalhe: \"%s\"\n", atual->pista_estatica);
            
            // Adiciona a pista à BST
            pistas_coletadas = inserirPista(pistas_coletadas, atual->pista_estatica);
            
            // Exibe a associação com o suspeito (se encontrado na hash)
            if (suspeito_da_pista != NULL) {
                printf("Indicio aponta para: **%s**\n", suspeito_da_pista);
            }
            sleep(1.5); // Da uma breve pausa no programa
            printf("Pista adicionada ao seu caderno de notas.\n");
            // Limpa a pista estática para que não seja coletada novamente
            atual->pista_estatica[0] = '\0'; 
            printf("----------------------------------------\n");
        }

        // Verifica se é um nó-folha (fim do caminho)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nEste caminho termina aqui. Nao ha mais saidas. Pressione 's' para sair.\n");
        }

        sleep(1.5); // Da uma breve pausa no programa
        printf("\nO que deseja fazer? [e] Esquerda, [d] Direita, [s] Sair: ");
        
        // Leitura da escolha do jogador
        if (scanf(" %c", &escolha) != 1) {
            limparBuffer(); // Limpa se a leitura falhar
            continue;
        }
        
        // Limpa o buffer após a leitura de um caractere
        limparBuffer(); 
        
        escolha = tolower(escolha); // Padroniza para minúsculo

        // Controle da navegação
        if (escolha == 's') {
            sleep(1.5); // Da uma breve pausa no programa
            printf("\n--- FIM DA EXPLORACAO ---\n");
            break; // Sai do loop
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a esquerda
            } else {
                sleep(1.5); // Da uma breve pausa no programa
                printf("Caminho a esquerda bloqueado.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a direita
            } else {
                printf("Caminho a direita bloqueado.\n");
            }
        } else {
            sleep(1.5); // Da uma breve pausa no programa
            printf("Opcao invalida. Por favor, escolha 'e', 'd' ou 's'.\n");
        }
    }
    
    // Retorna a raiz da BST de pistas (pode ter sido modificada)
    return pistas_coletadas;
}

/**
 * @brief Percorre a BST de pistas coletadas e verifica quantas apontam para o suspeito acusado.
 * (Utiliza a BST para iterar (In-order) e a Hash para consultar o suspeito).
 *
 * @param raiz O nó raiz da BST.
 * @param acusacao O nome do suspeito sendo acusado.
 * @param contador Ponteiro para a variável onde o total de provas será armazenado.
 * @param tabela_suspeitos A tabela hash para consultar a associação pista/suspeito.
 */
void contarProvas(PistaNode *raiz, const char *acusacao, int *contador, HashNode *tabela_suspeitos[]) {
    // Caso base: se o nó é nulo, retorna
    if (raiz == NULL) return;
    
    // 1. Visita a subárvore esquerda
    contarProvas(raiz->esquerda, acusacao, contador, tabela_suspeitos);
    
    // 2. Processa o nó atual: verifica se a pista aponta para o acusado
    const char* suspeito_pista = encontrarSuspeito(tabela_suspeitos, raiz->pista);
    
    // Verifica se a pista aponta para alguém E se essa pessoa é o acusado
    if (suspeito_pista != NULL && strcmp(suspeito_pista, acusacao) == 0) {
        (*contador)++; // Incrementa o valor do contador (via ponteiro)
    }
    
    // 3. Visita a subárvore direita
    contarProvas(raiz->direita, acusacao, contador, tabela_suspeitos);
}

/**
 * @brief Conduz a fase de julgamento final, solicitando o suspeito e verificando as provas.
 *
 * @param pistas_coletadas A raiz da BST de pistas coletadas.
 * @param tabela_suspeitos A tabela hash para consultar a associação pista/suspeito.
 */
void verificarSuspeitoFinal(PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]) {
    char acusacao[MAX_STR];
    int contagem_provas = 0;
    
    sleep(1.5); // Da uma breve pausa no programa
    printf("\n========================================\n");
    printf("         FASE DE JULGAMENTO FINAL       \n");
    printf("========================================\n");
    sleep(1.5); // Da uma breve pausa no programa

    // 1. Lista as pistas coletadas
    if (pistas_coletadas == NULL) {
        sleep(1.5); // Da uma breve pausa no programa
        printf("Voce nao coletou nenhuma pista. O caso e inconclusivo.\n");
        return;
    }

    sleep(1.5); // Da uma breve pausa no programa
    printf("\n--- Pistas Coletadas (em ordem alfabetica) ---\n");
    exibirPistas(pistas_coletadas);
    printf("----------------------------------------------\n");

    sleep(1.5); // Da uma breve pausa no programa
    // 2. Solicita a acusação
    printf("\nCom base nas suas descobertas, quem voce acusa? (Nome do Suspeito): ");
    
    // Lê a linha completa (fgets é mais seguro para ler nomes com espaços)
    if (fgets(acusacao, MAX_STR, stdin) == NULL) {
        fprintf(stderr, "Erro na leitura da acusacao.\n");
        return;
    }
    // Remove o '\n' que o fgets deixa no final da string
    acusacao[strcspn(acusacao, "\n")] = '\0';
    
    // 3. Verificação das provas (usando a função global contarProvas)
    
    // Chama a função recursiva para contar quantas pistas apontam para o acusado
    contarProvas(pistas_coletadas, acusacao, &contagem_provas, tabela_suspeitos);

    sleep(1.5); // Da uma breve pausa no programa
    // 4. Exibe o veredito
    printf("\n=> O Detetive acusa: **%s**\n", acusacao);
    printf("=> Pistas encontradas contra o acusado: **%d**\n", contagem_provas);

    // Requisito: Pelo menos duas pistas
    if (contagem_provas >= 2) {
        sleep(2.5); // Da uma breve pausa no programa
        printf("\n[ V E R E D I T O: C U L P A D O ]\n");
        sleep(1.5); // Da uma breve pausa no programa
        printf("A acusacao e sustentada por %d provas solidas. A justica prevaleceu!\n", contagem_provas);
    } else {
        sleep(2.5); // Da uma breve pausa no programa
        printf("\n[ V E R E D I T O: I N C O N C L U S I V O ]\n");
        sleep(1.5); // Da uma breve pausa no programa
        printf("As %d provas coletadas sao insuficientes para uma condenacao.\n", contagem_provas);
        sleep(1.5); // Da uma breve pausa no programa
        printf("Sao necessarias pelo menos duas provas para sustentar a acusacao.\n");
    }
}

// --- Função Principal (main) ---

/**
 * @brief Monta o mapa da mansão, inicializa a tabela hash e inicia a exploração.
 */
int main() {
    // 1. Inicialização das Estruturas
    
    // Inicializa a Tabela Hash com todos os ponteiros nulos
    HashNode *tabela_suspeitos[HASH_SIZE] = { NULL };
    
    // Raiz da BST de pistas coletadas (inicia vazia)
    PistaNode *pistas_coletadas = NULL;
    
    // 2. Montagem do Mapa da Mansão (Árvore Binária)
    
    // Criação dos nós (Sala) com nome e pista estática
    // Raiz (Hall de Entrada)
    Sala *hall = criarSala("Hall de Entrada", "Mancha de oleo no chao");

    // Nível 1
    Sala *sala_estar = criarSala("Sala de Estar", "Carta de amor rasgada");
    Sala *cozinha = criarSala("Cozinha", "Faca de prata faltando");
    
    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca", "Livreto de viagem para Paris");
    Sala *jardim = criarSala("Jardim", ""); // Sem pista
    Sala *dispensa = criarSala("Dispensa", "Pó branco misterioso");
    Sala *escritorio = criarSala("Escritorio", "Recibo de compra de veneno");
    
    // Conexões
    hall->esquerda = sala_estar;
    hall->direita = cozinha;
    
    sala_estar->esquerda = biblioteca;
    sala_estar->direita = jardim;
    
    cozinha->esquerda = dispensa;
    cozinha->direita = escritorio;

    // Nível 3 (Nós-folha, fim dos caminhos)
    biblioteca->esquerda = criarSala("Quarto Principal", "Relogio de pulso quebrado");
    biblioteca->direita = criarSala("Banheiro", "");
    
    dispensa->esquerda = criarSala("Porao", "Chave dourada antiga");
    
    // 3. Montagem da Tabela Hash (Associação Pista -> Suspeito)
    // Pistas e seus Suspeitos (Suspeitos: Charles, Diana, William, Kate)
    
    inserirNaHash(tabela_suspeitos, "Mancha de oleo no chao", "Charles");
    inserirNaHash(tabela_suspeitos, "Carta de amor rasgada", "Diana");
    inserirNaHash(tabela_suspeitos, "Recibo de compra de veneno", "Charles"); // Charles tem 2
    inserirNaHash(tabela_suspeitos, "Pó branco misterioso", "William");
    inserirNaHash(tabela_suspeitos, "Livreto de viagem para Paris", "Diana");  // Diana tem 2
    inserirNaHash(tabela_suspeitos, "Faca de prata faltando", "Kate");
    inserirNaHash(tabela_suspeitos, "Relogio de pulso quebrado", "Kate");      // Kate tem 2
    inserirNaHash(tabela_suspeitos, "Chave dourada antiga", "William");
    
    sleep(1.5); // Da uma breve pausa no programa
    
    // Mensagem de início
    printf("========================================\n");
    printf("     ##### ENIGMA STUDIOS ##### \n");
    printf("========================================\n");
    sleep(2); // Da uma breve pausa no programa

    printf("========================================\n");
    printf("MYSTERY OF THE MANSION - DETECTIVE QUEST \n");
    printf("========================================\n");
    sleep(1.5); // Da uma breve pausa no programa
    
    // 4. Início da Exploração
    // A função de exploração retorna a raiz da BST de pistas atualizada
    pistas_coletadas = explorarSalas(hall, pistas_coletadas, tabela_suspeitos);

    // 5. Julgamento Final
    verificarSuspeitoFinal(pistas_coletadas, tabela_suspeitos);
    
    // 6. Limpeza de Memória (essencial para evitar memory leaks)
    printf("\n--- FIM DO PROGRAMA ---\n");
    liberarMapa(hall);
    liberarPistas(pistas_coletadas);
    liberarHash(tabela_suspeitos);
    printf("Memoria alocada liberada com sucesso.\n");

    return 0; // Indica que o programa terminou com sucesso
}