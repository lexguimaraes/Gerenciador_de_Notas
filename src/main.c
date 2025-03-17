#include "helper.h"

int cont = 1;
int modo_ord = -1;// 1 para ordenar pelas notas, 2 pela carga horária, e 3 pelo nome, valores negativos para decrescente;
char nome[30];
TLSE* lista = NULL;

static void quit(GtkWidget* widget, gpointer user_data) {
    escreve_arq(lista,nome);
    TLSE_libera(lista);
    //gtk_window_destroy(GTK_WINDOW(widget));
}

static void ordena(GtkWidget* widget, gpointer user_data) {
    const int novo_modo = GPOINTER_TO_INT(user_data);
    if (novo_modo == modo_ord) {
        modo_ord*=-1;
        lista = TLSE_reverse(lista);
    }
    else {
        modo_ord = novo_modo;
        atualiza_modo(&lista, modo_ord);
    }


    GtkWidget* grid = g_object_get_data(G_OBJECT(widget), "grid");
    atualiza_notas(lista,grid,1);
}



static void pop_error(GtkWidget* widget, const char* erro, int time) {
    GtkRoot* root = gtk_widget_get_root(widget);
    if (!GTK_IS_WINDOW(root)) {
        printf("not root\nnunca deveria acontecer\n");
        return;
    }
    GtkWidget* dialog = gtk_message_dialog_new(
        GTK_WINDOW(root),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_NONE,
        "%s",erro);

    gtk_window_set_deletable(GTK_WINDOW(dialog),FALSE);

    gtk_widget_set_visible(dialog,1);
    g_timeout_add_seconds(time,(GSourceFunc)gtk_window_destroy,dialog);
}

static void add_Nota(GtkWidget* widget, gpointer data) {
    GtkWidget* entry1 = g_object_get_data(G_OBJECT(widget),"entry1");
    GtkWidget* entry2 = g_object_get_data(G_OBJECT(widget), "entry2");
    GtkWidget* entry3 = g_object_get_data(G_OBJECT(widget),"entry3");
    GtkWidget* grid = g_object_get_data(G_OBJECT(widget),"grid");
    char materia_nome[50];
    const char* text = gtk_editable_get_text(GTK_EDITABLE(entry1));
    if (!*text || *text == ' ') {
        pop_error(widget, "Erro: Insira um nome",1);
        return;
    }
    strcpy(materia_nome,text);
    text = gtk_editable_get_text(GTK_EDITABLE(entry2));
    if (!is_digit(text)) {
        pop_error(widget, "Erro: Nota deve ser número",1);
        return;
    }
    int nota = atoi(text);
    text = gtk_editable_get_text(GTK_EDITABLE(entry3));
    if (!is_digit(text)){
        pop_error(widget, "Erro: Carga Horária deve ser número",1);
        return;
    }
    int ch = atoi(text);
    if (nota > 100 || nota < 0) {
        pop_error(widget, "Erro: Nota deve ser menor do que 100 e maior do que 0",1);
    }
    if (ch < 0 || ch > 200) {
        pop_error(widget, "Erro: Carga Horária deve ser menor do que 200 e maior do que 0",1);
    }
    int ind;
    lista = TLSE_insere(lista, materia_nome,nota,ch,modo_ord, &ind);
    atualiza_notas(lista, grid, ind);
}


static void rem_Nota(GtkWidget* widget, gpointer data) {
    GtkWidget* entry = g_object_get_data(G_OBJECT(widget), "entry");
    GtkWidget* grid = g_object_get_data(G_OBJECT(widget), "grid_notas");
    const char* s = gtk_editable_get_text(GTK_EDITABLE(entry));
    int ind = -1;
    lista = TLSE_remove(lista,s,&ind);
    if (ind == -1) {
        pop_error(widget, "Matéria não encontrada",1);
    }else atualiza_notas(lista,grid,ind);

}
GtkWidget* cria_grid_remocao(GtkWidget* grid_notas) {
    GtkWidget *button, *grid, *label, *entry;

    grid = gtk_grid_new();
    entry = gtk_entry_new();
    label = cria_label_contorno("Matéria a remover: ","contorno");
    gtk_entry_set_max_length(GTK_ENTRY(entry), 49);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label("Confirmar");
    g_signal_connect(button, "clicked", G_CALLBACK(rem_Nota), NULL);

    g_object_set_data(G_OBJECT(button),"entry", entry);
    g_object_set_data(G_OBJECT(button),"grid_notas",grid_notas);


    gtk_grid_attach(GTK_GRID(grid), label, 0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid), entry, 0,1,2,1);
    gtk_grid_attach(GTK_GRID(grid), button,0,2,2,1);

    return grid;
}


