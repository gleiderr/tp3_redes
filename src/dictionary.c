#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"
#include "pilha.h"
#include "msg.h"

#define MAX_BUFF 1024

typedef struct {
    char chave[MAX_CHAVE];
    char valor[MAX_VALOR];
} Par;

Pilha* dicionario;

void buildDictionary(char* const fname) {
    int i;
    dicionario = novaPilha();
    FILE* f = fopen(fname, "r");
    char buff[MAX_BUFF];

    while(!feof(f)) {
        fgets(buff, MAX_BUFF, f);

        //Ignorando espaços em branco iniciais
        char* aux = buff;
        while(aux[0] == ' ') aux = &aux[1];

        if(aux[0] != '#') { //Se linha não for comentário:
            Par* par = (Par*) malloc(sizeof(Par));
            sscanf(aux, "%s%[^\f\n]", par->chave, par->valor);

            //Removendo brancos finais
            i = strlen(par->valor) - 1;
            while(i > 0  && par->valor[i] == ' ') {
                par->valor[i] = '\0';
                i--;
            }

            inserePilha(dicionario, par);
        } //else ignora e passa para próxima linha
    }

    fclose(f);
}

char* inDictionary(char* const chave) {
    Celula* aux = dicionario;
    while(aux = proxPilha(aux) != NULL)
        if(strcmp(objPilha(aux)->chave, chave) == 0)
            return objPilha(aux)->valor;
    return NULL;
}

void destroyDictionary() {
    Celula* aux = dicionario;
    while(aux = proxPilha(aux) != NULL)
        free(objPilha(aux));

    limpaPilha(dicionario);
}