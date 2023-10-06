#ifndef REGISTRO_H
#define REGISTRO_H

typedef struct registro
{
    int id;
    char titulo[100];
    char autor[100];

} registro;

long int *inserir_registro(registro entrada); //retorna a posição inicial do registro no arquivo de dados
registro pesquisar_registro_id(long int byteoffset); //retorna o registro presente no arquivo de dados


#endif