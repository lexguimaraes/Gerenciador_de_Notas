#ifndef HELPER_H
#define HELPER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

typedef struct Materia{
     char nome[50];
     int nota;
     int ch;
}materia;

int is_digit(const char* s);

void limpa_grid(GtkWidget* grid);





#endif // HELPER_H
