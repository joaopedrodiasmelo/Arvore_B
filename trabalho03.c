#include "arvore_b.h"
#include <stdio.h>

int main()
{
    int teste = 1;
    arvore_b *raiz = NULL;
    while (teste != 0)
    {
        teste = leitura_de_dados(&raiz);
    };
    return 0;
}