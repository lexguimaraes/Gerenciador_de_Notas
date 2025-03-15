#include "TLSE.h"



void TLSE_libera(TLSE* lista) {
    while (lista) {
        TLSE* t = lista->next;
        free(lista->mat);
        free(lista);
        lista = t;
    }
}

void TLSE_imprime(TLSE* lista) {
    while (lista) {
        printf("%s %d %d \n",lista->mat->nome, lista->mat->nota, lista->mat->ch);
        lista = lista->next;
    }
}

int compara(materia* materia1, materia*materia2,int modo){
    switch (modo) {
        case -1:
            return(materia2->nota > materia1->nota);
        case -2:
            return(materia2->ch > materia1->ch);
        case -3:
            return(strcmp(materia2->nome,materia1->nome));
        case 1:
            return (materia1->nota > materia2->nota);
        case 2:
            return (materia1->ch > materia2->ch);
        case 3:
            return (strcmp(materia1->nome,materia2->nome));
        default:
            return 0;
    }
}
TLSE* TLSE_busca(TLSE* lista, const char* nome, int* ind) {
    int i = 1;
    while (lista) {
        if (!strcmp(lista->mat->nome,nome)) {
            *ind = i;
            return lista;
        }
        lista = lista->next;;
        i++;
    }
    *ind = -1;
    return NULL;
}




TLSE* TLSE_insere(TLSE* lista, const char* nome, int nota, int ch,int modo,int* ind) {
    TLSE* temp = TLSE_busca(lista, nome,ind);
    if (temp) {
        lista = TLSE_remove(lista,nome,ind);
        lista = TLSE_insere(lista, nome, nota,ch,modo, ind);
        return lista;
    }

    TLSE* new = malloc(sizeof(TLSE));
    if (!new) {
        return lista;
    }
    new->mat = malloc(sizeof(materia));
    materia* nova = new->mat;
    strcpy(nova->nome,nome);
    nova->nota = nota;
    nova->ch = ch;
    int i = 1;
    TLSE* p = lista,*t = NULL,*prox;
    while (p) {
        if (compara(nova, p->mat, modo)<= 0)break;
        i++;
        t = p;
        p = p->next;
    }
    *ind = i;
    if (t) {
        t->next = new;
        new->next = p;
        return lista;
    }
    new->next = p;
    return new;
}

TLSE* TLSE_remove(TLSE* lista, const char* nome,int* ind) {
    if (!lista)return NULL;
    TLSE* p = lista, *ant = NULL, *t;
    int i = 1;
    while (p) {
        if (!strcmp(p->mat->nome,nome)) {
            break;
        }
        i++;
        ant = p;
        p = p->next;
    }
    *ind = i;
    if (!p) {
        *ind = -1;
        return lista;
    }

    if (!ant) {
        t = p->next;
        free(p->mat);
        free(p);
        return t;
    }

    ant->next = p->next;
    free(p->mat);
    free(p);
    return lista;
}