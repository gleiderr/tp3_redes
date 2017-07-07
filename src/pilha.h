#ifndef PILHA_H
#define PILHA_H

typedef struct node {
    void* obj;
    struct node* prox;
} Pilha;

Pilha* novaPilha();
void limpaPilha(Pilha* pilha);
Pilha* proxPilha(Pilha* pilha);

#endif