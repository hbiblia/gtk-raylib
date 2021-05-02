
#include <stdio.h>
#include <gtk-raylib.h>

Vector2 ballPosition = {-100.0f, -100.0f};
Color ballColor = DARKBLUE;

void start()
{
}

bool render()
{
    const char *hello = "move ball with mouse and click mouse button to change color";

    int w = GetScreenWidth();
    int w_text = MeasureText(hello, 20); // width text

    // Update
    //----------------------------------------------------------------------------------
    ballPosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ballColor = MAROON;
    else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) ballColor = LIME;
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) ballColor = DARKBLUE;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawCircleV(ballPosition, 40, ballColor);
        DrawText(hello, (w / 2) - (w_text / 2), 20, 20, LIGHTGRAY);
    }
    EndDrawing();
    return false;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
    gtk_window_set_title(GTK_WINDOW(win), "Example - input mouse");
    g_signal_connect(win, "destroy", gtk_main_quit, NULL);

    GtkWidget *embed = gtk_raylib_embed_new();
    gtk_container_add(GTK_CONTAINER(win), embed);
    g_signal_connect_swapped(embed, "render", G_CALLBACK(render), NULL);
    g_signal_connect_swapped(embed, "realize", G_CALLBACK(start), NULL);

    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}