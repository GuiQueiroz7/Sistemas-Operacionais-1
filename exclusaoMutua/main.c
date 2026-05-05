//EXEMPLO SIMPLES DE EXCLUSAO MUTUA
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h> //Biblioteca que permite o uso de comandos do UNIX

#define N 1000000
int c=0;

atomic_flag trava = ATOMIC_FLAG_INIT; //Variavel de trava -> Inicia destravada

void* incrementar(void* arg){
    for(int i=0; i<N;i++){//Processo que entra na Regiao Critica (Busy Waiting)
        //Enquanto a RC estiver ocupada, o while continua rodando
        while(atomic_flag_test_and_set(&trava)){

        }
        c++; //Procedimento que entra na RC quando ela estiver livre
        atomic_flag_clear(&trava);//Passa que a RC esta livre de procedimentos
    }
    return NULL;
}

//Decrementar e a mesma funcao de incrementar, porem ela remove 1 na variavel c
void* decrementar(void* arg){
    for(int i=0; i<N;i++){
        while(atomic_flag_test_and_set(&trava)){
        }
        c--;
        atomic_flag_clear(&trava);
    }
    return NULL;
}

int main(){
    pthread_t threadIncre, threadDecre; //Cria as threads de Incrementacao e Decrementacao

    //Atribui as threads as funcoes do comeco do codigo
    pthread_create(&threadIncre, NULL, incrementar, NULL);
    pthread_create(&threadDecre, NULL, decrementar, NULL);

    //Inicializa a thread de Incrementacao e apos a de Decrementacao
    pthread_join(threadIncre, NULL);
    pthread_join(threadDecre, NULL);

    printf("Valor de C apos decrementar\nDeve ser igual a 0\nAtualmente o valor de C e: %d\n", c);

    return 0;
}
