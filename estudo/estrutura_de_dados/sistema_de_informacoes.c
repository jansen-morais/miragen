#include <stdio.h> // Biblioteca padrão de entrada e saída (printf, scanf)
#include <stdlib.h> // Biblioteca padrão de funções gerais (rand, srand)
#include <time.h> // Biblioteca para funções de tempo (time)
// funçoes que capacitam a leitura da string como rsposta 
#include <stdarg.h>//paa lidar com a manipulação de argumentos variaveis
#include <string.h> // Adicionado: Inclui a biblioteca string.h para usar strcmp()
// Adção para corrigiro o problema do segundo loop
#include <ctype.h> // Adicionado para a função tolower

typedef struct
{
    char nome[50];
    int idade;
    float media;
}Aluno;

int main(){
    Aluno aluno1 = {"joão", 20, 8.5 };

    printf("Aluno: %s\n", aluno1.nome);
    printf("Idadee: %d\n", aluno1.idade);
    printf("Média: %.2f\n", aluno1.media);


            return 0;

}