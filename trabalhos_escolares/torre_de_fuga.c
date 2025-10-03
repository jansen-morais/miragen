#include <stdio.h>    // Para funções de entrada e saída (printf, scanf)
#include <stdlib.h>   // Para funções gerais (EXIT_SUCCESS, system)
#include <string.h>   // Para manipulação de strings (strcmp, strcpy, strcspn)
#include <time.h>     // Para medição de tempo (clock, CLOCKS_PER_SEC)
#include <unistd.h>   // Biblioteca para manipulacao do tempo(sleep), simula tempo de espera.
                      // (OBS: 'sleep' é padrão em sistemas Unix-like (Linux/macOS). Em Windows, use 'Sleep' de <windows.h>.)
#include <time.h>     // Inclui a biblioteca para manipulacao de tempo (time), usada para inicializar o rand() e medir tempo.


// Define o número máximo de componentes que o inventário pode armazenar
#define MAX_COMPONENTES 20
// Constante para o nome do componente-chave a ser buscado
#define CHAVE_BUSCA "Chip Central"

// ---------------------- 1. Criação de Structs ----------------------

// Struct 'Componente': representa um componente individual no inventário
typedef struct {
    char nome[30];      // Campo para armazenar o nome do componente
    char tipo[20];      // Campo para armazenar o tipo (ex: controle, suporte)
    int prioridade;     // Campo para armazenar o nível de prioridade (1 a 10)
} Componente;


// ---------------------- Variáveis Globais ----------------------

// Vetor (Lista Sequencial) para armazenar os componentes da torre
Componente inventario[MAX_COMPONENTES];
// Contador de quantos componentes estão atualmente no inventário
int numComponentes = 0;
// Variável para contagem de comparações nos algoritmos de ordenação
long long comparacoesOrdenacao = 0;
// Variável para contagem de comparações na busca binária
long long comparacoesBuscaBinaria = 0;


// ---------------------- Protótipos das Funções Comuns ----------------------
// Declaração da função para limpar o buffer de entrada (stdin)
void limparBuffer();
// Declaração da função que exibe o menu principal do programa
void exibirMenuPrincipal();
// Declaração da função para cadastrar um novo componente
void cadastrarComponente();
// Declaração da função para listar todos os componentes do vetor
void mostrarComponentes(const Componente comps[], int n);

// ---------------------- Protótipos das Funções de Ordenação e Busca ----------------------
// Declaração do Bubble Sort para ordenação por Nome
void bubbleSortNome(Componente comps[], int n);
// Declaração do Insertion Sort para ordenação por Tipo
void insertionSortTipo(Componente comps[], int n);
// Declaração do Selection Sort para ordenação por Prioridade
void selectionSortPrioridade(Componente comps[], int n);
// Declaração da Busca Binária por Nome (funciona se ordenado por Nome)
void buscaBinariaPorNome(const Componente comps[], int n);

// ---------------------- Função Auxiliar de Medição ----------------------
// Declaração da função para medir o tempo e exibir o desempenho
void medirTempoOrdenacao(void (*algoritmo)(Componente[], int), const char* nomeAlgoritmo, int n);


// ---------------------- Função Principal (main) ----------------------
int main() {
    int opcao; // Variável para a opção do menu

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Loop principal do programa
    do {
        system("clear || cls"); // Limpa a tela do console

        exibirMenuPrincipal(); // Exibe o menu
        printf("Escolha uma opcao: ");
        
        // Lê a opção do usuário e verifica a validade
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1; // Força uma opção inválida
        } else {
            limparBuffer(); // Limpa o buffer após a leitura do inteiro
        }

        // Estrutura switch para processar a opção principal
        switch (opcao) {
            case 1:
                cadastrarComponente(); // Chama a função de cadastro
                break;
            case 2:
                mostrarComponentes(inventario, numComponentes); // Chama a função para listar
                break;
            case 3:
                // Mede o tempo e ordena por Nome (Bubble Sort)
                medirTempoOrdenacao(bubbleSortNome, "Bubble Sort (Nome)", numComponentes);
                break;
            case 4:
                // Mede o tempo e ordena por Tipo (Insertion Sort)
                medirTempoOrdenacao(insertionSortTipo, "Insertion Sort (Tipo)", numComponentes);
                break;
            case 5:
                // Mede o tempo e ordena por Prioridade (Selection Sort)
                medirTempoOrdenacao(selectionSortPrioridade, "Selection Sort (Prioridade)", numComponentes);
                break;
            case 6:
                // Busca Binária (requer ordenação prévia por NOME)
                buscaBinariaPorNome(inventario, numComponentes);
                break;
            case 0:
                printf("\nSaindo do sistema de montagem. Boa sorte na fuga!\n");
                break;
            default:
                printf("\nOpcao invalida. Por favor, tente novamente.\n");
                break;
        }

        // Pausa e aguarda a entrada do usuário, exceto ao sair
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar(); // Aguarda o usuário pressionar ENTER
        }

    } while (opcao != 0); // O loop continua até que o usuário escolha '0' para sair

    return EXIT_SUCCESS; // Retorna 0 (sucesso)
}


