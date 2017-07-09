#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "msg.h"

#define MAX_BUFF 1024

typedef struct {
    char chave[MAX_CHAVE];
    char valor[MAX_VALOR];
} Par;

Pilha* dicionario;

/* Remove brancos finais e iniciais */
void strip(char* s) {
    int i;
    char* aux = s;
    
    //Ignorando espaços em branco iniciais    
    while(aux[0] == ' ' || aux[0] == '\t') aux = &aux[1];

    //Ignorando brancos finais
    for(i = strlen(aux) - 1; i >= 0 && (aux[i] == ' ' || aux[i] == '\t'); i--)
        aux[i] = '\0';

    strcpy(s, aux);
}

void buildDictionary(char const* fname) {
    dicionario = novaPilha();
    FILE* f = fopen(fname, "r");
    char buff[MAX_BUFF];

    while(!feof(f)) {
        fgets(buff, MAX_BUFF, f);

        strip(buff);

        if(buff[0] != '#') { //Se linha não for comentário:
            Par* par = (Par*) malloc(sizeof(Par));
            sscanf(buff, "%s%[^\f\n]", par->chave, par->valor);

            strip(par->valor);

            Celula* aux;
            if((aux = inDictionary(par->chave)) != NULL){
                //Chaves repetidas
                free(objPilha(aux));
                setObj(aux, par);
            } else if(strlen(par->chave) > 0 || strlen(par->valor) > 0) {
                //somente valores/chaves não vazios
                inserePilha(dicionario, par);
            }
        } //else ignora e passa para próxima linha
    }

    fclose(f);
}

Celula* inDictionary(char const* chave) {
    Celula* aux = dicionario;
    while((aux = proxPilha(aux)) != NULL){
        Par* par = (Par*) objPilha(aux); 
        if(strcmp(par->chave, chave) == 0)
            return aux;
    }
    return NULL;
}

void destroyDictionary() {
    Celula* aux = dicionario;
    //puts("destroyDictionary");
    while((aux = proxPilha(aux)) != NULL){
        /*Par* par = (Par*) objPilha(aux);
        printf("<%s><%s>\n", par->chave, par->valor);*/
        free(objPilha(aux));
    }

    limpaPilha(dicionario);
}