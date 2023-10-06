#include "arvore_b.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

long int procurar_chave(arvore_b *raiz, int chave_id);
void inserir_registro_arquivo(arvore_b **raiz, registro chave);
void inserir_registro_arvoreB(arvore_b **raiz, registro_arvore registro_arv);
void inserir_elemento_pagina(arvore_b **raiz, arvore_b **pagina, bool *verificacao, registro_arvore **registro_aux, registro_arvore elemento);
arvore_b *criar_pagina();
void auxiliar_insercao();

// função responsável pela leitura/tratamento dos dados e realização das operações do input

int leitura_de_dados(arvore_b **raiz)
{

    char *input = calloc(1000, sizeof(char));
    scanf(" %[^(\r|\n)]", input);
    int leitura = 0;
    char *aux2;

    char *adicionar = calloc(4, sizeof(char));
    char *buscar = calloc(10, sizeof(char));

    // adiciona as strings os valores do input até o índice desejado para então verificar qual operação realizar
    strncpy(adicionar, &input[0], 3);
    adicionar[3] = '\0';
    strncpy(buscar, &input[0], 9);
    adicionar[9] = '\0';

    registro auxiliar;

    // irá inserir um novo registro
    if (strcmp(adicionar, "ADD") == 0)
    {
        for (int a = 0; a < strlen(input); a++)
        {
            int indiceFinal = 0;

            if (input[a] == '\'')
            {
                for (int b = a + 1; input[b] != '\''; b++)
                {
                    indiceFinal = b;
                }

                // tratamento da String separando os parametros passados no input
                switch (leitura)
                {
                case 0:
                    // separando o id
                    aux2 = calloc(indiceFinal - a, sizeof(char));
                    strncpy(aux2, &input[a + 1], indiceFinal - a);
                    aux2[indiceFinal - a] = '\0';
                    auxiliar.id = atoi(aux2);
                    leitura++;
                    free(aux2);
                    break;
                case 1:
                    // separando o titulo
                    strncpy(auxiliar.titulo, &input[a + 1], indiceFinal - a);
                    auxiliar.titulo[indiceFinal - a] = '\0';
                    leitura++;
                    break;
                case 2:
                    // separando o autor
                    strncpy(auxiliar.autor, &input[a + 1], indiceFinal - a);
                    auxiliar.autor[indiceFinal - a] = '\0';
                    leitura++;
                    break;

                default:
                    break;
                }
                a = indiceFinal + 1; // pula para a  segunda ocorrencia do  caracter
            }
        }
        printf("----------------------------------------------------------\n");
        // função para inserir registro
        inserir_registro_arquivo(raiz, auxiliar);
        return 1;
    }

    // pesquisa por um registro por meio da chave ID
    if (strcmp(buscar, "SEARCH id") == 0)
    {
        for (int a = 0; a < strlen(input); a++)
        {
            int indiceFinal = 0;

            if (input[a] == '\'')
            {
                for (int b = a + 1; input[b] != '\''; b++)
                {
                    indiceFinal = b;
                }
                // separando apenas o ID do resto da string input

                aux2 = calloc(indiceFinal - a, sizeof(char));
                strncpy(aux2, &input[a + 1], indiceFinal - a);
                aux2[indiceFinal - a] = '\0';
                auxiliar.id = atoi(aux2);
                free(aux2);
                break;
            }
        }
        printf("----------------------------------------------------------\n");
        // função para pesquizar na arvore
        arvore_b *auxiliar_procura = *raiz;
        long int posicao = (long int)procurar_chave(auxiliar_procura, auxiliar.id);
        if (posicao != -1)
        {
            registro print = pesquisar_registro_id(posicao);
            printf("%d - %s - %s\n", print.id, print.titulo, print.autor);

            return 1;
        }
        else
        {
            printf("Não encontrado\n");
            return 1;
        }
    }
    return 0;
}

// função responsável por inserir o registro no arquivo de dados e chamar a função para inserir a chave na arvore b
void inserir_registro_arquivo(arvore_b **raiz, registro chave)
{

    long int verifica_chave = procurar_chave(*raiz, chave.id); // verifica se o registro já eta presente no arquivo de dados

    if (verifica_chave != -1)
    {
        printf("Erro ao inserir registro, chave primária duplicada\n");
    }
    else // registro não está presente no arquivo de dados
    {
        // registro que irá ser inserido em uma pagina da arvore
        registro_arvore registroARV_b;
        registroARV_b.id = chave.id;
        long int *byte = inserir_registro(chave); // função que insere o registro no arquivo de dados e retorna o byteofsset desse registro

        registroARV_b.byte_ofsset = byte[0];

        inserir_registro_arvoreB(raiz, registroARV_b); // insere o registro na arvore B
    }
}

void inserir_registro_arvoreB(arvore_b **raiz, registro_arvore registro_arv)
{
    bool verifica;                                                   // variável que auxilia na determinação de novas raizes
    arvore_b *pagina;                                                // variável auxiliar na repartição das paginas
    registro_arvore *registro_aux = calloc(1, sizeof(registro_arv)); // registro auxiliar responsável por receber os dados do registro que irá subir no split

    inserir_elemento_pagina(&(*raiz), &pagina, &verifica, &registro_aux, registro_arv);

    if (verifica) // foi criada uma nova raiz
    {

        arvore_b *auxiliar = criar_pagina();
        auxiliar->numero_chaves = 1;
        auxiliar->registros[0].id = (*registro_aux).id;
        auxiliar->registros[0].byte_ofsset = (*registro_aux).byte_ofsset;
        auxiliar->filhos[1] = pagina;
        auxiliar->filhos[0] = (*raiz);
        *raiz = auxiliar;
    }
    printf("Registro inserido\n");
}

