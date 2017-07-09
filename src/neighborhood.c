#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //inet_network()
#include <arpa/inet.h> //inet_network()

#include "neighborhood.h"
#include "msg.h"
#include "pilha.h"

typedef struct {
    struct in_addr sin_addr; //Utilizar inet_network() atribuindo o retorno para sin_addr.s_addr
    uint16_t port;
} Neighbor;

Pilha* neighborhood;

void die(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

void buildNeighborhood(char const *argv[], int begin, int n) {
    int i, j;

    neighborhood = novaPilha();

    for(i = begin; i < n; i++){
        //Extração do ip e da porta
        char s_ip[16]; //000.000.000.000\0
        char* s_port;
        for(j = 0; argv[i][j] != ':'; j++)
            s_ip[j] = argv[i][j];
        s_ip[j] = '\0';
        s_port = &argv[i][j+1];

        Neighbor* n = (Neighbor*) malloc(sizeof(Neighbor));
        n->port = htons(atoi(s_port));
        if((n->sin_addr.s_addr = inet_network(s_ip)) < 0)
            die("error: inet_network");

        inserePilha(neighborhood, n);
    }
}

void dispatch(int s, Msg_query* query, struct sockaddr_in* sin) {
    struct sockaddr_in sin_aux;
    Celula* aux = neighborhood;

    bzero((char*) &sin_aux, sizeof(struct sockaddr_in));
    sin_aux.sin_family = AF_INET;

    while((aux = proxPilha(aux)) != NULL){
        Neighbor* n = objPilha(aux);
        if(n->port != sin->sin_port && n->sin_addr.s_addr != sin->sin_addr.s_addr){
            sin_aux.sin_port = n->port;
            sin_aux.sin_addr = n->sin_addr;
            if((sendto(s, query, sizeof(query), 0, &sin_aux, sizeof(struct sockaddr_in))) < 0)
                die("error: sendto");
        }
    }
}

void destroyNeighborhood() {
    Celula* aux = neighborhood;
    puts("destroyNeighborhood");
    while((aux = proxPilha(aux)) != NULL){
        Neighbor* n = (Neighbor*) objPilha(aux);
        printf("<%s:%d>\n", inet_ntoa(n->sin_addr), n->port);
        free(objPilha(aux));
    }

    limpaPilha(neighborhood);
}