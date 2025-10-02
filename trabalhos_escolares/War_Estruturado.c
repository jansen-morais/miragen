#include <stdio.h>   // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrao (malloc, calloc, free, rand, srand) para alocacao dinamica e numeros aleatorios.
#include <string.h>  // Inclui a biblioteca para manipulacao de strings (strcspn, strcpy, strcmp).
#include <unistd.h>  // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>    // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().

// ------------------------------------------------------------------------------------------------
// --- DEFINICOES DE ESTRUTURAS E VARIAVEIS GLOBAIS ---
// ------------------------------------------------------------------------------------------------

// Define a estrutura de dados que representa um território no jogo.
typedef struct {
    char nome[30];  // Nome do território.
    char cor[10];   // Cor do exército que controla o território (o "dono").
    int tropas;     // Quantidade de tropas estacionadas no território.
} Territorio;

// NOVA ESTRUTURA: Define a estrutura de dados que representa um jogador.
typedef struct {
    char cor[10];           // Cor do exército do jogador (Ex: "Vermelho").
    char* missao;           // PONTEIRO para a string da missão, alocada dinamicamente.
    int territorios_conquistados; // Contador para a lógica de vitória da missão.
} Jogador;


// Variavel Global para o Numero de Territorios
int g_num_territorios = 0;

// Vetor de strings com as missões estratégicas pré-definidas.
char* MISSOES[] = {
    "Conquistar 3 territorios seguidos.",
    "Eliminar todas as tropas da cor Vermelha.",
    "Controlar pelo menos 5 territorios.",
    "Ter pelo menos 15 tropas distribuidas.",
    "Conquistar 2 territorios com mais de 5 tropas."
};
const int TOTAL_MISSOES = sizeof(MISSOES) / sizeof(MISSOES[0]);


// ------------------------------------------------------------------------------------------------
// --- PROTÓTIPOS DE FUNÇÕES (NOVAS) ---
// ------------------------------------------------------------------------------------------------
void atribuirMissao(Jogador* jogador);
int verificarMissao(Jogador* jogador, Territorio* mapa);
void exibirMissao(const Jogador* jogador);


// ------------------------------------------------------------------------------------------------
// --- Funcoes Auxiliares ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Limpa o buffer de entrada (stdin) após o uso de scanf.
 * Garante que o próximo fgets não leia uma string vazia.
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Le e descarta caracteres
}

/**
 * @brief Simula a rolagem de um dado de 6 faces.
 * @return int: O valor sorteado do dado (1 a 6).
 */
int rolar_dado() {
    return (rand() % 6) + 1; // Gera numero entre 1 e 6.
}

// ------------------------------------------------------------------------------------------------
// --- Funcoes de Gerenciamento de Memoria ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Libera a memória alocada dinamicamente para os territórios e, agora, para as missões.
 * @param mapa Ponteiro para o bloco de memória dos territórios.
 * @param jogador Ponteiro para a struct do jogador (inclui a missão alocada).
 */
void liberar_memoria(Territorio* mapa, Jogador* jogador) {
    if (mapa != NULL) {
        printf("\n--- Liberando memoria alocada ---\n");
        free(mapa); // Libera a memória dos territórios.
        printf("Memoria dos territorios liberada.\n");
    }
    
    // NOVO REQUISITO: Liberar a memória da missão.
    if (jogador != NULL && jogador->missao != NULL) {
        free(jogador->missao); // Libera a string da missão alocada com malloc.
        printf("Memoria da missao do jogador liberada.\n");
    }
    
}

/**
 * @brief Solicita o número de territórios ao usuário e aloca a memória necessária.
 * @return Territorio*: O ponteiro para o array de territórios alocado, ou NULL em caso de falha.
 */
