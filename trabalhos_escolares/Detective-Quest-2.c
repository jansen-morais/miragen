#include <stdio.h>    // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <stdlib.h>   // Inclui a biblioteca padrao (malloc, calloc, free, rand, srand) para alocacao dinamica e numeros aleatorios.
#include <string.h>   // Inclui a biblioteca para manipulacao de strings (strcspn, strcpy, strcmp).
#include <unistd.h>   // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>     // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().
#include <ctype.h>    // Para a função tolower

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
void inicializarHash(HashNode *tabela[]); // NOVO: Para resetar a hash
Sala* montarMapa();                      // NOVO: Para criar o mapa em cada partida
void popularHash(HashNode *tabela[]);    // NOVO: Para popular a hash em cada partida

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
void contarProvas(PistaNode *raiz, const char *acusacao, int *contador, HashNode *tabela_suspeitos[]);
void verificarSuspeitoFinal(PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]);

// --- Implementação das Funções Auxiliares (Mapa e Hash) ---

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar interferências em leituras subsequentes.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Lê e descarta caracteres até o '\n'
}

/**
 * @brief Inicializa a tabela Hash, definindo todos os ponteiros como NULL.
 * @param tabela O array de ponteiros da tabela hash.
 */
void inicializarHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        tabela[i] = NULL;
    }
}

/**
 * @brief Monta o mapa da mansão (Árvore Binária) com todas as salas e pistas.
 *
 * @return O ponteiro para o Hall de Entrada (raiz da árvore).
 */
Sala* montarMapa() {
    // Criação dos nós (Sala) com nome e pista estática
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

    return hall; // Retorna a raiz
}

/**
 * @brief Popula a Tabela Hash com as associações Pista -> Suspeito.
 * @param tabela O array de ponteiros da tabela hash.
 */
void popularHash(HashNode *tabela[]) {
    // Pistas e seus Suspeitos (Suspeitos: Charles, Diana, William, Kate)
    // Nota: Charles, Diana e Kate têm 2 pistas cada, satisfazendo o critério de julgamento.
    inserirNaHash(tabela, "Mancha de oleo no chao", "Charles");
    inserirNaHash(tabela, "Carta de amor rasgada", "Diana");
    inserirNaHash(tabela, "Recibo de compra de veneno", "Charles");
    inserirNaHash(tabela, "Pó branco misterioso", "William");
    inserirNaHash(tabela, "Livreto de viagem para Paris", "Diana");
    inserirNaHash(tabela, "Faca de prata faltando", "Kate");
    inserirNaHash(tabela, "Relogio de pulso quebrado", "Kate");
    inserirNaHash(tabela, "Chave dourada antiga", "William");
}

// --- Implementação das Funções da Árvore de Salas (Mapa) ---

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) para o mapa.
 * (A alocação dinâmica é essencial para a estrutura de árvore.)
 *
 * @param nome O nome da sala.
 * @param pista O conteúdo da pista estática nesta sala.
 * @return Ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala *)malloc(sizeof(Sala)); // Aloca memória

    if (novaSala == NULL) {
        perror("Erro ao alocar memoria para Sala");
        exit(EXIT_FAILURE); // Encerra o programa em caso de falha grave
    }

    // Inicializa os campos da Sala
    strncpy(novaSala->nome, nome, MAX_STR - 1);
    novaSala->nome[MAX_STR - 1] = '\0';

    strncpy(novaSala->pista_estatica, pista, MAX_STR - 1);
    novaSala->pista_estatica[MAX_STR - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Libera recursivamente a memória alocada para o mapa da mansão (Árvore de Salas).
 * (Libera a memória em ordem pós-ordem para evitar ponteiros pendurados.)
 * @param raiz O nó raiz da subárvore a ser liberada.
 */
void liberarMapa(Sala *raiz) {
    if (raiz == NULL) return;

    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);

    free(raiz); // Libera o nó atual
}

// --- Implementação das Funções da Tabela Hash (Pista -> Suspeito) ---

/**
 * @brief Função de hash simples: usa a primeira letra da chave para o índice.
 *
 * @param chave A string (pista) a ser hasheada.
 * @return Um índice válido dentro do tamanho da tabela.
 */
int hash(const char *chave) {
    if (chave == NULL || chave[0] == '\0') {
        return 0;
    }
    // Garante que a primeira letra seja minúscula para o cálculo
    return (tolower(chave[0]) - 'a') % HASH_SIZE;
}

/**
 * @brief Insere uma nova associação pista-suspeito na tabela hash.
 * (Usa encadeamento para resolver colisões.)
 *
 * @param tabela O array de ponteiros da tabela hash.
 * @param pista A chave (nome da pista).
 * @param suspeito O valor (nome do suspeito).
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int index = hash(pista);

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

    // Insere o novo nó no início da lista ligada (encadeamento)
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
    int index = hash(pista);
    
    HashNode *atual = tabela[index];
    // Percorre a lista ligada no índice
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Pista encontrada
        }
        atual = atual->proximo;
    }

    return NULL; // Pista não encontrada
}

/**
 * @brief Libera toda a memória alocada para a tabela hash.
 * @param tabela O array de ponteiros da tabela hash.
 */
void liberarHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *atual = tabela[i];
        // Percorre e libera a lista ligada em cada índice
        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabela[i] = NULL;
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
    PistaNode *novoNo = (PistaNode *)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para PistaNode");
        exit(EXIT_FAILURE);
    }

    // Inicializa os campos
    strncpy(novoNo->pista, pista, MAX_STR - 1);
    novoNo->pista[MAX_STR - 1] = '\0';

    novoNo->esquerda = NULL;
    novoNo->direita = NULL;

    return novoNo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma recursiva.
 * (A BST garante a ordem alfabética das pistas.)
 *
 * @param raiz O nó raiz da subárvore atual.
 * @param pista O conteúdo da pista a ser inserida.
 * @return O nó raiz da subárvore atualizada.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Ignora se for igual (não insere duplicatas)

    return raiz;
}

/**
 * @brief Exibe todas as pistas armazenadas na BST em ordem alfabética (percurso In-ordem).
 * @param raiz O nó raiz da BST.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista); // Processa (imprime) o nó
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Libera recursivamente a memória alocada para a BST de pistas.
 * @param raiz O nó raiz da BST a ser liberada.
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
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
    Sala *atual = raiz;
    char escolha;

    printf("\n--- INICIO DA EXPLORACAO DA MANSAO ---\n");
    printf("Pressione 's' para sair a qualquer momento e iniciar o julgamento.\n");

    while (atual != NULL) {
        printf("\nVoce esta em: **%s**\n", atual->nome);

        if (atual->pista_estatica[0] != '\0') {
            const char* suspeito_da_pista = encontrarSuspeito(tabela_suspeitos, atual->pista_estatica);
            
            sleep(1); 
            printf("----------------------------------------\n");
            printf("!! PISTA ENCONTRADA !!\n");
            printf("Detalhe: \"%s\"\n", atual->pista_estatica);
            
            pistas_coletadas = inserirPista(pistas_coletadas, atual->pista_estatica); // Adiciona à BST
            
            if (suspeito_da_pista != NULL) {
                printf("Indicio aponta para: **%s**\n", suspeito_da_pista);
            }
            sleep(1);
            printf("Pista adicionada ao seu caderno de notas.\n");
            atual->pista_estatica[0] = '\0'; // Limpa a pista da sala
            printf("----------------------------------------\n");
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nEste caminho termina aqui. Nao ha mais saidas. Pressione 's' para sair.\n");
        }

        sleep(1);
        printf("\nO que deseja fazer? [e] Esquerda, [d] Direita, [s] Sair: ");
        
        if (scanf(" %c", &escolha) != 1) {
            limparBuffer(); 
            continue;
        }
        
        limparBuffer(); 
        escolha = tolower(escolha);

        if (escolha == 's') {
            sleep(1);
            printf("\n--- FIM DA EXPLORACAO ---\n");
            break;
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                sleep(1);
                printf("Caminho a esquerda bloqueado.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                sleep(1);
                printf("Caminho a direita bloqueado.\n");
            }
        } else {
            sleep(1);
            printf("Opcao invalida. Por favor, escolha 'e', 'd' ou 's'.\n");
        }
    }
    
    return pistas_coletadas;
}

/**
 * @brief Percorre a BST e conta quantas pistas coletadas apontam para o acusado.
 *
 * @param raiz O nó raiz da BST.
 * @param acusacao O nome do suspeito sendo acusado.
 * @param contador Ponteiro para a variável onde o total de provas será armazenado.
 * @param tabela_suspeitos A tabela hash para consultar o suspeito da pista.
 */
void contarProvas(PistaNode *raiz, const char *acusacao, int *contador, HashNode *tabela_suspeitos[]) {
    if (raiz == NULL) return;
    
    contarProvas(raiz->esquerda, acusacao, contador, tabela_suspeitos);
    
    const char* suspeito_pista = encontrarSuspeito(tabela_suspeitos, raiz->pista);
    
    // Compara a pista coletada com o acusado
    if (suspeito_pista != NULL && strcmp(suspeito_pista, acusacao) == 0) {
        (*contador)++;
    }
    
    contarProvas(raiz->direita, acusacao, contador, tabela_suspeitos);
}

/**
 * @brief Conduz a fase de julgamento final e verifica as provas.
 *
 * @param pistas_coletadas A raiz da BST de pistas coletadas.
 * @param tabela_suspeitos A tabela hash para consultar a associação pista/suspeito.
 */
