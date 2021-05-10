
#include <stdio.h>
#include <gtk-raylib.h>

const int screenWidth = 800;
const int screenHeight = 450;
int count = 0;
char **droppedFiles = {0};

void start()
{
    SetTargetFPS(60);
}

bool render()
{
    // Update
    //----------------------------------------------------------------------------------
    if (IsFileDropped())
    {
        droppedFiles = GetDroppedFiles(&count);
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (count == 0)
        DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
    else
    {
        DrawText("Dropped files:", 100, 40, 20, DARKGRAY);

        for (int i = 0; i < count; i++)
        {
            if (i % 2 == 0)
                DrawRectangle(0, 85 + 40 * i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
            else
                DrawRectangle(0, 85 + 40 * i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));

            DrawText(droppedFiles[i], 120, 100 + 40 * i, 10, GRAY);
        }

        DrawText("Drop new files...", 100, 110 + 40 * count, 20, DARKGRAY);
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

    GtkWidget *embed = gtk_raylib_embed_new();
    gtk_container_add(GTK_CONTAINER(win), embed);
    g_signal_connect_swapped(embed, "render", G_CALLBACK(render), NULL);
    g_signal_connect_swapped(embed, "realize", G_CALLBACK(start), NULL);

    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}