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

// Funções Auxiliares de Gerenciamento do Jogo
void limparBuffer();
void inicializarHash(HashNode *tabela[]); 
Sala* montarMapa();                      
void popularHash(HashNode *tabela[]);    

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

// --- Implementação das Funções Auxiliares de Gerenciamento do Jogo ---

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar interferências em leituras subsequentes.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Lê e descarta caracteres até o '\n'
}

/**
 * @brief Inicializa a tabela Hash, definindo todos os ponteiros como NULL.
 * (Crucial para garantir um estado limpo antes de popular a tabela em um novo jogo).
 * @param tabela O array de ponteiros da tabela hash.
 */
void inicializarHash(HashNode *tabela[]) {
    // Itera sobre todos os índices e define o ponteiro inicial como nulo
    for (int i = 0; i < HASH_SIZE; i++) {
        tabela[i] = NULL;
    }
}

/**
 * @brief Monta o mapa da mansão (Árvore Binária) com todas as salas e pistas.
 * (Esta função é chamada a cada novo jogo para recriar o mapa e as pistas estáticas.)
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
    Sala *jardim = criarSala("Jardim", ""); // Sem pista neste nó
    Sala *dispensa = criarSala("Dispensa", "Pó branco misterioso");
    Sala *escritorio = criarSala("Escritorio", "Recibo de compra de veneno");
    
    // Conexões (Estrutura de Árvore Binária)
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

    return hall; // Retorna a raiz da árvore
}

/**
 * @brief Popula a Tabela Hash com as associações Pista -> Suspeito.
 * (Esta função é chamada a cada novo jogo para restaurar as associações.)
 * @param tabela O array de ponteiros da tabela hash.
 */
void popularHash(HashNode *tabela[]) {
    // Insere todas as associações pista-suspeito na Tabela Hash
    inserirNaHash(tabela, "Mancha de oleo no chao", "Charles");
    inserirNaHash(tabela, "Carta de amor rasgada", "Diana");
    inserirNaHash(tabela, "Recibo de compra de veneno", "Charles"); // Charles tem 2 provas
    inserirNaHash(tabela, "Pó branco misterioso", "William");
    inserirNaHash(tabela, "Livreto de viagem para Paris", "Diana");  // Diana tem 2 provas
    inserirNaHash(tabela, "Faca de prata faltando", "Kate");
    inserirNaHash(tabela, "Relogio de pulso quebrado", "Kate");      // Kate tem 2 provas
    inserirNaHash(tabela, "Chave dourada antiga", "William");
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
    Sala *novaSala = (Sala *)malloc(sizeof(Sala)); // Aloca memória

    if (novaSala == NULL) {
        perror("Erro ao alocar memoria para Sala");
        exit(EXIT_FAILURE);
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
 * @param raiz O nó raiz da subárvore a ser liberada.
 */
void liberarMapa(Sala *raiz) {
    if (raiz == NULL) return;

    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);

    free(raiz); // Libera o nó atual em pós-ordem
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
    // Garante que o índice esteja dentro do limite HASH_SIZE
    return (tolower(chave[0]) - 'a') % HASH_SIZE;
}

/**
 * @brief Insere uma nova associação pista-suspeito na tabela hash.
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

    // Insere no início da lista ligada (encadeamento para colisão)
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
    // Percorre a lista ligada
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Retorna o suspeito
        }
        atual = atual->proximo;
    }

    return NULL; // Não encontrado
}

/**
 * @brief Libera toda a memória alocada para a tabela hash.
 * @param tabela O array de ponteiros da tabela hash.
 */
void liberarHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *atual = tabela[i];
        // Libera cada nó na lista ligada
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
 * @param pista O conteúdo da pista.
 * @return Ponteiro para o novo PistaNode criado.
 */
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novoNo = (PistaNode *)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para PistaNode");
        exit(EXIT_FAILURE);
    }

    // Inicializa
    strncpy(novoNo->pista, pista, MAX_STR - 1);
    novoNo->pista[MAX_STR - 1] = '\0';

    novoNo->esquerda = NULL;
    novoNo->direita = NULL;

    return novoNo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma recursiva.
 * @param raiz O nó raiz da subárvore atual.
 * @param pista O conteúdo da pista a ser inserida.
 * @return O nó raiz da subárvore atualizada.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista); // Caso base: insere aqui
    }

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Vai para a esquerda
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);  // Vai para a direita
    }
    // Se for igual, não faz nada (não insere duplicatas)

    return raiz;
}

/**
 * @brief Exibe todas as pistas armazenadas na BST em ordem alfabética (percurso In-ordem).
 * @param raiz O nó raiz da BST.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista); // Imprime o nó
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
 * @param raiz O nó atual (Hall de Entrada).
 * @param pistas_coletadas A raiz da BST de pistas coletadas.
 * @param tabela_suspeitos A tabela hash.
 * @return A raiz atualizada da BST de pistas coletadas.
 */