Territorio* alocar_territorios() {
    int num;
    Territorio* mapa = NULL; 

    printf("\n==========================================\n");
    printf("             *****JOGO WAR***** \n");
    printf("              Iniciando o Jogo! \n");
    printf("==========================================\n");
    sleep(2); 

    printf("==========================================\n");
    printf("         SISTEMA DE CONFIGURACAO \n");
    printf("==========================================\n");

    do {
        printf("Digite o numero total de territorios a serem cadastrados: ");
        if (scanf("%d", &num) != 1 || num <= 0) {
            printf("Erro: Numero invalido. Digite um valor inteiro positivo.\n");
            limpar_buffer();
            num = 0;
        }
    } while (num <= 0);
    
    limpar_buffer();
    g_num_territorios = num;

    // calloc aloca e inicializa com zero.
    mapa = (Territorio*)calloc(g_num_territorios, sizeof(Territorio));

    if (mapa == NULL) {
        perror("Erro ao alocar memoria para o mapa de territorios");
        g_num_territorios = 0; 
        return NULL;
    }

    printf("Memoria alocada com sucesso para %d territorios.\n\n", g_num_territorios);
    return mapa; 
}

// ------------------------------------------------------------------------------------------------
// --- Funcoes de Missao e Jogador ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Sorteia uma missão do vetor de MISSOES e aloca-a dinamicamente para o jogador.
 * * Uso de PONTEIRO e alocação dinâmica (requisito).
 * @param jogador Ponteiro para a struct do Jogador (passagem por referência).
 */
void atribuirMissao(Jogador* jogador) {
    // Sorteia um índice no intervalo [0, TOTAL_MISSOES - 1].
    int indice_sorteado = rand() % TOTAL_MISSOES;
    char* missao_base = MISSOES[indice_sorteado];
    
    // Armazenamento Dinâmico (malloc) da Missão (requisito)
    // +1 para o caractere nulo '\0'.
    jogador->missao = (char*)malloc(strlen(missao_base) + 1); 

    if (jogador->missao == NULL) {
        perror("Erro ao alocar memoria para a missao do jogador");
        return;
    }

    // Copia a missão sorteada para a memória alocada dinamicamente.
    strcpy(jogador->missao, missao_base);
}

/**
 * @brief Exibe a missão do jogador (apenas uma vez, no início).
 * @param jogador Ponteiro constante para a struct do Jogador (passagem por valor conceitual).
 */
void exibirMissao(const Jogador* jogador) {
    printf("\n==========================================\n");
    printf("              SUA MISSAO  \n");
    printf("==========================================\n");
    printf("Cor do Exercito: %s\n", jogador->cor);
    printf("Objetivo: %s\n", jogador->missao);
    printf("==========================================\n");
    sleep(3);
}

/**
 * @brief Verifica se a missão do jogador foi cumprida.
 * * (Lógica Simples Inicial - requisito)
 * @param jogador Ponteiro para a struct do Jogador (passagem por referência).
 * @param mapa Ponteiro para o array de territórios.
 * @return int: 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(Jogador* jogador, Territorio* mapa) {
    // Lógica 1: Controlar pelo menos 5 territórios.
    if (strcmp(jogador->missao, MISSOES[2]) == 0) { 
        int contagem = 0;
        for (int i = 0; i < g_num_territorios; i++) {
            if (strcmp((mapa + i)->cor, jogador->cor) == 0) {
                contagem++;
            }
        }
        if (contagem >= 5) {
            printf("\nPARABENS! O jogador %s cumpriu sua missao de 'Controlar pelo menos 5 territorios'!\n", jogador->cor);
            return 1;
        }
    }
    
    // Lógica 2: Conquistar 3 territórios seguidos. (Baseado em um contador simples)
    if (strcmp(jogador->missao, MISSOES[0]) == 0) {
         if (jogador->territorios_conquistados >= 3) {
            printf("\nPARABENS! O jogador %s cumpriu sua missao de 'Conquistar 3 territorios seguidos'!\n", jogador->cor);
            return 1;
        }
    }
    
    // Lógica 3: Eliminar todas as tropas de uma cor (Ex: Vermelha).
    if (strcmp(jogador->missao, MISSOES[1]) == 0) {
        // Encontra a cor a ser eliminada (neste caso, "Vermelha" pela descrição da missão)
        int tropas_restantes = 0;
        for (int i = 0; i < g_num_territorios; i++) {
            if (strcmp((mapa + i)->cor, "Vermelha") == 0) {
                tropas_restantes += (mapa + i)->tropas;
            }
        }
        if (tropas_restantes == 0) {
            printf("\nPARABENS! O jogador %s cumpriu sua missao de 'Eliminar todas as tropas da cor Vermelha'!\n", jogador->cor);
            return 1;
        }
    }
    
    // Adicione a lógica para as outras missões aqui.
    
    return 0; // Missão não cumprida.
}


// ------------------------------------------------------------------------------------------------
// --- Funcoes de Manipulacao de Dados (Exibir, Cadastrar) ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Realiza o cadastro de todos os territórios.
 * @param mapa Ponteiro para o array dinâmico de territórios.
 */