GtkWidget* cria_grid_adicao(GtkWidget* stack,GtkWidget* grid_notas) {
    GtkWidget *button, *grid,*label;

    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label("Confirmar");

    GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 49);
    label = cria_label_contorno("Nome da matéria : ","contorno");
    gtk_grid_attach(GTK_GRID(grid),label,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,0,1,1);
    g_object_set_data(G_OBJECT(button), "entry1", entry);

    label = cria_label_contorno("Nota : ","contorno");
    entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 3);

    gtk_grid_attach(GTK_GRID(grid),label,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,1,1,1);
    g_object_set_data(G_OBJECT(button), "entry2", entry);

    label = cria_label_contorno("Carga Horária : ","contorno");
    entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 3);

    gtk_grid_attach(GTK_GRID(grid),label,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,2,1,1);

    g_object_set_data(G_OBJECT(button), "stack", stack);
    g_object_set_data(G_OBJECT(button),"entry3",entry);
    g_object_set_data(G_OBJECT(button),"grid",grid_notas);
    g_signal_connect(button,"clicked",G_CALLBACK(add_Nota),NULL);

    gtk_grid_attach(GTK_GRID(grid),button,0,3,2,1);

    return grid;
}

static void atualizar_pagina_adicao(GtkWidget* widget, gpointer data) {
    GtkWidget* grid = gtk_stack_get_child_by_name(GTK_STACK(widget),"addnota");
    if (!grid)return;
    gtk_stack_set_visible_child(GTK_STACK(widget),grid);
}


static void atualizar_pagina(GtkWidget* widget,gpointer data) {
    GtkWidget* scrolled = gtk_stack_get_child_by_name(GTK_STACK(widget), "scrollable");
    if (!scrolled)return;
    gtk_stack_set_visible_child(GTK_STACK(widget), scrolled);
}

static void atualizar_pagina_remocao(GtkWidget* widget, gpointer data) {
    GtkWidget* page = gtk_stack_get_child_by_name(GTK_STACK(widget),"rem_nota");
    if (!page)return;
    gtk_stack_set_visible_child(GTK_STACK(widget),page);
}


static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window, *button, *grid,*stack,*box,*switcher,*scrollable,*content_box;
    window = gtk_application_window_new(app); // cria janela
    gtk_window_set_title(GTK_WINDOW (window), "vasco"); //define titulo
    gtk_window_set_default_size(GTK_WINDOW(window),1200,800); //define tamanho



    stack = gtk_stack_new();
    grid = gtk_grid_new();
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    switcher = gtk_stack_switcher_new();
    scrollable = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    GtkWidget* label;


    GtkWidget* grid_notas = gtk_grid_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrollable),content_box);
    gtk_box_append(GTK_BOX(content_box),grid_notas);


    button = gtk_button_new_with_label("               Nota               ");
    g_object_set_data(G_OBJECT(button), "grid",grid_notas);
    g_signal_connect(button, "clicked", G_CALLBACK(ordena), (gpointer)1);
    gtk_grid_attach(GTK_GRID(grid_notas),button, 1,0,1,1);

    button = gtk_button_new_with_label("               Carga Horária               ");
    g_object_set_data(G_OBJECT(button), "grid",grid_notas);
    g_signal_connect(button, "clicked", G_CALLBACK(ordena), (gpointer)2);
    gtk_grid_attach(GTK_GRID(grid_notas),button, 2,0,1,1);

    button = gtk_button_new_with_label("               Nome da Matéria               ");
    g_object_set_data(G_OBJECT(button), "grid",grid_notas);
    g_signal_connect(button, "clicked", G_CALLBACK(ordena), (gpointer)3);
    gtk_grid_attach(GTK_GRID(grid_notas),button, 0,0,1,1);

    label = cria_label_contorno("         CR:         ", "destaque");
    gtk_grid_attach(GTK_GRID(grid_notas),label, 3,0,1,1);

    inicializa_lista(&lista, nome,modo_ord);
    atualiza_notas(lista,grid_notas,1);


    gtk_stack_add_titled(GTK_STACK(stack), grid,"grid", "Menu");
    gtk_stack_add_titled(GTK_STACK(stack),scrollable, "scrollable","Notas");
    gtk_stack_add_titled(GTK_STACK(stack),cria_grid_adicao(stack,grid_notas),"addnota", "Adicionar Nota");
    gtk_stack_add_titled(GTK_STACK(stack),cria_grid_remocao(grid_notas),"rem_nota","Remover Nota");

    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));

    gtk_box_append(GTK_BOX(box),switcher);
    gtk_box_append(GTK_BOX(box),stack);

    gtk_widget_set_size_request(scrollable, 1200, 800);
    gtk_widget_set_size_request(grid_notas, 1000, 800);
    gtk_stack_set_hhomogeneous(GTK_STACK(stack),FALSE);
    gtk_widget_set_valign(content_box, GTK_ALIGN_START);
    gtk_widget_set_margin_start(content_box,70);
    gtk_widget_set_halign(content_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);



    gtk_window_set_child(GTK_WINDOW(window), box);

    button = gtk_button_new_with_label("Adicionar Notas");
    g_signal_connect_swapped(button,"clicked", G_CALLBACK(atualizar_pagina_adicao),stack);
    gtk_grid_attach(GTK_GRID(grid),button,0,0,2,1);


    button = gtk_button_new_with_label("Ver Notas");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(atualizar_pagina), stack);
    gtk_grid_attach(GTK_GRID(grid), button, 0,1,1,1);


    button = gtk_button_new_with_label("Remover Notas");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(atualizar_pagina_remocao),stack);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1,1,1);


    button = gtk_button_new_with_label("Sair");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);
    g_signal_connect (window, "destroy", G_CALLBACK (quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0 , 2, 2, 1);



    gtk_window_present (GTK_WINDOW (window)); //spawna janela

}

