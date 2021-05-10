
#include <stdio.h>
#include <gtk-raylib.h>

Camera camera = {0};
Vector3 cubePosition = {0};
Vector3 cubeSize = {0};
Ray ray = {0}; // Picking line ray
bool collision = false;

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

    cubePosition = (Vector3){0.0f, 1.0f, 0.0f};
    cubeSize = (Vector3){2.0f, 2.0f, 2.0f};

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
}

bool render()
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateCamera(&camera); // Update camera

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (!collision)
        {
            ray = GetMouseRay(GetMousePosition(), camera);

            // Check collision between ray and box
            collision = CheckCollisionRayBox(ray,
                                             (BoundingBox){(Vector3){cubePosition.x - cubeSize.x / 2, cubePosition.y - cubeSize.y / 2, cubePosition.z - cubeSize.z / 2},
                                                           (Vector3){cubePosition.x + cubeSize.x / 2, cubePosition.y + cubeSize.y / 2, cubePosition.z + cubeSize.z / 2}});
        }
        else
            collision = false;
    }
    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    if (collision)
    {
        DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
        DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

        DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
    }
    else
    {
        DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
        DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
    }

    DrawRay(ray, MAROON);
    DrawGrid(10, 1.0f);

    EndMode3D();

    DrawText("Try selecting the box with mouse!", 240, 10, 20, DARKGRAY);

    if (collision)
        DrawText("BOX SELECTED", (screenWidth - MeasureText("BOX SELECTED", 30)) / 2, (int)(screenHeight * 0.1f), 30, GREEN);

    DrawFPS(10, 10);

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