#ifndef ARVORE_B_H
#define ARVORE_B_H
#define ORDEM_ARVORE 11
#define QUANTIDADE_MINIMA 5
#define QUANTIDADE_MAXIMA 10
#include "registro.h"

typedef struct chave_arvore
{
    int id;
    long int byte_ofsset;
} registro_arvore;

typedef struct arvoreB
{
    int numero_chaves;
    registro_arvore *registros;
    struct arvoreB **filhos;
} arvore_b;

int leitura_de_dados(arvore_b **raiz);

#endif