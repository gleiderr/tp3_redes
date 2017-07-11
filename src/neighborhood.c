#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //inet_network()
#include <arpa/inet.h> //inet_network()
#include <strings.h> //bzero()

#include "neighborhood.h"
#include "pilha.h"
#include "util.h"

typedef struct {
    struct in_addr sin_addr; //Utilizar inet_network() atribuindo o retorno para sin_addr.s_addr
    uint16_t port;
} Neighbor;

Pilha* neighborhood;

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
        if(inet_aton(s_ip, &n->sin_addr) < 0)
            die("error: inet_network");

        inserePilha(neighborhood, n);
    }
}

void dispatch(int s, Msg_query* query, struct sockaddr_in* except) {
    //puts("dispatch_in()");
    struct sockaddr_in sin_aux;
    Celula* aux = neighborhood;

    bzero((char*) &sin_aux, sizeof(struct sockaddr_in));
    sin_aux.sin_family = AF_INET;

    int send;
    while((aux = proxPilha(aux)) != NULL){
        Neighbor* n = objPilha(aux);
        if(except == NULL) //Se não houver excessão de vizinho
            send = 1;
        else if(n->port != except->sin_port || n->sin_addr.s_addr != except->sin_addr.s_addr)
            send = 1;

        if(send) {
            sin_aux.sin_port = n->port;
            sin_aux.sin_addr = n->sin_addr;
            //printf("sendto(%s:%d)\n", inet_ntoa(sin_aux.sin_addr), sin_aux.sin_port);
            if((sendto(s, query, sizeof(query), 0, (struct sockaddr*) &sin_aux, sizeof(sin_aux))) < 0)
                die("error: sendto");
        }
    }
    //puts("dispatch_out()");
}

void destroyNeighborhood() {
    Celula* aux = neighborhood;
    //puts("destroyNeighborhood");
    while((aux = proxPilha(aux)) != NULL){
        //Neighbor* n = (Neighbor*) objPilha(aux);
        //printf("<%s:%d>\n", inet_ntoa(n->sin_addr), ntohs(n->port));
        free(objPilha(aux));
    }

    limpaPilha(neighborhood);
}