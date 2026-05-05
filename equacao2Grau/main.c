//ACHANDO OS VALORES DAS RAIZES DE UMA FUNCAO POLINOMIAL DE 2 GRAU POR MEIO DE BHASKARA
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> //Implementacao da biblioteca de Fiber

typedef struct{
    double a, b, c;
    double delta;
    double sqrtDelta;
    double x1, x2;
    LPVOID mainFiber;
    LPVOID deltaFiber;
    LPVOID sqrtFiber;
    LPVOID x1Fiber;
    LPVOID x2Fiber;
}elemEquacao;//Struct que armazena os valores referentes a equeçao e sua resolucao

//Fiber que calcula o delta e chama sqrtFiber
LPVOID WINAPI DeltaFiber(LPVOID parametro){
    elemEquacao* fx = (elemEquacao*)parametro;
    printf("[DeltaFiber] Calculando Delta...\n");
    fx->delta = (fx->b*fx->b)-(4*fx->a*fx->c);
    if(fx->delta<0){
        printf("[DeltaFiber] Delta menor que zero, nao ha raizes em f(x)\n");
        SwitchToFiber(fx->mainFiber);
    }
    SwitchToFiber(fx->sqrtFiber);
}

//Fiber que calcula sqrt(delta) e chama x1Fiber
LPVOID WINAPI SqrtFiber(LPVOID parametro){
    elemEquacao* fx = (elemEquacao*)parametro;
    printf("[SqrtFiber] Calculando da Raiz Quadrada de Delta...\n");
    fx->sqrtDelta = sqrt(fx->delta);
    SwitchToFiber(fx->x1Fiber);
}

//Fiber que calcula o valor de X1 e chama a X2Fiber
LPVOID WINAPI X1Fiber(LPVOID parametro){
    elemEquacao* fx = (elemEquacao*)parametro;
    printf("[X1Fiber] Calculando o valor de x1...\n");
    fx->x1 = (-(fx->b) + fx->sqrtDelta)/(2 * fx->a);
    SwitchToFiber(fx->x2Fiber);
}

//Fiber que calcula o valor de X2 e retorna a execucao do programa para MainFiber
LPVOID WINAPI X2Fiber(LPVOID parametro){
    elemEquacao* fx = (elemEquacao*)parametro;
    printf("[X2Fiber] Calculando o valor de x2...\n");
    fx->x2 = (-(fx->b) - (fx->sqrtDelta))/(2 * fx->a);
    SwitchToFiber(fx->mainFiber);
}

int main(){
    LPVOID mainFiber = ConvertThreadToFiber(NULL);
    LPVOID fDelta;
    elemEquacao fx;

    //Trata caso a fiber nao seja inicializada corretamente
    if(!mainFiber){
        printf("Erro ao inicializar o processo de fibra\n");
        return 1;
    }

    //Declaracao direta
    /*
    fx.a = 1;
    fx.b = -5;
    fx.c = 6;
    */

    //Atribuicao dos valores via usuário
    printf("Atribuindo os valores da funcao de 2 grau que representam A, B e C:\n");
    printf("Insira o valor de A: ");
    scanf("%lf", &fx.a);
    printf("Insira o valor de B: ");
    scanf("%lf", &fx.b);
    printf("Insira o valor de C: ");
    scanf("%lf", &fx.c);


    fx.mainFiber = mainFiber;
    fx.deltaFiber = CreateFiber(0, DeltaFiber, &fx);
    fx.sqrtFiber = CreateFiber(0, SqrtFiber, &fx);
    fx.x1Fiber = CreateFiber(0, X1Fiber, &fx);
    fx.x2Fiber = CreateFiber(0, X2Fiber, &fx);


    printf("\nEncontrando as raizes de f(x) = %.0fx^2 ", fx.a);
    //Tratando os valores de B
    if(fx.b>0)
        printf("+ %.0fx ", fx.b);
    else if(fx.b<0)
        printf("%.0fx ", fx.b);

    //Tratando os valores de C
    if(fx.c>0)
        printf(" + %.0f\n", fx.c);
    else if(fx.c<0)
        printf(" %.0f\n", fx.c);
    else
        printf("\n");

    SwitchToFiber(fx.deltaFiber);

    if(fx.delta >= 0){
        printf("\nValores das Raizes:\n");
        printf("X1 = %.2f\n", fx.x1);
        printf("X2 = %.2f\n", fx.x2);
    }

    DeleteFiber(fDelta);
    DeleteFiber(fx.sqrtFiber);
    DeleteFiber(fx.x1Fiber);
    DeleteFiber(fx.x2Fiber);

    return 0;
}
