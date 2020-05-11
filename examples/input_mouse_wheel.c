
#include <stdio.h>
#include <gtk/gtk.h>

#include <gtkraylib.h>

int scrollSpeed, boxPositionY;

void start()
{
    boxPositionY = GetScreenHeight() / 2 - 40;
    scrollSpeed = 4; // Scrolling speed in pixels
}

bool render()
{
    // Update
    //----------------------------------------------------------------------------------
    boxPositionY -= (GetMouseWheelMove() * scrollSpeed);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawRectangle(GetScreenWidth() / 2 - 40, boxPositionY, 80, 80, MAROON);

        DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
        DrawText(FormatText("Box position Y: %03i", boxPositionY), 10, 40, 20, LIGHTGRAY);
    }
    EndDrawing();
    return false;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 450);
    gtk_window_set_title(GTK_WINDOW(win), "Example - Input mouse wheel");
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