void cadastrar_territorios(Territorio* mapa) {
    int i;
    
    printf("==========================================\n");
    printf("  SISTEMA DE CADASTRO DE TERRITORIOS \n");
    printf("==========================================\n");
    sleep(1); 
    printf("Iniciando o cadastro de %d territorios.\n\n", g_num_territorios);

    for (i = 0; i < g_num_territorios; i++) {
        Territorio* t = mapa + i; 

        printf("--- Cadastro do Territorio %d de %d ---\n", i + 1, g_num_territorios);

        // 1. Leitura do NOME
        printf("Digite o nome do territorio (max 29 caracteres): ");
        if (fgets(t->nome, sizeof(t->nome), stdin) == NULL) return; 
        t->nome[strcspn(t->nome, "\n")] = '\0'; 

        // 2. Leitura da COR
        printf("Digite a cor do exercito (max 9 caracteres): ");
        if (fgets(t->cor, sizeof(t->cor), stdin) == NULL) return; 
        t->cor[strcspn(t->cor, "\n")] = '\0'; 

        // 3. Leitura da QUANTIDADE DE TROPAS
        do {
            printf("Digite a quantidade de tropas (minimo 1): ");
            if (scanf("%d", &(t->tropas)) != 1 || t->tropas <= 0) {
                printf("Erro: Entrada invalida. A quantidade de tropas deve ser um inteiro maior que 0.\n");
                limpar_buffer(); 
                t->tropas = 0; 
            }
        } while (t->tropas <= 0);

        limpar_buffer(); 
        printf("\n");
    }
}

/**
 * @brief Exibe todos os territórios cadastrados e seus dados atuais.
 * @param mapa Ponteiro para o array dinâmico de territórios.
 */
void exibir_territorios(Territorio* mapa) {
    int i;
    
    sleep(1);
    printf("\n==========================================\n");
    printf("  DADOS DOS TERRITORIOS ATUAIS \n");
    printf("==========================================\n");
    sleep(1); 

    for (i = 0; i < g_num_territorios; i++) {
        Territorio* t = mapa + i; 

        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", t->nome);
        printf("  Cor do Exercito: %s\n", t->cor);
        printf("  Tropas: %d\n", t->tropas);
        printf("---\n");
        sleep(0.3); 
    }
}

// ------------------------------------------------------------------------------------------------
// --- Funcao de Batalha/Ataque ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Simula um ataque entre dois territórios.
 * @param atacante Ponteiro para a struct do território atacante.
 * @param defensor Ponteiro para a struct do território defensor.
 * @param jogador Ponteiro para a struct do jogador (para atualizar o contador de conquistas).
 * @return int: 1 se o ataque resultou em uma CONQUISTA, 0 caso contrário.
 */
