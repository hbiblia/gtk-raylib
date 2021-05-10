
#include <stdio.h>
#include <gtk-raylib.h>

bool render()
{
    const char *hello = "Congrats! You created your first window!";

    int w = GetScreenWidth();
    int h = GetScreenHeight();
    int w_text = MeasureText(hello, 20); // width text
    
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawText(hello, (w/2) - (w_text/2), (h/2) - 20, 20, LIGHTGRAY); // Example
    }
    EndDrawing();
    return true;
}

void start()
{
    SetTargetFPS(60);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
    gtk_window_set_title(GTK_WINDOW(win),"Example - window");
    g_signal_connect(win, "destroy", gtk_main_quit, NULL);

    GtkWidget *embed = gtk_raylib_embed_new();
    gtk_container_add(GTK_CONTAINER(win), embed);
    g_signal_connect_swapped(embed, "render", G_CALLBACK(render), NULL);
    g_signal_connect_swapped(embed, "realize", G_CALLBACK(start), NULL);

    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}