PistaNode* explorarSalas(Sala *raiz, PistaNode *pistas_coletadas, HashNode *tabela_suspeitos[]) {
    Sala *atual = raiz; // Inicia no Hall
    char escolha;

    printf("\n--- INICIO DA EXPLORACAO DA MANSAO ---\n");
    printf("Pressione 's' para sair a qualquer momento e iniciar o julgamento.\n");

    while (atual != NULL) { // Loop de navegação
        printf("\nVoce esta em: **%s**\n", atual->nome);

        // Verifica e coleta a pista, se houver
        if (atual->pista_estatica[0] != '\0') {
            const char* suspeito_da_pista = encontrarSuspeito(tabela_suspeitos, atual->pista_estatica);
            
            sleep(1); 
            printf("----------------------------------------\n");
            printf("!! PISTA ENCONTRADA !!\n");
            printf("Detalhe: \"%s\"\n", atual->pista_estatica);
            
            pistas_coletadas = inserirPista(pistas_coletadas, atual->pista_estatica); 
            
            if (suspeito_da_pista != NULL) {
                printf("Indicio aponta para: **%s**\n", suspeito_da_pista);
            }
            sleep(1);
            printf("Pista adicionada ao seu caderno de notas.\n");
            atual->pista_estatica[0] = '\0'; // Limpa a pista para evitar coleta duplicada
            printf("----------------------------------------\n");
        }

        // Informa se o caminho acabou (nó-folha)
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

        // Lógica de navegação
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
 * @brief Percorre a BST (In-order) e conta quantas pistas coletadas apontam para o acusado.
 *
 * @param raiz O nó raiz da BST.
 * @param acusacao O nome do suspeito sendo acusado.
 * @param contador Ponteiro para a variável onde o total de provas será armazenado.
 * @param tabela_suspeitos A tabela hash para consultar a associação pista/suspeito.
 */
void contarProvas(PistaNode *raiz, const char *acusacao, int *contador, HashNode *tabela_suspeitos[]) {
    if (raiz == NULL) return;
    
    contarProvas(raiz->esquerda, acusacao, contador, tabela_suspeitos); // Esquerda
    
    // Processa o nó atual: verifica se a pista aponta para o acusado
    const char* suspeito_pista = encontrarSuspeito(tabela_suspeitos, raiz->pista);
    
    if (suspeito_pista != NULL && strcmp(suspeito_pista, acusacao) == 0) {
        (*contador)++; // Incrementa o contador através do ponteiro
    }
    
    contarProvas(raiz->direita, acusacao, contador, tabela_suspeitos); // Direita
}

/**
 * @brief Conduz a fase de julgamento final e verifica se há provas suficientes (>= 2).
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
    
    // Leitura da acusação (usa fgets para ler nomes com espaços)
    if (fgets(acusacao, MAX_STR, stdin) == NULL) {
        fprintf(stderr, "Erro na leitura da acusacao.\n");
        return;
    }
    acusacao[strcspn(acusacao, "\n")] = '\0'; // Remove o caractere de nova linha

    // Conta as provas usando a função recursiva
    contarProvas(pistas_coletadas, acusacao, &contagem_provas, tabela_suspeitos);

    sleep(1.5);
    printf("\n=> O Detetive acusa: **%s**\n", acusacao);
    printf("=> Pistas encontradas contra o acusado: **%d**\n", contagem_provas);

    // Veredito (Critério: Pelo menos duas pistas)
    if (contagem_provas >= 2) { 
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
 * @brief Inicializa o programa, contém o loop principal de repetição e gerencia a alocação/liberação de memória entre os jogos.
 */
int main() {
    char jogar_novamente; // Variável de controle do loop principal

    // Mensagem de início (fora do loop)
    printf("========================================\n");
    printf("      ##### ENIGMA STUDIOS ##### \n");
    printf("========================================\n");
    sleep(2);

    // O loop 'do-while' garante que o jogo seja executado pelo menos uma vez
    do {
        // --- 1. CONFIGURAÇÃO DO NOVO JOGO ---
        
        // Inicializa a Tabela Hash para o novo jogo
        HashNode *tabela_suspeitos[HASH_SIZE];
        inicializarHash(tabela_suspeitos);
        
        // Monta o Mapa da Mansão (cria todos os nós e pistas)
        Sala *hall = montarMapa();
        
        // Popula a Tabela Hash com as associações pista-suspeito
        popularHash(tabela_suspeitos);
        
        // A BST de pistas coletadas começa vazia
        PistaNode *pistas_coletadas = NULL;
        
        sleep(1);
        printf("\n========================================\n");
        printf("MYSTERY OF THE MANSION - DETECTIVE QUEST \n");
        printf("========================================\n");
        sleep(1.5);
        
        // --- 2. EXECUÇÃO DO JOGO ---
        
        // Explora as salas e coleta as pistas
        pistas_coletadas = explorarSalas(hall, pistas_coletadas, tabela_suspeitos);

        // Realiza o julgamento final
        verificarSuspeitoFinal(pistas_coletadas, tabela_suspeitos);
        
        // --- 3. LIMPEZA E REPETIÇÃO ---
        
        // Libera a memória alocada para o mapa, pistas e tabela hash do jogo que acabou
        printf("\n--- FIM DA PARTIDA ---\n");
        liberarMapa(hall);
        liberarPistas(pistas_coletadas);
        liberarHash(tabela_suspeitos);
        printf("Memoria do jogo liberada com sucesso.\n");
        
        // Pergunta de repetição
        printf("\nDeseja iniciar uma nova investigacao? [s] Sim / [n] Nao: ");
        if (scanf(" %c", &jogar_novamente) != 1) {
            jogar_novamente = 'n'; // Assume 'não' em caso de erro
        }
        limparBuffer(); // Limpa o buffer após a leitura

        jogar_novamente = tolower(jogar_novamente); // Padroniza a resposta

    } while (jogar_novamente == 's'); // Continua se a resposta for 's'

    // Mensagem final
    printf("\n========================================\n");
    printf("  Obrigado por jogar! Voltaremos em breve.\n");
    printf("========================================\n");

    return 0; // Encerra o programa
}