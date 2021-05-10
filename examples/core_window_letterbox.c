
#include <stdio.h>
#include <gtk-raylib.h>

const int windowWidth = 800;
const int windowHeight = 450;

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

// Clamp Vector2 value with min and max and return a new vector2
// NOTE: Required for virtual mouse, to clamp inside virtual game size
Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max)
{
    Vector2 result = value;
    result.x = (result.x > max.x) ? max.x : result.x;
    result.x = (result.x < min.x) ? min.x : result.x;
    result.y = (result.y > max.y) ? max.y : result.y;
    result.y = (result.y < min.y) ? min.y : result.y;
    return result;
}

int gameScreenWidth = 640;
int gameScreenHeight = 480;
RenderTexture2D target;
Color colors[10] = {0};

void start()
{
    // Render texture initialization, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, FILTER_BILINEAR); // Texture scale filter to use

    for (int i = 0; i < 10; i++)
        colors[i] = (Color){GetRandomValue(100, 250), GetRandomValue(50, 150), GetRandomValue(10, 100), 255};

    SetTargetFPS(60);
}

bool render()
{
    // Update
    //----------------------------------------------------------------------------------
    // Compute required framebuffer scaling
    float scale = min((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);

    if (IsKeyPressed(KEY_SPACE))
    {
        // Recalculate random colors for the bars
        for (int i = 0; i < 10; i++)
            colors[i] = (Color){GetRandomValue(100, 250), GetRandomValue(50, 150), GetRandomValue(10, 100), 255};
    }

    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 mouse = GetMousePosition();
    Vector2 virtualMouse = {0};
    virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth * scale)) * 0.5f) / scale;
    virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight * scale)) * 0.5f) / scale;
    virtualMouse = ClampValue(virtualMouse, (Vector2){0, 0}, (Vector2){(float)gameScreenWidth, (float)gameScreenHeight});

    // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
    //SetMouseOffset(-(GetScreenWidth() - (gameScreenWidth*scale))*0.5f, -(GetScreenHeight() - (gameScreenHeight*scale))*0.5f);
    //SetMouseScale(1/scale, 1/scale);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);

    ClearBackground(RAYWHITE); // Clear render texture background color

    for (int i = 0; i < 10; i++)
        DrawRectangle(0, (gameScreenHeight / 10) * i, gameScreenWidth, gameScreenHeight / 10, colors[i]);

    DrawText("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, WHITE);

    DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);

    EndTextureMode();

    // Draw RenderTexture2D to window, properly scaled
    DrawTexturePro(target.texture, (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                   (Rectangle){(GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f,
                               (float)gameScreenWidth * scale, (float)gameScreenHeight * scale},
                   (Vector2){0, 0}, 0.0f, WHITE);

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