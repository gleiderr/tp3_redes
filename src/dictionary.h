#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "pilha.h"

/* Constrói um dicionário global a partir do arquivo [fname] */
void buildDictionary(char const* fname);

/* Se [chave] estiver no dicionario, sua [Celula*] correspondente é retornada. Senão retorna NULL.*/
Celula* inDictionary(char const* chave);

/* Se [chave] estiver no dicionario, seu valor [char*] correspondente é retornado. Senão retorna NULL.*/
char* valorInDictionary(char const* chave);

/* Libera memória ocupada pelo dicionário global. */
void destroyDictionary();

#endif