#include "helper.h"


int is_digit(const char* s) {
    const char* p = s;
    if (!*p) {
        return 0;
    }
    while (*p) {
        if (*p < 48 || *p > 57) {
            return 0;
        }
        p++;
    }
    return 1;
}

void limpa_grid(GtkWidget* grid) {
    GtkWidget* child;
    while ( (child = gtk_grid_get_child_at(GTK_GRID(grid),0,1)) ) {
        gtk_grid_remove_row(GTK_GRID(grid),1);
        printf("limpando grid %d \n",1);
    }
}