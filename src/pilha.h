#ifndef PILHA_H
#define PILHA_H

typedef struct node {
    void* obj;
    struct node* prox;
} Pilha;

typedef Pilha Celula;

/* Cria cabeça da pilha */
Pilha* novaPilha();

/* Remove todas as células da pilha. Não se importa com objetos. */
void limpaPilha(Pilha* pilha);

/* Retorna próximo objeto apontado pela célula da pilha. */
Celula* proxPilha(Celula* const celula);

/* Insere objeto na pilha. Alocação do objeto é responsabilidade do usuário da pilha. */
void inserePilha(Pilha* pilha, void* obj);

/* Retorna objeto armazenado na célula. */
void* objPilha(Celula* celula);

/* Atribui objeto à celula da pilha*/
void setObj(Celula* celula, void* obj);

#endif