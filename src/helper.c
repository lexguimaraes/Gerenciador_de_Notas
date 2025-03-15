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

int is_word(const char* s) {
    const char* p = s;
    while (*p) {
        if (*p != ' ')return 1;
        p++;
    }
    return 0;
}

void limpa_grid(GtkWidget* grid,int ind) {
    GtkWidget* child;
    while ( (child = gtk_grid_get_child_at(GTK_GRID(grid),0,ind)) ) {
        gtk_grid_remove_row(GTK_GRID(grid),ind);
    }
}

GtkWidget* cria_label_contorno(const char* s, const char* classe) {
    GtkWidget* label = gtk_label_new(s);
    if (classe) {
        gtk_widget_add_css_class(label,classe);
    }
    return label;
}

void atualiza_notas(TLSE* lista, GtkWidget* grid,int i) {
    if (i == -1)return;
    limpa_grid(grid,i);
    int notas = 0,ch = 0;
    for (int j = 1;j < i;j++) {
        notas+= lista->mat->nota * lista->mat->ch;
        ch+= lista->mat->ch;
        lista = lista->next;
    }
    GtkWidget* label;
    char buff[50];
    double cr = 0;
    while (lista) {
        label = cria_label_contorno(lista->mat->nome,"contorno");
        gtk_grid_attach(GTK_GRID(grid),label,0,i,1,1);

        sprintf(buff, ("%d"),lista->mat->nota);
        label = cria_label_contorno(buff,"contorno");
        gtk_grid_attach(GTK_GRID(grid),label,1,i,1,1);

        sprintf(buff, ("%d"),lista->mat->ch);
        label = cria_label_contorno(buff,"contorno");
        gtk_grid_attach(GTK_GRID(grid),label,2,i,1,1);
        notas+= lista->mat->nota * lista->mat->ch;
        ch+= lista->mat->ch;
        i++;
        lista = lista->next;
    }
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid),3,1);
    if (GTK_IS_WIDGET(child)) gtk_grid_remove(GTK_GRID(grid),child);

    if (ch > 0 ) cr = (1.0*notas)/(1.0*ch);
    cr/=10.0;
    sprintf(buff, ("%.3f"), cr);
    label = cria_label_contorno(buff,"contorno");
    gtk_grid_attach(GTK_GRID(grid),label,3,1,1,1);
}

void atualiza_modo(GtkWidget* grid, TLSE** lista,const int modo) {
    TLSE* p = *lista;
    limpa_grid(grid,1);
    TLSE* new = NULL;
    int ph;
    while (p) {
        new = TLSE_insere(new, p->mat->nome, p->mat->nota,p->mat->ch,modo,&ph);
        p = p->next;
    }
    p = *lista ;
    TLSE_libera(p);
    *lista = new;
}

void escreve_arq(TLSE* lista, const char* nome) {
    if (!lista)return;
    char buff[60];
    strcpy(buff,nome);
    strcat(buff,".bin");
    FILE* fbin = fopen(buff,"wb");
    if (!fbin)exit(1);
    memset(buff,0,sizeof(buff));
    strcpy(buff,nome);
    strcat(buff,".csv");
    FILE* ftxt = fopen(buff, "wt");
    if (!ftxt)exit(1);
    fprintf(ftxt,"  Materias  ;  Notas  ;  Carga Horária  ;  CR  \n");
    int notas = 0, ch = 0;
    double cr = 0;
    while (lista) {
        fwrite(lista->mat, sizeof(materia),1,fbin);
        fprintf(ftxt,"  %s  ;  %d  ;  %d  ;\n",lista->mat->nome,lista->mat->nota,lista->mat->ch);
        notas += lista->mat->nota * lista->mat->ch;
        ch += lista->mat->ch;
        lista = lista->next;
    }
    cr = (1.0*notas)/(1.0*ch);
    cr/= 10.0;
    fprintf(ftxt, ";;; %.3f",cr);
    fclose(fbin);
    fclose(ftxt);
}

void inicializa_lista(TLSE** lista, const char* nome,int modo) {
    TLSE* p = *lista;
    char buff[60];
    strcpy(buff,nome);
    strcat(buff,".bin");
    FILE* f = fopen(buff,"rb");
    materia mat;
    int a;
    while (fread(&mat,sizeof(materia),1,f)) {
        p = TLSE_insere(p, mat.nome, mat.nota, mat.ch,modo,&a);
    }
    fclose(f);
    *lista = p;
}