#ifndef GTK_RAYLIB_H
#define GTK_RAYLIB_H
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include <epoxy/gl.h>
// #include <epoxy/glx.h>
#include <raymath.h>
#include <raylib.h>

static const int keymap_raylib[] =
    {0, 1, 2, 3, 4, 5, 6, 7, KEY_BACKSPACE, KEY_TAB, 10, 11, 12, KEY_ENTER, 14, 15,
     KEY_LEFT_SHIFT, KEY_LEFT_CONTROL, KEY_LEFT_ALT, KEY_PAUSE, KEY_CAPS_LOCK, 21, 22,
     23, 24, 25, 26, KEY_ESCAPE, 28, 29, 30, 31, KEY_SPACE, KEY_PAGE_UP, KEY_PAGE_DOWN,
     KEY_END, KEY_HOME, KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, 41, 42, 43, KEY_PRINT_SCREEN,
     KEY_INSERT, KEY_DELETE, 47, 48, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX,
     KEY_SEVEN, KEY_EIGHT, KEY_NINE, 58, 59, 60, 61, 62, 63, 64, KEY_A, KEY_B, KEY_C, KEY_D,
     KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
     KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_KB_MENU,
     KEY_KB_MENU, 93, 94, 95, KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4,
     KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9, KEY_KP_MULTIPLY, KEY_KP_ADD, 108,
     KEY_KP_SUBTRACT, KEY_KP_DECIMAL, KEY_KP_DIVIDE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
     KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, 124, 125,
     126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
     KEY_NUM_LOCK, KEY_SCROLL_LOCK, 146, 147, 148, 149, 150, 151, 152,
     153, 154, 155, 156, 157, 158, 159, 160, KEY_RIGHT_SHIFT,
     162, KEY_RIGHT_CONTROL, 164, KEY_RIGHT_ALT, 166, 167, 168, 169, 170,
     171, 172, 173, 174, 175, 176, 177, 178, 179,
     180, 181, 182, 183, 184, 185, KEY_SEMICOLON, KEY_EQUAL, KEY_COMMA,
     KEY_MINUS, KEY_PERIOD, KEY_SLASH, 192, 193, 194, 195, 196, 197,
     198, 199, 200, 201, 202, 203, 204, 205, 206,
     207, 208, 209, 210, 211, 212, 213, 214, 215,
     216, 217, 218, KEY_LEFT_BRACKET, KEY_BACKSLASH, KEY_RIGHT_BRACKET, 222, 223, 224,
     225, 226, 227, 228, 229, 230, 231, 232, 233,
     234, 235, 236, 237, 238, 239, 240, 241, 242,
     243, 244, 245, 246, 247, 248, 249};
// -------------------------------------------------------
// BASE CORE RAYLIB
// -------------------------------------------------------

// -------------------------------------------------------
// GTK GLAREA
// -------------------------------------------------------

GtkWidget *gtk_raylib_embed_new(void);
void gtk_raylib_init(GtkWidget *window);

#endif