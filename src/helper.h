#ifndef HELPER_H
#define HELPER_H


#include <gtk/gtk.h>
#include "TLSE.h"


int is_digit(const char* s);

void limpa_grid(GtkWidget* grid,int ind);

GtkWidget* cria_label_contorno(const char* s, const char* classe);

void escreve_arq(TLSE* lista, const char* nome);

void inicializa_lista(TLSE** lista, const char* nome,int modo);

void atualiza_notas(TLSE* lista, GtkWidget* grid,int i);

void atualiza_modo(GtkWidget* grid, TLSE** lista, int modo);





#endif // HELPER_H
