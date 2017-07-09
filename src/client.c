#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "msg.h"

int openClient(char const* addr) {
    int i, sock;
    int server_port;
    struct sockaddr_in sin;
	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 4;
	
    bzero((char*) &sin, sizeof(sin));
    
    /* Abrindo Socket */
    if (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) < 0) {
        perror("error: socket");
        return 0;
    }

    /* Extraindo ip e porto do argumento recebido. */
    char host[15+1]; //000.000.000.000\0
    for(i = 0; addr[i] != ':'; i++)
        host[i] = addr[i];
    host[i] = '\0';
    char temPort[4+1];
    for(; addr[i] != '\0'; i++)
        temPort[i] = addr[i];
    server_port = atoi(temPort); //Lendo e convertendo a porta do servidor

    printf("Servent IP:%s, PORT:%i\n", host, server_port);

    memset((char *) &sin, 0, sizeof(sin));
    sin.sin_port = htons(server_port);
    sin.sin_family = AF_INET;

    if (inet_aton(host , sin.sin_addr) == 0) 
    {
    	perror("error: inet_aton");
        return 0;
    }

    /* Coloca o timer no */
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout)) < 0) {
	    perror("error: timeout");
	}
   
    return sock;
}

void sendMSG(int s, char* key, struct sockaddr_in* si_other, int sLen) {
    Msg_clireq msgReq; // Mensagem que será enviada ao servent referencia quando uma chave for requisitada.
    msgReq.type = CLIREQ;
    memcpy(msgReq.chave, key, strlen(key));
    
    if (sendto(s, &msgReq, sizeof(Msg_clireq) , 0 , (struct sockaddr *) si_other, sLen) == -1)
    {
   	    perror("error: sendMSG");
        close(s);
        exit(1);
    }
}

/**
 * Função principal, abre a conexão com o servent referencia da rede, por argumento são recebidos o IP e PORT. Então ele entre em loop e fica esperando
 * uma chave do usuário para enviar uma requisição, e fica esperando por 4 segundos por uma resposta se não a obtiver ele reenvia a requisição. 
 * Caso a resposta chegue 
 */
int main(int argc, char const *argv[]) {
    
    struct sockaddr_in sin;
    int sock, sLen = sizeof(sin);
    
    /* Abre conexão UDP com servent referencia. */
    if(!(sock = openClient(argv[1], &sin)))
        exit(-1);
    printf("Socket opened!\n");

    printf("Esse é um programa simples de troca de chaves-valores em um Sistema ​Peer-to-peer.\n
    		Digite o valor de uma chave, ou digite sair par finalizar a aplicação. \n", );
    /* Rotina principal, recebe o valor*/
    int waitingForKey = 1, endClient = 0;
    char key[MAX_CHAVE];
    while(!endClient) {
        if(waitingForKey){
        	printf("Chave: ");
        	gets(key);

        	
        }
    }
    
    close(s);
    exit(0);
}