void inserir_elemento_pagina(arvore_b **raiz, arvore_b **pagina, bool *verificacao, registro_arvore **registro_aux, registro_arvore elemento)
{

    if (!(*raiz)) // ponteiro raiz é nulo gera um true
    {
        *verificacao = true;
        (*registro_aux)->id = elemento.id;
        (*registro_aux)->byte_ofsset = elemento.byte_ofsset;
        *pagina = NULL;

        return;
    }
    else
    {
        int index = 1;
        // percorre o vetor de chaves para encontrar a posição correta da chave a ser inserida
        while (index < (*raiz)->numero_chaves && elemento.id > (*raiz)->registros[index - 1].id)
        {
            index++;
        }

        if (elemento.id == (*raiz)->registros[index - 1].id)
        {
            printf("Erro ao inserir registro, chave primária duplicada\n");
            *verificacao = false;
            return;
        }
        else
        {
            if (elemento.id < (*raiz)->registros[index - 1].id) // o elemento deve ser inserido na posição anterior os valor do index
                index--;

            // verifica se a chave pode ser inserida em algum  filho da pagina atual
            inserir_elemento_pagina(&((*raiz)->filhos[index]), pagina, verificacao, registro_aux, elemento);

            // significa que existe uma pagina filho que possui a posição correta para se inserir esse novo elemento
            if (verificacao)
            {
                // encontrou a pagina que deve ser inserida sem realizar split
                if ((*raiz)->numero_chaves < QUANTIDADE_MAXIMA)
                {
                    auxiliar_insercao(*raiz, *pagina, **registro_aux); // verificar se gera erro esse **
                    *verificacao = false;
                }
                else
                { // realização do  split da pagina

                    arvore_b *nova_pagina = criar_pagina();

                    if (index < QUANTIDADE_MINIMA + 1)
                    {
                        auxiliar_insercao(nova_pagina, (*raiz)->filhos[QUANTIDADE_MAXIMA], (*raiz)->registros[QUANTIDADE_MAXIMA - 1]);
                        (*raiz)->numero_chaves--;
                        auxiliar_insercao(*raiz, *pagina, **registro_aux);
                    }
                    else
                    {
                        auxiliar_insercao(nova_pagina, *pagina, **registro_aux);
                    }

                    // redistribuindo os elementos entre as paginas
                    for (int a = QUANTIDADE_MINIMA + 2; a < ORDEM_ARVORE; a++)
                    {
                        auxiliar_insercao(nova_pagina, (*raiz)->filhos[a], (*raiz)->registros[a - 1]);
                    }
                    (*raiz)->numero_chaves = QUANTIDADE_MINIMA;
                    nova_pagina->filhos[0] = NULL;
                    (*registro_aux)->id = (*raiz)->registros[QUANTIDADE_MINIMA].id;
                    (*registro_aux)->byte_ofsset = (*raiz)->registros[QUANTIDADE_MINIMA].byte_ofsset;
                    *pagina = nova_pagina;
                }
            }
        }
    }
}

// função responsável por inserir os registros_arvore na pagina de maneira ordenada
void auxiliar_insercao(arvore_b *raiz, arvore_b *pagina, registro_arvore elemento)
{
    int index = raiz->numero_chaves;
    bool verifica = true;

    // ordena as chaves se for necessário
    while (verifica)
    {
        if (elemento.id < raiz->registros[index - 1].id)
        {
            raiz->registros[index].id = raiz->registros[index - 1].id;
            raiz->registros[index].byte_ofsset = raiz->registros[index - 1].byte_ofsset;
            raiz->filhos[index + 1] = raiz->filhos[index];
            index--;
        }
        if (elemento.id >= raiz->registros[index - 1].id || index < 0)
            verifica = false;
    }
    raiz->registros[index].id = elemento.id;
    raiz->registros[index].byte_ofsset = elemento.byte_ofsset;
    raiz->filhos[index + 1] = pagina;
    raiz->numero_chaves++;
}

arvore_b *criar_pagina()
{
    arvore_b *auxiliar = calloc(1, sizeof(arvore_b));

    auxiliar->numero_chaves = 0;

    auxiliar->registros = calloc(QUANTIDADE_MAXIMA, sizeof(registro_arvore));

    auxiliar->filhos = calloc(ORDEM_ARVORE, sizeof(arvore_b *));

    for (int a = 0; a < QUANTIDADE_MAXIMA; a++)
    {
        auxiliar->registros[a].id = -1;
        auxiliar->registros[a].byte_ofsset = -1;
        auxiliar->filhos[a] = NULL;
    }

    auxiliar->filhos[ORDEM_ARVORE - 1] = NULL; // pega o ultimo filho

    return auxiliar;
}

long int procurar_chave(arvore_b *raiz, int chave_id)
{
    if (raiz)
    {
        int index = 1;

        // percorre a pagina para verificar o indice que a chave esta na pagina
        while (index < raiz->numero_chaves && chave_id > raiz->registros[index - 1].id)
            index++;

        if (chave_id == raiz->registros[index - 1].id)
        {
            return raiz->registros[index - 1].byte_ofsset; // chave encontrada
        }
        else
        {
            if (chave_id < raiz->registros[index - 1].id)
                return procurar_chave(raiz->filhos[index - 1], chave_id);

            return procurar_chave(raiz->filhos[index], chave_id);
        }
    }

    return -1;
}