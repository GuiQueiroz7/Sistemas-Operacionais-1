#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM 5 //Tamanho do buffer
#define ITENS 10 //Total de itens para produzir/consumir

int buffer[TAM];
int p = 0;//Indice do produtor
int c = 0;//Indice do consumidor

sem_t vazio;
sem_t cheio;
pthread_mutex_t mut;

void* produtor(void* arg){
    for(int i=0; i< ITENS;i++){
        int item = i + 1;//Produz um item
        sem_wait(&vazio);//Funcao P do semaforo
        pthread_mutex_lock(&mut);

        buffer[p] = item;
        printf("Produtor inseriu %d na pos: %d\n", item, p);
        p = (p+1)%TAM;

        pthread_mutex_unlock(&mut);
        sem_post(&cheio);
        usleep(100000);//Atraso para melhor vizualizacao do semaforo
    }
    return NULL;
}

void* consumidor(void* arg){
    for(int i=0; i< ITENS;i++){
        sem_wait(&cheio);//Funcao P do semaforo
        pthread_mutex_lock(&mut);

        int item = buffer[c];
        printf("Consumidor consumiu %d na pos: %d\n", item, c);
        c = (c+1)%TAM;

        pthread_mutex_unlock(&mut);
        sem_post(&vazio);
        usleep(100000);//Atraso para melhor vizualizacao do semaforo
    }
    return NULL;
}

int main(){
    pthread_t t_prod, t_cons;

    //Inicializar semaforos e mutex
    sem_init(&vazio, 0, TAM);//Vazio comecao com tamanho TAM
    sem_init(&cheio, 0, 0);//Cheio inicializa sem tamanho
    pthread_mutex_init(&mut, NULL);

    //Criando threads de produzir e consumir itens
    pthread_create(&t_prod, NULL, produtor, NULL);
    pthread_create(&t_cons, NULL, consumidor, NULL);

    //Inicializa as threads de produzir e consumir
    pthread_join(t_prod, NULL);
    pthread_join(t_cons, NULL);

    //Destroi os semaforos e mutex -> Semelhante ao free() para ponteiros
    sem_destroy(&vazio);
    sem_destroy(&cheio);
    pthread_mutex_destroy(&mut);

    printf("\nProcesso finalizado com sucesso.\n");
    return 0;
}
