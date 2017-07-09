#include <stdlib.h>

#include "pilha.h"

Pilha* novaPilha() {
    Pilha* pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->obj = NULL;
    pilha->prox = NULL;
    return pilha;
}

void limpaPilha(Pilha* pilha) {
    Pilha *aux = pilha;
    while(aux->prox != NULL){
        Pilha *ant = aux;
        aux = aux->prox;
        free(ant);
    }
    free(aux);
}

Celula* proxPilha(Celula* const celula) {
    return celula->prox;
}

void* objPilha(Celula* celula) {
    return celula->obj;
}

void setObj(Celula* celula, void* obj) {
    celula->obj = obj;
}

void inserePilha(Pilha* pilha, void* obj) {
    Celula* celula = (Celula*) malloc(sizeof(Celula));
    celula->obj = obj;
    celula->prox = pilha->prox;

    pilha->prox = celula;
}
