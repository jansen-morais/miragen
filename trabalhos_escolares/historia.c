#include <stdio.h>
#include <string.h> //biblioteca para manipulação de strings
#include <ctype.h> //biblioteca para funçoes de caracters.(tolower)
#include <unistd.h> //biblioteca para manipulaçao do tempo(sleep), simula tempo de espera
#include <stdlib.h> //Inclui a biblioteca stdlib.h, que contém funções gerais, como atoi 
//(para converter uma string para um inteiro).
#include <stdarg.h>//paa lidar com a manipulação de argumentos variaveis
#include <stdlib.h> // Biblioteca padrão de funções gerais (rand, srand)
#include <string.h> // Adicionado: Inclui a biblioteca string.h para usar strcmp()

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (ungetc(c, stdin) != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

int print_digitado(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[2048];
    int len = snprintf(buffer, sizeof(buffer), format, args);
    //printf("buffer formatado: %s\n", buffer);//imprime o buffer formatado

    if (len < 0) { // Correção: operador <
        fprintf(stderr, "Erro ao formatar a string.\n");
        va_end(args);
        return -1;
    }

    for (int i = 0; i < len; i++){ // Correção: operador <
        printf("%c", buffer[i]);
        fflush(stdout);//adcionado para sincronizar o buffer de saida
        usleep(50000);
    }

    va_end(args);
    return len;
}

int main() {

   
    print_digitado("...\n");
    sleep(2);
        //limpar_buffer();
    print_digitado("Ola, tudo bem!\n...\n");
    //getchar();
    sleep(1.5);

    char nome[50];
    print_digitado("Como vc se chama?\n");
    fflush(stdin);//limpa o buffer do teclado antes de ler o nome
    fgets(nome, sizeof(nome), stdin);
   // nome[strcspn(nome, "\n")] = 0;
    nome[strcspn(nome, "\n")] = 0;//essa linha vai substituir o caractere de nova linnha 
   //deixado para traz por 0
   // getchar();

    //limpar_buffer(); // Limpeza do buffer após fgets(vc acaba tendo que apertar enter para comtinuar)

    //printf("%s\n", nome);

    sleep(1.5);
    print_digitado("...\n");
    sleep(1.5);
    print_digitado("Entendi\n"); sleep(1);
    printf("%s\n",nome);
    //getchar(); 
    sleep(1.5);
    print_digitado("Gostei do nome\n :)\n");
    sleep(1.5);
    print_digitado("Pois bem,"); printf("%s...\n",nome);
    sleep(1.5);

char resposta[4];         
    print_digitado("posso contar-lhe uma curiosidade?\n");
    scanf("%3s",resposta);
        for(int i=0;resposta[i];i++){
            resposta[i]=tolower(resposta[i]);
        }
    if(strcmp(resposta,"sim")==0){
        
        char repetir[4];
// Aqui usarei o "do/while" pois preciso que execulte almenos uam vez antes do loop       
       do
       {
        
        print_digitado("Voçê sabia que\n");

        print_digitado("Todas as corujas têm uma postura ereta e olhos para frente,\n");
        sleep(1.5);
        print_digitado("o que as dá visão binocular, como a dos seres humanos.\n");
        sleep(1);
        print_digitado("Entretanto, os olhos delas não são esferas,\n");
        sleep(1);
        print_digitado("mas sim tubos que providenciam uma percepção de profundidade melhor e permite que elas vejam suas presas a grande distância\n");
        sleep(1);
        print_digitado("por conta disso a visõa de perto delas fica prejudicada\n");
        sleep(1);
        print_digitado("e tambem os olhos de uma coruja têm uma estrutura óssea como suporte, o que não permite que eles se virem.\n");
        sleep(1.5);
        print_digitado("por conta disso, elas conseguem virar a cabeça por 270 graus(135 para cada lado)\n");
        sleep(1.5);
        print_digitado("suprindo então a questão da não movimentação ocular...\n");
        sleep(1.5);
        print_digitado("Legau né?\n");
        sleep(1.5);
        print_digitado("no mais é so isso, ate a proxima\n...\n");
        sleep(2);
        print_digitado("Quer ouvir novamente?\n");
        scanf("%3s",&repetir);
         // esta função forçara a conversão do "sim" para minusculo caso precise
        for (int i = 0; repetir[i]; i++) {
            repetir[i] = tolower(repetir[i]);
        }
// Para que executea tarefa coloquei a mesma forma que esta no "if" porem dentro do "whilo"
       } while (strcmp(repetir,"sim")==0);
       
       print_digitado("\nTudo bem, ate a proxima!\n\n");

    }else{
        print_digitado("Tudo bem então...\n");
        print_digitado("Ate a proxima...\n\n");
    }
    
    return 0;
}