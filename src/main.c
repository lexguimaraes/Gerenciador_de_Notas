#include <gtk/gtk.h>
#include "helper.h"

char nome[30];


static void quit(GtkWidget* widget, gpointer user_data) {

    gtk_window_destroy(GTK_WINDOW(widget));
}
static void pop_error(GtkWidget* widget, const char* erro, int time) {
    GtkRoot* root = gtk_widget_get_root(widget);
    if (!GTK_IS_WINDOW(root)) {
        printf("not root\n");
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

static void print_hello(GtkWidget* widget, gpointer data) {
    GtkWidget* entry1 = g_object_get_data(G_OBJECT(widget),"entry1");
    GtkWidget* entry2 = g_object_get_data(G_OBJECT(widget), "entry2");
    GtkWidget* entry3 = g_object_get_data(G_OBJECT(widget),"entry3");
    GtkWidget* box = g_object_get_data(G_OBJECT(widget),"box");
    const char* text = gtk_editable_get_text(GTK_EDITABLE(entry1));
    printf("Nome da materia : %s\n",text);

    text = gtk_editable_get_text(GTK_EDITABLE(entry2));
    if (!is_digit(text)) {
        pop_error(widget, "Erro: Nota deve ser número",2);
        return;
    }
    printf("%s\n",text);
    text = gtk_editable_get_text(GTK_EDITABLE(entry3));
    if (!is_digit(text)){
        pop_error(widget, "Erro: Carga Horária deve ser número",2);
        return;
    }

    GtkWidget* child;
    while ((child = gtk_widget_get_first_child(box))!=NULL) {
        gtk_box_remove(GTK_BOX(box),child);
    }

    char buff[50];
    for (int i = 0;i < 100;i++) {
        sprintf(buff, ("%s %d"),nome,i);
        GtkWidget* label = gtk_label_new(buff);
        gtk_box_append(GTK_BOX(box),label);
    }
    printf("%s\n",text);
}


static void removendo_notas(GtkWidget* widget, gpointer data) {
    printf("Removendo Notas...\n");
}

GtkWidget* cria_grid(GtkWidget* stack,GtkWidget* content_box) {
    GtkWidget *button, *grid,*label;

    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    button = gtk_button_new_with_label("Confirmar");

    GtkWidget* entry = gtk_entry_new();
    label = gtk_label_new("Nome da matéria : ");
    gtk_grid_attach(GTK_GRID(grid),label,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,0,1,1);
    g_object_set_data(G_OBJECT(button), "entry1", entry);

    label = gtk_label_new("Nota : ");
    entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid),label,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,1,1,1);
    g_object_set_data(G_OBJECT(button), "entry2", entry);

    label = gtk_label_new("Carga Horária : ");
    entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid),label,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),entry,1,2,1,1);

    g_object_set_data(G_OBJECT(button), "stack", stack);
    g_object_set_data(G_OBJECT(button),"entry3",entry);
    g_object_set_data(G_OBJECT(button),"box",content_box);
    g_signal_connect(button,"clicked",G_CALLBACK(print_hello),NULL);

    gtk_grid_attach(GTK_GRID(grid),button,0,3,2,1);

    return grid;
}

static void atualizar_pagina(GtkWidget* widget,gpointer data) {
    GtkWidget* scrolled = gtk_stack_get_child_by_name(GTK_STACK(widget), "scrollable");
    if (!scrolled)return;
   /* printf("scrolled type: %s\n",G_OBJECT_TYPE_NAME(scrolled));
    GtkWidget* box = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scrolled));
    if (!box) {
        printf("not box\n");
        return;
    }
    printf("Box type: %s\n", G_OBJECT_TYPE_NAME(box));
    box = gtk_viewport_get_child(GTK_VIEWPORT(box));
    if (!GTK_IS_BOX(box)) {
        printf("Erro: Widget não é um GtkBox ");
        return;
    }
    GtkWidget* child;
    while ((child = gtk_widget_get_first_child(box))!=NULL) {
        gtk_box_remove(GTK_BOX(box),child);
    }


    GtkWidget* label;
    char buff[50];
    for (int i = 0;i < 100;i++) {
        sprintf(buff, ("%s %d"),nome,i);
        label = gtk_label_new(buff);
        gtk_box_append(GTK_BOX(box),label);
    }
    gtk_widget_set_halign(box,GTK_ALIGN_START);*/
    gtk_stack_set_visible_child(GTK_STACK(widget), scrolled);
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

    GtkWidget* label = gtk_label_new("Adicione Notas");

    gtk_box_append(GTK_BOX(content_box),label);


    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrollable),content_box);


    gtk_stack_add_titled(GTK_STACK(stack),grid,"grid", "Menu");
    gtk_stack_add_titled(GTK_STACK(stack),scrollable, "scrollable","Notas");
    gtk_stack_add_titled(GTK_STACK(stack),cria_grid(stack,content_box),"grid:2", "teste");

    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));


    gtk_widget_set_size_request(scrollable, 1200, 800);
    gtk_stack_set_hhomogeneous(GTK_STACK(stack),FALSE);
    gtk_widget_set_valign(content_box, GTK_ALIGN_START);
    gtk_widget_set_margin_start(content_box,50);
    gtk_widget_set_halign(content_box, GTK_ALIGN_START);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);


    gtk_box_append(GTK_BOX(box),switcher);
    gtk_box_append(GTK_BOX(box),stack);

    gtk_window_set_child(GTK_WINDOW(window), box);




    button = gtk_button_new_with_label("Ver Notas");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(atualizar_pagina), stack);
    gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);


    button = gtk_button_new_with_label("Remover Notas");
    g_signal_connect(button, "clicked", G_CALLBACK(removendo_notas),NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0,1,1);


    button = gtk_button_new_with_label("Sair");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0 , 1, 2, 1);

    gtk_window_present (GTK_WINDOW (window)); //spawna janela

}

static void ler_entrada(GtkWidget* button, gpointer user_data) {
    GtkWidget* entry = g_object_get_data(G_OBJECT(button),"entry");
    GtkWidget* window = g_object_get_data(G_OBJECT(button),"window");
    GtkApplication* app = g_object_get_data(G_OBJECT(button),"app");

    const char* texto = gtk_editable_get_text(GTK_EDITABLE(entry));
    strcpy(nome,texto);
    printf("%s\n",nome);
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
    // Configurar CSS para fontes
    GtkCssProvider* provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider,
        "window, label, button, entry { font-size: 14pt; }"
        ".title-label { font-size: 18pt; font-weight: bold; }");

    // Aplicar globalmente
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
    GtkWidget* label = gtk_label_new("Digite seu nome abaixo:");
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
    g_signal_connect (app, "activate", G_CALLBACK(activate_init),NULL); // conecta nossa janela a função activate para ativa-la
    status = g_application_run(G_APPLICATION (app), argc,argv);
    g_object_unref(app);
    return status;
}