static void ler_entrada(GtkWidget* button, gpointer user_data) {
    GtkWidget* entry = g_object_get_data(G_OBJECT(button),"entry");
    GtkWidget* window = g_object_get_data(G_OBJECT(button),"window");
    GtkApplication* app = g_object_get_data(G_OBJECT(button),"app");

    const char* texto = gtk_editable_get_text(GTK_EDITABLE(entry));
    strcpy(nome,texto);
    char buffer[30];
    strcpy(buffer,nome);
    strcat(buffer,".bin");
    FILE* f = fopen(buffer,"r");
    if (!f) {
        f = fopen(buffer,"wb");
        fclose(f);
    }
    else fclose(f);
    gtk_widget_set_visible(window, FALSE);
    activate(app, NULL);
    gtk_window_destroy(GTK_WINDOW(window));

}



static void activate_init(GtkApplication* app, gpointer user_data) {
    // ajeitar fonte e custom labels
    GtkCssProvider* provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider,
        "window, label, button, entry { font-size: 14pt; }"
        ".title-label { font-size: 18pt; font-weight: bold; }"
        ".contorno { border: 1px solid #aab1b9; border-radius: 6px; padding: 4px 8px; margin: 0px; }"
        ".destaque { box-shadow: 0 0 0 2px blue;border: 2px solid #3584e4; border-radius:5px; background-color: #f0f0f0; padding: 5px; margin: 0px; }");

    // aplicar
    GdkDisplay* display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);


    GtkWidget* window, *button, *grid;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "vasco");
    gtk_window_set_default_size(GTK_WINDOW(window),500,800);
    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);


    GtkWidget* entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 24);
    GtkWidget* label = cria_label_contorno("Digite seu nome abaixo:","contorno");
    gtk_grid_attach(GTK_GRID(grid),label,0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid),entry,0,1,2,1);
    gtk_window_set_child(GTK_WINDOW(window),grid);
    button = gtk_button_new_with_label("Confirmar");

    g_object_set_data(G_OBJECT(button), "window", window);
    g_object_set_data(G_OBJECT(button), "app", app);
    g_object_set_data(G_OBJECT(button), "entry", entry);

    g_signal_connect(button,"clicked",G_CALLBACK(ler_entrada),NULL);
    gtk_grid_attach(GTK_GRID(grid),button,0,2,2,1);
    gtk_window_present(GTK_WINDOW(window));
}

static void ativar_tema(GtkApplication* app) {
    GtkSettings* settings = gtk_settings_get_default();
    g_object_set(settings,
                "gtk-font-name", "Segoe UI 14", // fonte e tamanho
                NULL);
}

int main(int argc, char** argv){
    GtkApplication* app;
    int status;
    app = gtk_application_new("vasco.gigante", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "startup", G_CALLBACK(ativar_tema), NULL);
    g_signal_connect (app, "activate", G_CALLBACK(activate_init),NULL);// conecta nossa janela a função activate para ativa-la
    status = g_application_run(G_APPLICATION (app), argc,argv);
    g_object_unref(app);
    return status;
}