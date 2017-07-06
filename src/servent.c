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

typedef struct {
    char ip[16]; //000.000.000.000/0
    int port;
} Vizinho;

typedef struct {
    int count;
    Vizinhança vizinhanca[256];
} Vizinhanca;

void die(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
    int s; //Descritor do Socket 
    struct sockaddr_in sin;

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        die("error: socket");
    }

    int port = atoi(argv[1]); //Lendo e convertendo o Porto de Escuta
    bzero((char*) sin, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = htonl(INADDR_ANY);
    sin->sin_port = htons(port);
    if(bind(s, (struct sockaddr*) sin, sizeof(struct sockaddr_in)) < 0) {
        close(s);
        die("error: bind");
    }

    exit(EXIT_SUCCESS);
}