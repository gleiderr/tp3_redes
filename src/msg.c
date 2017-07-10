#include <stdlib.h>

#include "msg.h"
#include "pilha.h"

Pilha* queryMemory;

/* Aloca memória para armazenar queries. */
void buildQueryMemory() {
    queryMemory = novaPilha();
}

/* Insere determinada query na memória */
void insertQueryMemory(Msg_query* msg) {
    Msg_query* m = (Msg_query*) malloc(sizeof(Msg_query));
    m->type = msg->type; //Sempre igual a QUERY
    m->ttl = msg->ttl;
    m->sin_addr = msg->sin_addr;
    m->port = msg->port;
    m->seq = msg->seq;
    strcpy(m->chave, msg->chave);

    inserePilha(queryMemory, m);
}

/* Verifica se determinada Msg_query está na memória 
   IP e porto do cliente, número de sequência, e chave */
int inQueryMemory(Msg_query* msg) {
    Celula* aux = queryMemory;
    while((aux = proxPilha(aux)) != NULL) {
        Msg_query* m = objPilha(aux);
        if(msg->sin_addr.s_addr != m->sin_addr.s_addr || 
           msg->port != m->port || msg->seq != m->seq) {
            if(strcmp(msg->chave, m->chave) != 0)
            return 1;
        }
    }
    return 0;
}

/* Libera memória utilizada para armazenar queries. */
void destroyQueryMemory() {
    Celula* aux = queryMemory;
    while((aux = proxPilha(aux)) != NULL)
        free(objPilha(aux));

    limpaPilha(queryMemory);
}