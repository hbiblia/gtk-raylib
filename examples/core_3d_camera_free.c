
#include <stdio.h>
#include <gtk-raylib.h>

Camera3D camera = {0};
Vector3 cubePosition;

const int screenWidth = 800;
const int screenHeight = 450;

void start()
{
    // Define the camera to look into our 3d world
    camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type

    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode
}

bool render()
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateCamera(&camera); // Update camera

    if (IsKeyDown('Z'))
        camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

    DrawGrid(10, 1.0f);

    EndMode3D();

    DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 320, 133, BLUE);

    DrawText("Free camera default controls:", 20, 20, 10, BLACK);
    DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
    DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
    DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
    DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);

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