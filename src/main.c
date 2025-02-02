#include <gtk/gtk.h>

typedef struct {
    GtkLabel *label;
    GtkTextView *text_view;
    GtkLabel *timer_label;
    GtkFixed *fixed;
    GtkTextBuffer *buffer;
    guint timer_id;
    gint seconds;
} ClickedUserData;

void on_insert_text(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint length, gpointer user_data) {
    g_print("Введен текст: %.*s\n", length, text);
}

gboolean update_timer(gpointer data) {
    ClickedUserData *timer_data = (ClickedUserData *)data;
    timer_data->seconds++;

    int minutes = timer_data->seconds / 60;
    int seconds = timer_data->seconds % 60;

    gchar *time_str = g_strdup_printf("%d:%02d", minutes, seconds);
    gtk_label_set_text(timer_data->timer_label, time_str);
    g_free(time_str);


    if (timer_data->seconds >= 20) {
        GtkTextBuffer *buffer = timer_data->buffer;
        GtkTextIter start, end;

        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        g_print("Содержимое text_view: %s\n", text);

        int text_length = strlen(text);

        float time_in_minutes = (float)timer_data->seconds / 60.0f;
        float speed_cpm = (float)text_length / time_in_minutes;

        char text_buffer_label[50];
        sprintf(text_buffer_label, "<span weight=\"bold\" font=\"Verdana 16\">Ваша скорость: %.2f CPM</span>", speed_cpm);

        g_print("Таймер истек - 20 секунд пройдено!\n");

        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Время истекло!");

        GtkWidget *new_label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(new_label), text_buffer_label);

        gtk_fixed_put(timer_data->fixed, new_label, 300, 100);
        gtk_widget_show(new_label);

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        g_free(text);

        return FALSE;

    }
    return TRUE;
}

static void started_clicked(GtkButton *btn, gpointer user_data) {
    ClickedUserData *data = (ClickedUserData*)user_data;

    data->seconds = 0;
    gtk_label_set_text(data->timer_label, "0:00");

    data->timer_id = g_timeout_add(1000, update_timer, data);
}

static void app_activate(GApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    GtkWidget *fixed = gtk_fixed_new();
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *text_view = gtk_text_view_new();
    GtkWidget *label = gtk_label_new("Buffer Label");
    GtkWidget *timer_label = gtk_label_new("0:00");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    gtk_window_set_title(GTK_WINDOW(window), "helooo");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    ClickedUserData *data = g_new(ClickedUserData, 1);
    data->label = GTK_LABEL(label);
    data->text_view = GTK_TEXT_VIEW(text_view);
    data->timer_label = GTK_LABEL(timer_label);
    data->fixed = GTK_FIXED(fixed);
    data->buffer = GTK_TEXT_BUFFER(buffer);
    data->timer_id = 0;
    data->seconds = 0;

    g_signal_connect(start_button, "clicked", G_CALLBACK(started_clicked), data);
    g_signal_connect(buffer, "insert-text", G_CALLBACK(on_insert_text), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_fixed_put(GTK_FIXED(fixed), start_button, 200, 1);
    gtk_fixed_put(GTK_FIXED(fixed), label, 200, 40);
    gtk_fixed_put(GTK_FIXED(fixed), text_view, 10, 200);
    gtk_fixed_put(GTK_FIXED(fixed), timer_label, 240, 100);

    gtk_widget_set_size_request(text_view, 100, 50);

    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_widget_show_all(window);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkApplication *app = gtk_application_new("com.example.timer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}