void verificarSuspeitoFinal(PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]) {
    char acusacao[MAX_STR];
    int contagem_provas = 0;
    
    sleep(1);
    printf("\n========================================\n");
    printf("         FASE DE JULGAMENTO FINAL       \n");
    printf("========================================\n");
    sleep(1);

    if (pistas_coletadas == NULL) {
        sleep(1);
        printf("Voce nao coletou nenhuma pista. O caso e inconclusivo.\n");
        return;
    }

    sleep(1);
    printf("\n--- Pistas Coletadas (em ordem alfabetica) ---\n");
    exibirPistas(pistas_coletadas);
    printf("----------------------------------------------\n");

    sleep(1);
    printf("\nCom base nas suas descobertas, quem voce acusa? (Nome do Suspeito): ");
    
    if (fgets(acusacao, MAX_STR, stdin) == NULL) {
        fprintf(stderr, "Erro na leitura da acusacao.\n");
        return;
    }
    acusacao[strcspn(acusacao, "\n")] = '\0'; // Remove o '\n'

    contarProvas(pistas_coletadas, acusacao, &contagem_provas, tabela_suspeitos); // Conta as provas

    sleep(1.5);
    printf("\n=> O Detetive acusa: **%s**\n", acusacao);
    printf("=> Pistas encontradas contra o acusado: **%d**\n", contagem_provas);

    if (contagem_provas >= 2) { // Critério de vitória: 2 ou mais provas
        sleep(2);
        printf("\n[ V E R E D I T O: C U L P A D O ]\n");
        sleep(1);
        printf("A acusacao e sustentada por %d provas solidas. A justica prevaleceu!\n", contagem_provas);
    } else {
        sleep(2);
        printf("\n[ V E R E D I T O: I N C O N C L U S I V O ]\n");
        sleep(1);
        printf("As %d provas coletadas sao insuficientes para uma condenacao.\n", contagem_provas);
        sleep(1);
        printf("Sao necessarias pelo menos duas provas para sustentar a acusacao.\n");
    }
}

// --- Função Principal (main) com Loop de Repetição ---

/**
 * @brief Inicializa o programa, contém o loop principal de repetição e gerencia a alocação de memória.
 */
int main() {
    char jogar_novamente; // Variável para controlar o loop

    // Mensagem de início (fora do loop, só aparece uma vez)
    printf("========================================\n");
    printf("      ##### ENIGMA STUDIOS ##### \n");
    printf("========================================\n");
    sleep(2);

    do {
        // --- 1. CONFIGURAÇÃO DO NOVO JOGO ---
        
        // A Tabela Hash e a BST de Pistas devem ser re-inicializadas em cada jogo!
        
        // a) Inicializa a Tabela Hash com todos os ponteiros nulos
        HashNode *tabela_suspeitos[HASH_SIZE];
        inicializarHash(tabela_suspeitos);
        
        // b) Monta o Mapa da Mansão (Árvore Binária)
        Sala *hall = montarMapa();
        
        // c) Popula a Tabela Hash com as associações Pista -> Suspeito
        popularHash(tabela_suspeitos);
        
        // d) Raiz da BST de pistas coletadas (inicia vazia)
        PistaNode *pistas_coletadas = NULL;
        
        sleep(1);
        printf("\n========================================\n");
        printf("MYSTERY OF THE MANSION - DETECTIVE QUEST \n");
        printf("========================================\n");
        sleep(1.5);
        
        // --- 2. JOGO ---
        
        // Início da Exploração. Retorna a raiz da BST de pistas atualizada.
        pistas_coletadas = explorarSalas(hall, pistas_coletadas, tabela_suspeitos);

        // Julgamento Final
        verificarSuspeitoFinal(pistas_coletadas, tabela_suspeitos);
        
        // --- 3. LIMPEZA E PERGUNTA DE REPETIÇÃO ---
        
        // Libera TODA a memória alocada para o jogo atual
        printf("\n--- FIM DA PARTIDA ---\n");
        liberarMapa(hall);
        liberarPistas(pistas_coletadas);
        liberarHash(tabela_suspeitos);
        printf("Memoria do jogo liberada com sucesso.\n");
        
        // Pergunta se o jogador quer jogar de novo
        printf("\nDeseja iniciar uma nova investigacao? [s] Sim / [n] Nao: ");
        if (scanf(" %c", &jogar_novamente) != 1) {
            jogar_novamente = 'n'; // Assume 'não' em caso de erro de leitura
        }
        limparBuffer(); // Limpa o buffer após a leitura

        jogar_novamente = tolower(jogar_novamente);

    } while (jogar_novamente == 's'); // O loop continua enquanto a escolha for 's'

    printf("\n========================================\n");
    printf("  Obrigado por jogar! Voltaremos em breve.\n");
    printf("========================================\n");

    return 0;
}