int atacar(Territorio* atacante, Territorio* defensor, Jogador* jogador) {
    int dado_ataque, dado_defesa;
    int houve_conquista = 0;
    
    printf("\n--- SIMULACAO DE ATAQUE ---\n");
    printf("Atacante: %s (%s) vs Defensor: %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    printf("Rodando os dados...");
    fflush(stdout); 
    sleep(1); 

    // 1. Rolagem dos Dados
    dado_ataque = rolar_dado(); 
    dado_defesa = rolar_dado(); 

    printf("\nRolagem de Dados:\n");
    printf("  Dado do Ataque: %d\n", dado_ataque);
    printf("  Dado da Defesa: %d\n", dado_defesa);
    
    sleep(2); 

    // 2. Resolucao do Combate
    if (dado_ataque > dado_defesa) {
        // ATACANTE VENCEU!
        
        printf("\nRESULTADO: O ataque foi VITORIOSO! %s conquistou %s!\n", atacante->nome, defensor->nome);
        
        // Atualiza a cor (Conquista de Território)
        strcpy(defensor->cor, atacante->cor);
        
        // Atualiza o contador de conquistas do jogador.
        jogador->territorios_conquistados++;
        houve_conquista = 1;
        
        // Transfere metade das tropas.
        int tropas_transferidas = atacante->tropas / 2;
        defensor->tropas += tropas_transferidas; 
        atacante->tropas -= tropas_transferidas;
        
        printf("  > %s mudou de cor para %s.\n", defensor->nome, defensor->cor);
        printf("  > %d tropas foram transferidas de %s para %s.\n", 
               tropas_transferidas, atacante->nome, defensor->nome);
        
    } else { // DEFENSOR VENCEU!
        
        printf("\nRESULTADO: A defesa foi bem-sucedida! %s manteve o controle de %s.\n", defensor->nome, defensor->nome);
        
        // Zera o contador de conquistas seguidas se o atacante falhar.
        if (jogador->territorios_conquistados > 0) {
            printf("  > Sequencia de conquistas reiniciada para 0.\n");
            jogador->territorios_conquistados = 0;
        }

        // Penalidade: Atacante perde 1 tropa.
        if (atacante->tropas > 1) { 
            atacante->tropas--;
            printf("  > %s perdeu 1 tropa no ataque.\n", atacante->nome);
        } else {
            printf("  > %s ficou com tropas insuficientes para perder mais tropas (1 tropa restante).\n", atacante->nome);
        }
    }
    sleep(1.5); 
    return houve_conquista;
}

/**
 * @brief Gerencia a seleção dos territórios e executa o ataque.
 * @param mapa Ponteiro para o array dinâmico de territórios.
 * @param jogador Ponteiro para o jogador atual.
 */
void menu_ataque_rodada(Territorio* mapa, Jogador* jogador) {
    int id_atacante, id_defensor;
    Territorio *p_atacante, *p_defensor; 
    int ataque_bem_sucedido = 0; 

    sleep(1.5); 
    printf("\n==========================================\n");
    printf("              MENU DE ATAQUE \n");
    printf("==========================================\n");
    sleep(1.5); 
    
    while (!ataque_bem_sucedido) {
        exibir_territorios(mapa);
        
        // 1. Escolha do Atacante
        printf("\nEscolha o numero do TERRITORIO ATACANTE (1 a %d, ou 0 para CANCELAR): ", g_num_territorios);
        if (scanf("%d", &id_atacante) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limpar_buffer();
            continue; 
        }
        limpar_buffer();
        
        if (id_atacante == 0) return; 
        
        if (id_atacante < 1 || id_atacante > g_num_territorios) {
            printf("ID de territorio atacante invalido.\n");
            continue;
        }

        p_atacante = mapa + (id_atacante - 1); 
        
        // VALIDACAO: O atacante DEVE ser da cor do jogador.
        if (strcmp(p_atacante->cor, jogador->cor) != 0) {
             printf("Erro: O territorio selecionado (%s) nao pertence ao seu exercito (%s).\n", 
                    p_atacante->cor, jogador->cor);
             continue;
        }

        // Validacao: Tropas mínimas.
        if (p_atacante->tropas < 2) {
             printf("Erro: E necessario no minimo 2 tropas para atacar. Tente novamente.\n");
             continue;
        }
        
        // 2. Escolha do Defensor
        printf("Escolha o numero do TERRITORIO DEFENSOR (1 a %d): ", g_num_territorios);
        if (scanf("%d", &id_defensor) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        if (id_defensor < 1 || id_defensor > g_num_territorios) {
            printf("ID de territorio defensor invalido.\n");
            continue;
        }
        
        p_defensor = mapa + (id_defensor - 1); 
        
        // 3. Validacoes de Regras do Jogo
        
        // Regra 1: Não pode atacar a si mesmo.
        if (p_atacante == p_defensor) {
            printf("Erro: Um territorio nao pode atacar a si mesmo.\n");
            continue;
        }
        
        // Regra 2: Não pode atacar territórios da mesma cor.
        if (strcmp(p_atacante->cor, p_defensor->cor) == 0) {
            printf("Erro: Nao e possivel atacar um territorio da mesma cor (%s).\n", p_atacante->cor);
            continue;
        }

        // 4. Execucao do Ataque
        atacar(p_atacante, p_defensor, jogador);
        ataque_bem_sucedido = 1; 
    }
}


// ------------------------------------------------------------------------------------------------
// --- Funcao Principal (main) ---
// ------------------------------------------------------------------------------------------------
int main() {
    // Inicializa o gerador de números aleatórios.
    srand(time(NULL)); 
    
    Territorio* mapa_territorios = NULL; 
    Jogador jogador_principal = {0}; // Inicializa a struct do jogador.
    int opcao = 0; 

    // Define a cor do jogador principal (para fins de missao/lógica).
    strcpy(jogador_principal.cor, "Vermelha");
    
    // 1. ALOCACAO DE MEMORIA E DEFINICAO DO TAMANHO
    mapa_territorios = alocar_territorios();
    
    if (mapa_territorios == NULL) {
        printf("Falha critica na alocacao de memoria. Encerrando o programa.\n");
        return 1;
    }
    
    // 2. CADASTRO DOS TERRITORIOS
    cadastrar_territorios(mapa_territorios);
    
    // 3. ATRIBUICAO E EXIBICAO DA MISSAO (NOVO REQUISITO)
    atribuirMissao(&jogador_principal);
    exibirMissao(&jogador_principal);

    // 4. LOOP PRINCIPAL DO JOGO
    do {
        // Exibe o estado atual do jogo.
        exibir_territorios(mapa_territorios);

        printf("\n==========================================\n");
        printf("                RODADA (%s) \n", jogador_principal.cor);
        printf("==========================================\n");
        printf("O que voce gostaria de fazer?\n");
        printf(" 1. Realizar um ataque\n");
        printf(" 2. Sair do Jogo\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limpar_buffer();
            opcao = 0; 
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                menu_ataque_rodada(mapa_territorios, &jogador_principal); 
                break;
            case 2:
                printf("\nOpcao 'Sair' selecionada. Encerrando o jogo...\n");
                sleep(1);
                break;
            default:
                printf("\nOpcao invalida. Por favor, escolha 1 ou 2.\n");
                sleep(1);
                break;
        }
        
        // 5. VERIFICACAO DE VITORIA POR MISSAO (NOVO REQUISITO)
        if (verificarMissao(&jogador_principal, mapa_territorios) == 1) {
            printf("\n\n##################################################\n");
            printf("#         O JOGADOR %s VENCEU O JOGO!        #\n", jogador_principal.cor);
            printf("##################################################\n\n");
            opcao = 2; // Força a saída do loop do jogo.
        }

    } while (opcao != 2);

    // 6. LIBERACAO DE MEMORIA (Atualizada para incluir a missão)
    liberar_memoria(mapa_territorios, &jogador_principal);

    // Mensagem de Encerramento final.
    sleep(1.5);
    printf("\n==========================================\n");
    printf("             *****JOGO WAR***** \n");
    printf("             Jogo Encerrado! \n");
    printf("==========================================\n");

    return 0; 
}
