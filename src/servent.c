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

int openSocket(char* porto) {
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

int main(int argc, char const *argv[]) {
    int i;
    uint32_t seq = 0;
    struct sockaddr_in sin;
    socklen_t sin_len = 0;

    buildDictionary(argv[2]);
    buildNeighborhood(argv, 3, argc);

    /*int s = openSocket(argv[1]); //Descritor do Socket
    
    //Estruturas 
    Msg_generica msg_generica;
    Msg_query msg_query;
    Msg_clireq* msg_clireq;

    while(1) {

        if (recvfrom(s, &msg_generica, sizeof(msg_generica), 0, &sin, &sin_len) < 0)
            die("error: recvfrom");

        switch(ntohs(msg_generica.type)) {
            case CLIREQ:
                msg_clireq = (Msg_clireq*) &msg_generica;

                msg_query.type = htons(QUERY);
                msg_query.ttl = htons(3);
                msg_query.sin_addr = sin.sin_addr; //Não uso htons. À priori já chegou formatado
                msg_query.port = sin.sin_port; //Não uso htons. À priori já chegou formatado
                msg_query.seq = seq++;
                strcpy(msg_query.chave, msg_clireq->chave);

                insertQueryMemory(&msg_query);

                //Encaminhar para vizinhança
                dispatch(&msg_query, NULL);

                if(inDictionary(msg_query.chave)) {
                    //Responder diretamente ao client
                }
                break;
            case QUERY:
                if(!inQueryMemory(&msg_generica)){
                    insertQueryMemory(&msg_generica);

                    if(inDictionary(msg_generica.chave)) {
                        //Responder diretamente ao client
                    }

                    //Decrementando no byte-order do host
                    msg_generica.ttl = ntohs(msg_generica.ttl) - 1; 
                    if(msg_query.ttl > 0) {
                        //Retornando para o byte-order da network
                        msg_query.ttl = htons(msg_query.ttl);
                        
                        //Encaminhar para vizinhança exceto para nó do qual a mensagem foi recebida
                        dispatch(&msg_query, &sin);
                    }
                }
                break;
            default:
                die("error: switch");
        }
    }*/

    destroyNeighborhood();
    destroyDictionary();
    exit(EXIT_SUCCESS);
}