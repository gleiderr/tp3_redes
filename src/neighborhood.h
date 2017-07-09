#ifndef VIZINHANCA_H
#define VIZINHANCA_H

#include "msg.h"

typedef struct {
    struct in_addr sin_addr; //Utilizar inet_network() atribuindo o retorno para sin_addr.s_addr
    uint16_t port;
} Neighbor;

typedef struct {
    int count;
    Neighbor neighbors[256];
} Neighborhood;

/* Constrói vizinhança a partir da lista de endereços [argv], iniciando no índice [begin], cujo [argv] contém [n] elementos. */
void buildNeighborhood(char const *argv[], int begin, int n, Neighborhood* neighborhood);

void dispatch(Msg_query* msg_query, Neighborhood* neighborhood, struct sockaddr_in* sin);

#endif