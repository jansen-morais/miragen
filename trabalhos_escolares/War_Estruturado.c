#include <stdio.h>   // Inclui a biblioteca padrao de entrada e saida (printf, scanf, fgets, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrao (malloc, calloc, free, rand, srand) para alocacao dinamica e numeros aleatorios.
#include <string.h>  // Inclui a biblioteca para manipulacao de strings (strcspn, strcpy, strcmp).
#include <unistd.h>  // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
#include <time.h>    // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand().

    // --- Definicao da Struct ---
    // Define a estrutura de dados que representa um território no jogo.
    typedef struct {
        char nome[30];  // Nome do território.
        char cor[10];   // Cor do exército que controla o território (o "dono").
        int tropas;     // Quantidade de tropas estacionadas no território.
    } Territorio;

        // --- Variavel Global para o Numero de Territorios ---
        // Armazena o tamanho do array alocado dinamicamente. É global para ser acessível por todas as funções.
        int g_num_territorios = 0;

// ------------------------------------------------------------------------------------------------
// --- Funcoes Auxiliares ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Limpa o buffer de entrada (stdin) após o uso de scanf.
 * * Essencial para remover o caractere '\n' deixado pelo scanf,
 * garantindo que o próximo fgets não leia uma string vazia.
 */
        void limpar_buffer() {
            int c;
            // Le e descarta caracteres um por um até encontrar a nova linha ou o Fim do Arquivo.
            while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Simula a rolagem de um dado de 6 faces.
 * * Utiliza a função rand() para gerar um número pseudo-aleatório no intervalo [1, 6].
 * @return int: O valor sorteado do dado.
 */
int rolar_dado() {
    // rand() % 6 gera um número de 0 a 5. Somando 1, o intervalo é [1, 6].
    return (rand() % 6) + 1;
}

// ------------------------------------------------------------------------------------------------
// --- Funcoes de Gerenciamento de Memoria ---
// ------------------------------------------------------------------------------------------------

/**
     * @brief Libera a memória alocada dinamicamente pelo programa.
     * * É crucial para evitar vazamentos de memória (requisito não funcional).
     * @param mapa Ponteiro para o bloco de memória a ser liberado.
     */
    void liberar_memoria(Territorio* mapa) {
        if (mapa != NULL) {
            printf("\n--- Liberando memoria alocada ---\n");
            // free() é a função que devolve o bloco de memória da heap para o sistema.
            free(mapa); 
            printf("Memoria liberada com sucesso.\n");
        }
    }
        
        

/**
 * @brief Solicita o número de territórios ao usuário e aloca a memória necessária.
 * * Utiliza alocação dinâmica (calloc) para criar o vetor de structs Territorio.
 * @return Territorio*: O ponteiro para o array de territórios alocado, ou NULL em caso de falha.
 */
Territorio* alocar_territorios() {
    int num;
    Territorio* mapa = NULL; // Ponteiro que receberá o endereço do bloco alocado.

    printf("\n==========================================\n");
    printf("            *****JOGO WAR***** \n");
    printf("             Iniciando o Jogo! \n");
    printf("==========================================\n");
    sleep(2); // serve para atrazar a entrega do proximo print na tela 



    printf("==========================================\n");
    printf("        SISTEMA DE CONFIGURACAO \n");
    printf("==========================================\n");

    // Laco para garantir que o usuário digite um número inteiro positivo.
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

    // Alocacao Dinamica: calloc aloca e inicializa com zero.
    mapa = (Territorio*)calloc(g_num_territorios, sizeof(Territorio));

    if (mapa == NULL) {
        perror("Erro ao alocar memoria para o mapa de territorios");
        g_num_territorios = 0; 
        return NULL;
    }

    printf("Memoria alocada com sucesso para %d territorios.\n\n", g_num_territorios);
    return mapa; // Retorna o ponteiro inicial.
}

// ------------------------------------------------------------------------------------------------
// --- Funcoes de Manipulacao de Dados ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Realiza o cadastro de todos os territórios.
 * * Recebe o ponteiro do array alocado e preenche seus dados.
 * @param mapa Ponteiro para o array dinâmico de territórios.
 */
    void cadastrar_territorios(Territorio* mapa) {
        int i;
        
        printf("==========================================\n");
        printf("   SISTEMA DE CADASTRO DE TERRITORIOS \n");
        printf("==========================================\n");
        sleep(1);  // serve para atrazar a entrega do proximo print na tela 
        printf("Iniciando o cadastro de %d territorios.\n\n", g_num_territorios);

            for (i = 0; i < g_num_territorios; i++) {
                // Usa Aritmética de Ponteiros para apontar para o território atual.
                Territorio* t = mapa + i; 

                printf("--- Cadastro do Territorio %d de %d ---\n", i + 1, g_num_territorios);

                // 1. Leitura do NOME (Uso de '->' para acessar membro via ponteiro)
                printf("Digite o nome do territorio (max 29 caracteres): ");
                if (fgets(t->nome, sizeof(t->nome), stdin) == NULL) return; 
                // Remove o '\n' e garante o terminador de string ('\0').
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
        printf("  DADOS DOS TERRITORIOS ATUAIS \n");
        printf("==========================================\n");
        sleep(1);  // serve para atrazar a entrega do proximo print na tela

        for (i = 0; i < g_num_territorios; i++) {
            Territorio* t = mapa + i; // Ponteiro para o território atual.

            printf("Territorio %d:\n", i + 1);
            printf("  Nome: %s\n", t->nome);
            printf("  Cor do Exercito: %s\n", t->cor);
            printf("  Tropas: %d\n", t->tropas);
            printf("---\n");
        sleep(1);  // serve para atrazar a entrega do proximo print na tela 
        }
        // CORREÇÃO: A linha 'return 0;' foi removida daqui, pois a função é VOID.
    }

// ------------------------------------------------------------------------------------------------
// --- Funcao de Batalha/Ataque ---
// ------------------------------------------------------------------------------------------------

/**
 * @brief Simula um ataque entre um território atacante e um defensor.
 * * O ataque é resolvido por rolagem de dados (rand()).
 * * O resultado altera as tropas e, se vitorioso, a cor do território defensor.
 * @param atacante Ponteiro para a struct do território atacante (dados são modificados).
 * @param defensor Ponteiro para a struct do território defensor (dados são modificados).
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_ataque, dado_defesa;
    
    printf("\n--- SIMULACAO DE ATAQUE ---\n");
    printf("Atacante: %s (%s) vs Defensor: %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Pausa para simular a preparação da rolagem (melhora a experiência do usuário).
    printf("Rodando os dados...");
    fflush(stdout); // Garante que o texto apareça antes do sleep.
    sleep(1);  // serve para atrazar a entrega do proximo print na tela

    // 1. Rolagem dos Dados (Novos valores a cada chamada)
    dado_ataque = rolar_dado(); 
    dado_defesa = rolar_dado(); 

    printf("\nRolagem de Dados:\n");
    printf("  Dado do Ataque: %d\n", dado_ataque);
    printf("  Dado da Defesa: %d\n", dado_defesa);
    
    sleep(2); // Pausa para o usuário ler os valores dos dados.

    // 2. Resolucao do Combate
    if (dado_ataque > dado_defesa) {
        // ATACANTE VENCEU!
        
        printf("\nRESULTADO: O ataque foi VITORIOSO! %s conquistou %s!\n", atacante->nome, defensor->nome);
        
        // Uso de Ponteiros: Conquista de Território
        // Transfere a cor do atacante para o defensor (strcpy da string).
        strcpy(defensor->cor, atacante->cor);
        
        // Calcula e transfere metade das tropas.
        int tropas_transferidas = atacante->tropas / 2;
        
        // Atualiza as tropas dos dois territórios.
        defensor->tropas += tropas_transferidas; 
        atacante->tropas -= tropas_transferidas;
        
        printf("  > %s mudou de cor para %s.\n", defensor->nome, defensor->cor);
        printf("  > %d tropas foram transferidas de %s para %s.\n", 
               tropas_transferidas, atacante->nome, defensor->nome);
        
    } else { // Defesa vence ou há empate (Defensor ganha no empate).
        // DEFENSOR VENCEU!
            sleep(1);  // serve para atrazar a entrega do proximo print na tela
        printf("\nRESULTADO: A defesa foi bem-sucedida! %s manteve o controle de %s.\n", defensor->nome, defensor->nome);
            sleep(1);  // serve para atrazar a entrega do proximo print na tela

        // Penalidade: Atacante perde 1 tropa.
        if (atacante->tropas > 1) { 
            atacante->tropas--;
            printf("  > %s perdeu 1 tropa no ataque.\n", atacante->nome);
        } else {
            printf("  > %s ficou com tropas insuficientes para perder mais tropas (1 tropa restante).\n", atacante->nome);
        }
    }
    sleep(1.5); // serve para atrazar a entrega do proximo print na tela 
 
}

/**
 * @brief Menu para selecionar e executar um ataque.
 * * Gerencia a entrada do usuário e valida as regras de ataque (cor, tropas, etc.).
 * @param mapa Ponteiro para o array dinâmico de territórios.
 */
void menu_ataque_rodada(Territorio* mapa) {
    int id_atacante, id_defensor;
    Territorio *p_atacante, *p_defensor; // Ponteiros para os territórios selecionados.
    int ataque_bem_sucedido = 0; 

        sleep(1.5);  // serve para atrazar a entrega do proximo print na tela    
    printf("\n==========================================\n");
    printf("             MENU DE ATAQUE \n");
    printf("==========================================\n");
        sleep(1.5);  // serve para atrazar a entrega do proximo print na tela
    
    // Loop que permite múltiplas tentativas de seleção antes de um ataque ser executado.
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
        
        if (id_atacante == 0) return; // Sai se o usuário cancelar.
        
        if (id_atacante < 1 || id_atacante > g_num_territorios) {
            printf("ID de territorio atacante invalido.\n");
            continue;
        }

        // Converte o ID (base 1) para Ponteiro (base 0).
        p_atacante = mapa + (id_atacante - 1); 
        
        // Validacao: Tropas mínimas (precisa de 1 para ficar e 1 para atacar).
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

        // 4. Execucao
        atacar(p_atacante, p_defensor);
        ataque_bem_sucedido = 1; // Ataque realizado com sucesso, sai do loop de seleção.
    }
}


// ------------------------------------------------------------------------------------------------
// --- Funcao Principal (main) ---
// ------------------------------------------------------------------------------------------------
        int main() {
            // Inicializa o gerador de números aleatórios com o tempo atual (garante aleatoriedade).
            srand(time(NULL)); 
            
            Territorio* mapa_territorios = NULL; // Ponteiro principal para o mapa.
            int opcao = 0; 

            // 1. ALOCACAO DE MEMORIA E DEFINICAO DO TAMANHO
            mapa_territorios = alocar_territorios();
            
            if (mapa_territorios == NULL) {
                printf("Falha critica na alocacao de memoria. Encerrando o programa.\n");
                return 1;
            }
            
            // 2. CADASTRO DOS TERRITORIOS
            cadastrar_territorios(mapa_territorios);

            // 3. LOOP PRINCIPAL DO JOGO (Executa até que a opção de Saída seja escolhida)
            do {
                // Exibe o estado atual do jogo antes de cada turno.
                exibir_territorios(mapa_territorios);

                printf("\n==========================================\n");
                printf("                RODADA \n");
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
                        menu_ataque_rodada(mapa_territorios); 
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
            } while (opcao != 2);

            // 4. LIBERACAO DE MEMORIA (Etapa final crucial)
            liberar_memoria(mapa_territorios);

            // Mensagem de Encerramento final.
            sleep(1.5);
            printf("\n==========================================\n");
            printf("            *****JOGO WAR***** \n");
            printf("            Jogo Encerrado! \n");
            printf("==========================================\n");

            return 0; 
        }

