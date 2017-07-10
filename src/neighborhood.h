#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include <netinet/in.h>
#include <stdint.h>

#include "msg.h"

/* Constrói vizinhança a partir da lista de endereços [argv], iniciando no índice [begin], cujo [argv] contém [n] elementos. */
void buildNeighborhood(char const *argv[], int begin, int n);

/* Encaminha QUERY, para toda a [neighborhood], exceto para o vizinho de endereço [sin]. */
void dispatch(int s, Msg_query* query, struct sockaddr_in* sin);

/* Libera memória ocupada pela vizinhança */
void destroyNeighborhood();

#endif