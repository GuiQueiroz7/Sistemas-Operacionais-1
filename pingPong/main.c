//MODELO DE IMPLEMENTACAO UTILIZANDO MULTI THREADS COM FIBERS - PING PONG
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //Implementacao do Fiber

//Declaro as fibers como Global para a tarefaA e tarefaB conseguirem identificar todas as fibers
LPVOID fiberA;
LPVOID fiberB;
LPVOID mainFiber;

VOID WINAPI tarefaA(LPVOID parametro){
    for(int i=0; i<=3;i++){
        printf("A fez a sua jogada %d, passando para B\n", i);
        SwitchToFiber(fiberB);//Troca a execucao da fiberA para a fiberB
    }
    printf("A - finalizou o jogo.Hora de voltar para main\n");
    SwitchToFiber(mainFiber);//Troca a execucao da fiberB para a mainFiber
}

VOID WINAPI tarefaB(LPVOID parametro){
    for(int i=0; i<=3;i++){
        printf("B fez a sua jogada %d, passando para A\n", i);
        SwitchToFiber(fiberA);
    }
}

int main(){
    mainFiber = ConvertThreadToFiber(NULL);

    //Trata caso a mainFiber nao foi inicializada corretamente
    if(mainFiber == NULL){
        printf("Erro ao inicializar o processo de fibra\n");
        return 1;
    }

    //Inicializo as Fibers ja atribuindo uma funcao a elas
    fiberA = CreateFiber(0, tarefaA, NULL);
    fiberB = CreateFiber(0, tarefaB, NULL);

    printf("Iniciando processo de ping-pong A/B\n");
    SwitchToFiber(fiberA);//Troca a execucao da mainFiber para a fiberA
    printf("Fim do jogo A/B");

    //Removo as Fibers da memória como boa prática
    DeleteFiber(fiberA);
    DeleteFiber(fiberB);

    return 0;
}
