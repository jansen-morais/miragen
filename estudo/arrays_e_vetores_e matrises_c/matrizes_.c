#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída

int main() { // Início da função principal do programa

    int index; // Declaração de uma variável inteira 'index' para armazenar o número do aluno

    char *Alunos[3][3] = { // Declaração e inicialização de uma matriz 3x3 de strings 'Alunos'
// Aqui são separadas por colunas na vertica e horizintal
//          0           1       2       : estas são as colunas, 3 no tatal assim como descrito na matriz
        {"Aluno 0", "PT: 30", "Mat: 90"}, // 0  :Dados do Aluno 0 (nome e notas em Português e Matemática)
        {"Aluno 1", "Pt: 60", "Mat: 60"}, // 1  :Dados do Aluno 1
        {"Aluno 2", "Pt: 90", "Mat: 30"} //  2  :Dados do Aluno 2
    };

    printf("Digite o número do Aluno que queira ver as notas...\n"); // Imprime uma mensagem para o usuário
    printf("Para o aluno 0, digite 0\n"); // Instruções para inserir o número do aluno 0
    printf("Para o aluno 1, digite 1\n"); // Instruções para inserir o número do aluno 1
    printf("Para o aluno 2, digite 2\n"); // Instruções para inserir o número do aluno 2

    scanf("%d", &index); // Lê o número do aluno digitado pelo usuário e armazena em 'index'
// Aqui chamara cada coluna da matriz
    printf("As notas do %s são: %s, %s...\n", Alunos[index][0], Alunos[index][1], Alunos[index][2]); // Imprime as notas do aluno selecionado

    return 0; // Indica que o programa terminou com sucesso
}