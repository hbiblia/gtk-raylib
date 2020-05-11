
#include <stdio.h>
#include <gtk/gtk.h>

#include <gtkraylib.h>

Vector2 ballPosition = {0};

void start()
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    ballPosition = (Vector2){(float)w/2, (float)h/2};
}

bool render()
{
    const char *hello = "move the ball with arrow keys";

    int w = GetScreenWidth();
    int w_text = MeasureText(hello, 20); // width text

    if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawText(hello, (w / 2) - (w_text / 2), 20, 20, LIGHTGRAY);
        DrawCircleV(ballPosition, 50, MAROON);
    }
    EndDrawing();
    return false;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
    gtk_window_set_title(GTK_WINDOW(win), "Example - input key");
    g_signal_connect(win, "destroy", gtk_main_quit, NULL);

    gtk_raylib_init(win);

    GtkWidget *embed = gtk_raylib_embed_new();
    gtk_container_add(GTK_CONTAINER(win), embed);
    g_signal_connect_swapped(embed, "render", G_CALLBACK(render), NULL);
    g_signal_connect_swapped(embed, "realize", G_CALLBACK(start), NULL);

    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}