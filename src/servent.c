#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "util.h"
#include "msg.h"
#include "dictionary.h"
#include "neighborhood.h"

#define MAX_BUFF 512
#define TIMEOUT_SEC 4
#define TIMEOUT_uSEC 0

int openSocket(const char* porto) {
    int s; //Descritor do Socket
    struct sockaddr_in sin;

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        die("error: socket");

    uint16_t port = atoi(porto); //Lendo e convertendo o Porto de Escuta
    bzero((char*) &sin, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    if(bind(s, (struct sockaddr*) &sin, sizeof(struct sockaddr_in)) < 0) {
        close(s);
        die("error: bind");
    }

    //Configurando timeout
    /*struct timeval time_str;
    time_str.tv_sec = TIMEOUT_SEC;
    time_str.tv_usec = TIMEOUT_uSEC;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &time_str, sizeof(time_str));*/

    return s;
}

/* Encaminha mensagem de RESPONSE ao client que fez a requisição, se valor for encontrado no dicionário. */
void clientResponse(int s, Msg_query* query) {
    char* valor;
    if((valor = valorInDictionary(query->chave)) != NULL) {
        //Responder diretamente ao client

        struct sockaddr_in sin;
        bzero((char*) &sin, sizeof(struct sockaddr_in));
        sin.sin_family = AF_INET;
        sin.sin_addr = query->sin_addr;
        sin.sin_port = query->port;
        
        Msg_response response;
        response.type = RESPONSE;
        
        strcpy(response.chave_valor, query->chave);
        strcat(response.chave_valor, "\t");
        strcat(response.chave_valor, valor);
        
        if((sendto(s, &response, sizeof(response), 0, (struct sockaddr*) &sin, sizeof(sin))) < 0)
            die("error: in clientResponse(): sendto");
    }
}

int main(int argc, char const *argv[]) {
    uint32_t seq = 0;
    struct sockaddr_in sin;
    socklen_t sin_len = 0;

    buildDictionary(argv[2]);
    buildNeighborhood(argv, 3, argc);
    void buildQueryMemory();

    int s = openSocket(argv[1]); //Descritor do Socket
    puts("Socket aberto!");
    
    //Estruturas 
    Msg_generica msg_generica;
    Msg_query msg_query;
    Msg_clireq* msg_clireq;

    while(1) {

        if (recvfrom(s, &msg_generica, sizeof(msg_generica), 0, (struct sockaddr*) &sin, &sin_len) < 0)
            die("error: recvfrom");

        switch(ntohs(msg_generica.type)) {
            case CLIREQ:
                msg_clireq = (Msg_clireq*) &msg_generica;

                msg_query.type = htons(QUERY);
                msg_query.ttl = htons(3);
                msg_query.sin_addr = sin.sin_addr; //Não uso htons. À priori já chegou formatado
                msg_query.port = sin.sin_port; //Não uso htons. À priori já chegou formatado
                msg_query.seq = htonl(seq++);
                strcpy(msg_query.chave, msg_clireq->chave);
                fprintf(stderr, "case CLIREQ: %s\n", msg_query.chave);

                insertQueryMemory(&msg_query);

                dispatch(s, &msg_query, NULL); //Encaminhar para vizinhança

                clientResponse(s, &msg_query);
                break;
            case QUERY:
                puts("case QUERY:");
                if(!inQueryMemory(&msg_generica)){
                    insertQueryMemory(&msg_generica);

                    clientResponse(s, &msg_generica);

                    //Decrementando no byte-order do host
                    msg_generica.ttl = ntohs(msg_generica.ttl) - 1; 
                    if(msg_generica.ttl > 0) {
                        //Retornando para o byte-order da network
                        msg_generica.ttl = htons(msg_generica.ttl);
                        
                        //Encaminhar para vizinhança exceto para nó do qual a mensagem foi recebida
                        dispatch(s, &msg_generica, &sin);
                    }
                }
                break;
            default:
                die("error: switch");
        }
    }

    destroyNeighborhood();
    destroyDictionary();
    void destroyQueryMemory();
    exit(EXIT_SUCCESS);
}