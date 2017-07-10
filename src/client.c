#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "msg.h"

int openClient(char const* addr, struct sockaddr_in* sin) {
    int i, sock;
    int server_port;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 4;
	
    /* Abrindo Socket */
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("error: socket");
        return 0;
    }

    /* Extraindo ip e porto do argumento recebido. */
    char host[15+1]; //000.000.000.000\0
    for(i = 0; addr[i] != ':'; i++)
        host[i] = addr[i];
    host[i] = '\0';

    /*for(; addr[i] != '\0'; i++)
        temPort[i] = addr[i];*/
    //Garantido port igual a escrita no addr, independente do tamalho e já garantindo \0 ao final.
    char* temPort = &addr[i+1]; 
    server_port = atoi(temPort); //Lendo e convertendo a porta do servidor

    printf("Servent IP:%s, PORT:%i\n", host, server_port);

    bzero((char*) sin, sizeof(&sin));
    sin->sin_port = htons(server_port);
    sin->sin_family = AF_INET;

    if (inet_aton(host , (struct in_addr *) &(sin->sin_addr)) == 0) 
    {
    	perror("error: inet_aton");
        return 0;
    }

    /* Coloca o timer no */
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof(timeout)) < 0) {
	    perror("error: timeout");
        return 0;
	}
   
    return sock;
}

void sendMSG(int sock, char* key, struct sockaddr_in* siServer) {
    Msg_clireq msgReq; // Mensagem que será enviada ao servent referencia quando uma chave for requisitada.
    msgReq.type = htons(CLIREQ);
    memcpy(msgReq.chave, key, strlen(key));
    
    if (sendto(sock, &msgReq, sizeof(Msg_clireq) , 0 , (struct sockaddr *) siServer, sizeof(struct sockaddr_in)) == -1)
    {
   	    perror("error: sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

/**
 * Função principal, abre a conexão com o servent referencia da rede, por argumento são recebidos o IP e PORT. Então ele entre em loop e fica esperando
 * uma chave do usuário para enviar uma requisição, e fica esperando por 4 segundos por uma resposta se não a obtiver ele reenvia a requisição. 
 * Caso a resposta chegue 
 */
int main(int argc, char const *argv[]) {
    Msg_response respMsg;
    struct sockaddr_in sout;
    int sock;
    socklen_t sOutLen = sizeof(sout);
    
    /* Abre conexão UDP com servent referencia. */
    if(!(sock = openClient(argv[1], &sout)))
        exit(EXIT_FAILURE);
    printf("Socket opened!\n");

    printf("Esse é um programa simples de troca de chaves-valores em um Sistema ​Peer-to-peer.\n Digite o valor de uma chave, ou digite sair para finalizar a aplicação. \n");
    /* Rotina principal, recebe o valor*/
    int waitingForKey = 1, endClient = 0, ntimeouts = 0;
    char key[MAX_CHAVE];
    while(!endClient) 
    {
        if(waitingForKey)
        {
        	printf("Chave: ");
        	if (fgets(key, sizeof(key), stdin)) {
                key[strcspn(key, "\n")] = '\0';
                if(strcmp(key, "sair") == 0)
                    endClient = 1;
                waitingForKey = 0;         
            }
        	else printf("Digite dentro do limite de 41 caracteres. \n");
        }
        else {
            sendMSG(sock, key, &sout);
        	if(recvfrom(sock, &respMsg ,sizeof(respMsg), 0, (struct sockaddr *) &sout, &sOutLen) < 0)
            {
    			//timeout reached
    			printf("Timout reached. Resending segment %d\n", ntimeouts);
    			ntimeouts++;
			}
            else 
            {
                if(ntohs(respMsg.type) == RESPONSE)
                {
                    printf("%s\n", respMsg.chave_valor);
                    ntimeouts = 0;
                    waitingForKey = 1;
                }
                else 
                    printf("Resposta com tipo errado: %d\n", ntohs(respMsg.type));
            }
        }

    }
    
    close(sock);
    exit(EXIT_SUCCESS);
}