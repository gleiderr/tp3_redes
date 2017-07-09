#ifndef VIZINHANCA_H
#define VIZINHANCA_H

#include <netinet/in.h>
#include <stdint.h>

/* Constrói vizinhança a partir da lista de endereços [argv], iniciando no índice [begin], cujo [argv] contém [n] elementos. */
void buildNeighborhood(char const *argv[], int begin, int n);

/* Encaminha QUERY, para toda a [neighborhood], exceto para o vizinho de endereço [sin]. */
void dispatch(Msg_query* msg_query, struct sockaddr_in* sin);

#endif