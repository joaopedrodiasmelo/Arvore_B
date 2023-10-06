#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DELIMiTADOR_TITULO "|"
#define DELIMITADOR_REGISTRO "#"
#define ARQUIVO_REGISTRO "registroDados.bin"
#define REGISTRO_EXCLUIDO "*|"

void escrita_no_arquivo(registro dado, FILE *arquivo, long int *byteOffset);

// função responsável por abrir o arquivo de dados e escreve-lo em sequência no arquivo
long int *inserir_registro(registro entrada)
{
    FILE *arquivo_dados = fopen(ARQUIVO_REGISTRO, "ab");
    fseek(arquivo_dados, 0, SEEK_END);
    long int *byteOffset = calloc(1, sizeof(long int));
    escrita_no_arquivo(entrada, arquivo_dados, byteOffset);
    fclose(arquivo_dados);

    return byteOffset; // posição do registro no arquivo
}

// função responsável por retornar o registro na posição passada(byteofset)
registro pesquisar_registro_id(long byteoffset)
{
    FILE *arquivo_dados = fopen(ARQUIVO_REGISTRO, "rb");
    registro auxiliar;
    char autor[100], titulo[100], caracter;
    int i = 0, h = 0, iD;

    fseek(arquivo_dados, byteoffset, SEEK_SET);

    fread(&iD, sizeof(int), 1, arquivo_dados);

    auxiliar.id = iD;
    
    //leitura do titulo 
    while (fread(&caracter, sizeof(char), 1, arquivo_dados) == 1)
    {
        if (caracter == '|')
        {
            titulo[i] = '\0';
            strcpy(auxiliar.titulo, titulo);
            break;
        }
        else
        {
            titulo[i] = caracter;
            i++;
        }
    }

    //leitura do autor
    while (fread(&caracter, sizeof(char), 1, arquivo_dados) == 1)
    {
        if (caracter == '#')
        {
            autor[h] = '\0';
            strcpy(auxiliar.autor, autor);
            break;
        }
        else
        {
            autor[h] = caracter;
            h++;
        }
    }
    fclose(arquivo_dados);

    return auxiliar;
}

// função responsável por escrever em um arquivo de dados e registrar a posição inicial do registro no arquivo
void escrita_no_arquivo(registro dado, FILE *arquivo, long int *byteOffset)
{
    byteOffset[0] = ftell(arquivo); //posição inicial do registro no arquivo de dados
    fwrite(&dado.id, sizeof(int), 1, arquivo);
    fwrite(&dado.titulo, sizeof(char), strlen(dado.titulo), arquivo);
    fwrite(DELIMiTADOR_TITULO, sizeof(char), 1, arquivo);
    fwrite(&dado.autor, sizeof(char), strlen(dado.autor), arquivo);
    fwrite(DELIMITADOR_REGISTRO, sizeof(char), 1, arquivo);
}
