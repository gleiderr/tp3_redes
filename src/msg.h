#ifndef MSG_H
#define MSG_H

#include <netinet/in.h>
#include <stdint.h>

#define CLIREQ 1
#define QUERY 2
#define RESPONSE 3

#define MAX_CHAVE 41
#define MAX_VALOR 161

typedef struct {
    uint16_t type; //Sempre igual a QUERY
    uint16_t ttl;
    struct in_addr sin_addr;
    uint16_t port;
    uint32_t seq;
    char chave[MAX_CHAVE];
} Msg_query;

typedef struct {
    uint16_t type; //Sempre igual a CLIREQ
    char chave[MAX_CHAVE];
} Msg_clireq;

typedef struct {
    uint16_t type;
    char chave_valor[MAX_CHAVE + MAX_VALOR]; // =202, por causa do \t e do \0
} Msg_response;

#define Msg_generica Msg_query

/* Verifica se determinada Msg_query está na memória */
int inQueryMemory(Msg_query* msg);

/* Insere determinada query na memória */
void insertQueryMemory(Msg_query* msg);

#endif