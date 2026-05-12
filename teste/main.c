#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct estudante{
    char nome[15];
    char prontuario[5];
    float media;
};

int ordenaPorNome(const void* p1, const void* p2){
    char* nome1 = ((struct estudante*) p1)->nome;
    char* nome2 = ((struct estudante*) p2)->nome;
    return strcmp(nome1, nome2);
}

int ordenaPorProntuario(const void* p1, const void* p2){
    char* pront1 = ((struct estudante*) p1)->prontuario;
    char* pront2 = ((struct estudante*) p2)->prontuario;
    return strcmp(pront1, pront2);
}

int ordenaPorMediaCresc(const void* p1, const void* p2){
    struct estudante *e1 = (struct estudante*)p1;
    struct estudante *e2 = (struct estudante*)p2;

    return (e1->media <= e2->media?-1:+1);
}

int ordenaPorMediaDecresc(const void* p1, const void* p2){
    struct estudante *e1 = (struct estudante*)p1;
    struct estudante *e2 = (struct estudante*)p2;

    return (e1->media <= e2->media?+1:-1);
}

void listaAlunos(struct estudante alunos[], int size){
    for(int i=0;i<size;i++)
        printf("Indice = %2d | Nome = %15s | Prontuario = %5s | Media = %2.2f\n", i, alunos[i].nome, alunos[i].prontuario, alunos[i].media);
}

int main(){
    struct estudante est[] = {{"Pedro","012", 6.2f}, {"Ana","125", 6.0f}, {"Renata","234", 8.9f}, {"Carlos","001", 10.0f}};

    printf("Estudantes nao ordenados\n");
    listaAlunos(est,4);

    printf("\nEstudantes Ordenados por ordem alfabetica\n");
    qsort(est, 4, sizeof(struct estudante), &ordenaPorNome);
    listaAlunos(est, 4);

    printf("\nEstudantes Ordenados por prontuario\n");
    qsort(est, 4, sizeof(struct estudante), &ordenaPorProntuario);
    listaAlunos(est, 4);

    printf("\nEstudantes Ordenados por media crescente\n");
    qsort(est, 4, sizeof(struct estudante), &ordenaPorMediaCresc);
    listaAlunos(est, 4);

    printf("\nEstudantes Ordenados por media decrescente\n");
    qsort(est, 4, sizeof(struct estudante), &ordenaPorMediaDecresc);
    listaAlunos(est, 4);
    return 0;
}