// ---------------------- Implementação das Funções Comuns ----------------------

// Função para limpar o buffer de entrada (stdin)
void limparBuffer() {
    int c; // Variável para armazenar cada caractere lido
    // Loop para ler e descartar caracteres do buffer até encontrar '\n' ou EOF
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para exibir o menu principal
void exibirMenuPrincipal() {
    printf("========================================================\n");
    printf("      PLANO DE FUGA - CODIGO DA ILHA (COMPONENTE)\n");
    printf("========================================================\n");
    sleep(2); //Da uma pausa no programa 
    printf(" Componentes atuais: %d/%d\n", numComponentes, MAX_COMPONENTES);
    printf("--------------------------------------------------------\n");
    sleep(1.5); //Da uma pausa no programa
    printf("1. Cadastrar Componente\n");
    printf("2. Listar Componentes Atuais\n");
    printf("--------------------------------------------------------\n");
    sleep(1.5); //Da uma pausa no programa
    printf("--- Estrategias de Ordenacao ---\n");
    printf("3. Ordenar por NOME (Classificação por bolha)\n");
    printf("4. Ordenar por TIPO (Classificação de inserção)\n");
    printf("5. Ordenar por PRIORIDADE (Ordenação por seleção)\n");
    printf("--------------------------------------------------------\n");
    sleep(1.5); //Da uma pausa no programa
    printf("6. Buscar Componente-Chave (Busca por Nome)\n");
    printf("0. Sair do Programa\n");
    printf("--------------------------------------------------------\n");
    sleep(1.5); //Da uma pausa no programa
}

// Função para cadastrar um novo componente no inventário
void cadastrarComponente() {
    // Verifica se o vetor está cheio
    if (numComponentes >= MAX_COMPONENTES) {
        printf("\nInventario cheio! Nao e possivel adicionar mais componentes.\n");
        return;
    }

    Componente novoComp; // Cria uma variável temporária para o novo componente

    sleep(1.5); //Da uma pausa no programa
    printf("\n--- Cadastrar Novo Componente ---\n");
    
    // Leitura do Nome (usando fgets para segurança)
    printf("Nome do Componente: ");
    fgets(novoComp.nome, sizeof(novoComp.nome), stdin);
    // Remove o '\n' que fgets adiciona no final da string
    novoComp.nome[strcspn(novoComp.nome, "\n")] = 0; 

    // Leitura do Tipo (usando fgets para segurança)
    printf("Tipo (ex: controle, suporte, propulsao): ");
    fgets(novoComp.tipo, sizeof(novoComp.tipo), stdin);
    novoComp.tipo[strcspn(novoComp.tipo, "\n")] = 0;

    // Leitura da Prioridade (validando a faixa de 1 a 10)
    do {
        printf("Prioridade (1 a 10): ");
        if (scanf("%d", &novoComp.prioridade) != 1) {
            printf("Entrada invalida. Por favor, digite um numero inteiro.\n");
            limparBuffer(); // Limpa o buffer em caso de erro de leitura
            novoComp.prioridade = -1; // Força a repetição
        } else {
            limparBuffer(); // Limpa o buffer após leitura bem-sucedida do inteiro
            if (novoComp.prioridade < 1 || novoComp.prioridade > 10) {
                printf("Prioridade deve estar entre 1 e 10.\n");
            }
        }
    } while (novoComp.prioridade < 1 || novoComp.prioridade > 10);

    // Adiciona o novo componente na próxima posição livre do vetor
    inventario[numComponentes] = novoComp;
    numComponentes++; // Incrementa o contador de componentes
    
    printf("\nComponente '%s' cadastrado com sucesso!\n", novoComp.nome);
}

// Função para listar todos os componentes do inventário de forma formatada
void mostrarComponentes(const Componente comps[], int n) {
    int i; // Variável de controle do loop

    printf("\n--- Componentes Atuais (%d itens) ---\n", n);

    // Verifica se o inventário está vazio
    if (n == 0) {
        printf("Inventario vazio.\n");
        return;
    }

    // Imprime um cabeçalho formatado para a tabela
    printf("--------------------------------------------------------------\n");
    printf("%-25s %-15s %-10s\n", "Nome", "Tipo", "Prioridade");
    printf("--------------------------------------------------------------\n");

    // Loop para percorrer e imprimir os dados de cada componente
    for (i = 0; i < n; i++) {
        // Imprime os dados do componente, usando formatação para alinhar colunas
        printf("%-25s %-15s %-10d\n", comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
    printf("--------------------------------------------------------------\n");
}


// ---------------------- Implementação dos Algoritmos de Ordenação ----------------------

// Função de ordenação Bubble Sort: Ordena os componentes por NOME (alfabético)
// Recebe o vetor de componentes e o número de itens
void bubbleSortNome(Componente comps[], int n) {
    int i, j; // Variáveis de controle para os loops
    Componente temp; // Variável temporária para a troca
    
    // Reinicia o contador global de comparações
    comparacoesOrdenacao = 0;

    // Loop externo: controla o número de passes necessários
    for (i = 0; i < n - 1; i++) {
        // Loop interno: compara e troca elementos adjacentes
        for (j = 0; j < n - 1 - i; j++) {
            comparacoesOrdenacao++; // Incrementa o contador de comparações
            
            // Compara os nomes usando strcmp (retorna > 0 se o primeiro for 'maior' alfabeticamente)
            if (strcmp(comps[j].nome, comps[j + 1].nome) > 0) {
                // Realiza a troca dos structs inteiros
                temp = comps[j];
                comps[j] = comps[j + 1];
                comps[j + 1] = temp;
            }
        }
    }
}

// Função de ordenação Insertion Sort: Ordena os componentes por TIPO (alfabético)
// Recebe o vetor de componentes e o número de itens
void insertionSortTipo(Componente comps[], int n) {
    int i, j; // Variáveis de controle para os loops
    Componente chave; // Variável para armazenar o elemento a ser inserido
    
    // Reinicia o contador global de comparações
    comparacoesOrdenacao = 0;

    // Loop externo: percorre o vetor a partir do segundo elemento
    for (i = 1; i < n; i++) {
        chave = comps[i]; // O elemento atual a ser inserido na sublista ordenada
        j = i - 1;        // Posição anterior

        // Loop interno: move os elementos maiores para a direita
        // A comparação é baseada no campo 'tipo'
        // Adiciona 1 à comparação na condição do while
        while (j >= 0 && strcmp(comps[j].tipo, chave.tipo) > 0) {
            comparacoesOrdenacao++; // Incrementa o contador de comparações
            comps[j + 1] = comps[j]; // Move o elemento para a direita
            j = j - 1;
        }
        
        // Contabiliza a última comparação que falhou na condição do while
        if (j >= 0) {
            comparacoesOrdenacao++;
        }
        
        comps[j + 1] = chave; // Insere a chave na sua posição correta
    }
}

// Função de ordenação Selection Sort: Ordena os componentes por PRIORIDADE (decrescente)
// Recebe o vetor de componentes e o número de itens
void selectionSortPrioridade(Componente comps[], int n) {
    int i, j; // Variáveis de controle para os loops
    int max_idx; // Índice do componente de maior prioridade (máximo)
    Componente temp; // Variável temporária para a troca
    
    // Reinicia o contador global de comparações
    comparacoesOrdenacao = 0;

    // Loop externo: percorre o vetor do início ao penúltimo elemento
    for (i = 0; i < n - 1; i++) {
        max_idx = i; // Assume que o elemento atual é o de maior prioridade

        // Loop interno: encontra o componente de maior prioridade restante
        for (j = i + 1; j < n; j++) {
            comparacoesOrdenacao++; // Incrementa o contador de comparações
            
            // Compara as prioridades (decrescente: maior número de prioridade vem primeiro)
            if (comps[j].prioridade > comps[max_idx].prioridade) {
                max_idx = j; // Atualiza o índice do máximo
            }
        }

        // Se o elemento de maior prioridade não for o atual, realiza a troca
        if (max_idx != i) {
            temp = comps[i];
            comps[i] = comps[max_idx];
            comps[max_idx] = temp;
        }
    }
}


// ---------------------- Implementação da Busca ----------------------

// Função para buscar o componente-chave usando Busca Binária (por NOME)
// PRÉ-REQUISITO: O vetor DEVE estar ordenado por NOME
void buscaBinariaPorNome(const Componente comps[], int n) {
    int esquerda = 0; // Índice inicial
    int direita = n - 1; // Índice final
    int meio; // Índice do meio
    int encontrado = 0; // Flag de sucesso
    int cmp; // Variável para armazenar o resultado de strcmp
    
    // Reinicia o contador de comparações
    comparacoesBuscaBinaria = 0;

    printf("\n--- Busca Binaria por Componente-Chave ---\n");
    printf("Buscando o item critico: '%s'\n", CHAVE_BUSCA);
    
    // Verifica se o vetor está vazio
    if (n == 0) {
        printf("Inventario vazio. Impossivel buscar.\n");
        return;
    }

    // O loop continua enquanto o lado esquerdo não ultrapassar o lado direito
    while (esquerda <= direita) {
        meio = esquerda + (direita - esquerda) / 2; // Calcula o meio de forma segura
        comparacoesBuscaBinaria++; // Incrementa o contador

        // Compara o nome no meio do vetor com o nome a ser buscado
        cmp = strcmp(comps[meio].nome, CHAVE_BUSCA);

        if (cmp == 0) { // Item encontrado (cmp é 0)
            encontrado = 1;
            printf("\nCHAVE ENCONTRADA! Iniciando a montagem da torre...\n");
            printf("Nome: %s | Tipo: %s | Prioridade: %d\n", comps[meio].nome, comps[meio].tipo, comps[meio].prioridade);
            break; // Sai do loop
        } else if (cmp < 0) { 
            // Nome no meio é "menor" que o buscado. Busca na metade direita.
            esquerda = meio + 1;
        } else { // cmp > 0
            // Nome no meio é "maior" que o buscado. Busca na metade esquerda.
            direita = meio - 1;
        }
    }

    // Exibe o resultado da busca
    if (!encontrado) {
        printf("\nComponente-chave '%s' nao encontrado no inventario.\n", CHAVE_BUSCA);
        printf("E necessario coletar este item critico antes de iniciar a torre.\n");
    }
    
    // Exibe o desempenho da busca
    printf("\nTotal de comparacoes realizadas (Busca Binaria): %lld\n", comparacoesBuscaBinaria);
}


// ---------------------- Implementação da Medição de Desempenho ----------------------

// Função que executa o algoritmo de ordenação, mede o tempo e exibe os resultados
void medirTempoOrdenacao(void (*algoritmo)(Componente[], int), const char* nomeAlgoritmo, int n) {
    clock_t inicio, fim; // Variáveis para armazenar o tempo
    double tempo_execucao; // Variável para o tempo final em segundos
    
    // Verifica se há componentes para ordenar
    if (n <= 1) {
        printf("\nNao ha componentes suficientes (%d) para realizar a ordenacao.\n", n);
        return;
    }

    printf("\n--- Executando %s ---\n", nomeAlgoritmo);

    // 1. Inicia a contagem do tempo
    inicio = clock(); 
    
    // 2. Chama o algoritmo de ordenação passado como parâmetro (ponteiro para função)
    algoritmo(inventario, n); 
    
    // 3. Finaliza a contagem do tempo
    fim = clock(); 

    // 4. Calcula o tempo de execução em segundos
    tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // 5. Exibe os resultados e o desempenho educacional
    printf("\nOrdenacao por %s concluida com sucesso!\n", nomeAlgoritmo);
    printf("Estrategia de Priorizacao: %s\n", nomeAlgoritmo);
    printf("Total de comparacoes realizadas: %lld\n", comparacoesOrdenacao);
    printf("Tempo de execucao (segundos): %f\n", tempo_execucao);
    
    // 6. Mostra o resultado final da ordenação
    mostrarComponentes(inventario, n);

        return 0;

}