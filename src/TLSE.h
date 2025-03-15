#ifndef TLSE_H
#define TLSE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Materia{
    char nome[50];
    int nota;
    int ch;
}materia;

typedef struct Lista {
    materia* mat;
    struct Lista* next;
}TLSE;

void TLSE_libera(TLSE* lista);

void TLSE_imprime(TLSE* lista);

TLSE* TLSE_busca(TLSE* lista, const char* nome, int* ind);

TLSE* TLSE_insere(TLSE* lista, const char* nome, int nota, int ch,int modo,int* ind);

int compara(materia* materia1, materia*materia2,int modo);

TLSE* TLSE_remove(TLSE* lista, const char* nome,int* ind);



#endif // TLSE_H
