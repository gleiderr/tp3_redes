#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define MAX_BUFF 512
#define MAX_MSG 202
#define TIMEOUT_SEC 4
#define TIMEOUT_uSEC 0

#define CLIREQ 1
#define QUERY 2
#define RESPONSE 3

typedef struct {
    struct in_addr sin_addr; //Utilizar inet_network() atribuindo o retorno para sin_addr.s_addr
    uint16_t port;
} Vizinho;

typedef struct {
    int count;
    Vizinhança vizinhos[256];
} Vizinhanca;

typedef struct {
    uint16_t type;
    uint16_t ttl;
    struct in_addr sin_addr;
    uint16_t port;
    uint32_t seq;
    char msg[MAX_MSG];
} serventMsg;

void die(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    int i;
    int s; //Descritor do Socket 
    struct sockaddr_in sin;
    socklen_t sin_len = sizeof(sockaddr_in);
    Vizinhanca vizinhanca;

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        die("error: socket");
    }

    int port = atoi(argv[1]); //Lendo e convertendo o Porto de Escuta
    bzero((char*) &sin, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    if(bind(s, (struct sockaddr*) &sin, sin_len) < 0) {
        close(s);
        die("error: bind");
    }

    //Configurando timeout
    struct timeval time_str;
    time_str.tv_sec = TIMEOUT_SEC;
    time_str.tv_usec = TIMEOUT_uSEC;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &time_str, sizeof(time_str));

    //Leitura do arquivo
    //Não implementada
    /*char buff[MAX_BUFF];
    while(!feof(stdin)) {
        fgets(buff, MAX_BUFF, stdin);
    }*/

    serventMsg msg;
    srand(port); //Porto como Semente, considerando que cada programa operará em um porto diferente
    while(1) {
        if (recv(s, &msg, sizeof(serventMsg), 0, &sin, &sin_len) < 0)
            die("error: recvfrom");

        switch(ntohs(msg.type)) {
            case CLIREQ:
                msg.type = htons(QUERY);
                msg.ttl = htons(3);
                msg.sin_addr = sin.sin_addr; //Não utilizo htons, pois à priori já chegou formatado
                msg.port = sin.sin_port; //Não utilizo htons, pois à priori já chegou formatado
                msg.seq = htonl(rand());
                //msg.msg; não atribuido porque será enviada assim como recebida

                insertInMemory(&msg);

                //Encaminhar para vizinhança
                for(i = 0; i < vizinhanca.count; i++) {
                    sin.sin_addr = vizinhanca.vizinhos[i].sin_addr;
                    sin.sin_port = vizinhanca.vizinhos[i].port;
                    if (sendto(s, &msg, sizeof(serventMsg), 0, (struct sockaddr*) &sin, slen) < 0)
                        die("error: sendto");
                }

                //procurar a chave no dicionário local e responder ao cliente se chave encontrada
                if(inDictionary(msg.msg)) {
                    //Responder diretamente ao client
                }
                break;
            case QUERY:
                if(!inMemory(&msg)){
                    insertInMemory(&msg);
                    //procurar a chave no dicionário local e responder ao cliente se chave encontrada

                    msg.ttl = ntohs(msg.ttl) - 1; //Decrementando no byte-order do host
                    if(msg.ttl > 0) {
                        msg.ttl = htons(msg.ttl); //Retornando para o byte-order da network
                        
                        //Encaminhar para vizinhança exceto para aquele do qual a mensagem foi recebida
                    }
                }
                break;
            default:
                die("error: switch");
        }
    }

    exit(EXIT_SUCCESS);
}