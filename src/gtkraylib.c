// #define RLGL_IMPLEMENTATION
// #define RLGL_STANDALONE
#include <gtkraylib.h>

// render viewport
static int embedWidth;
static int embedHeight;

// :(
static GtkWidget *glAreaGlobal;

// -------------------------------------------------------
// BASE CORE RAYLIB
// -------------------------------------------------------

// -------------------------------------------------------
// GTK GLAREA
// -------------------------------------------------------
static void _unrealize(GtkWidget *widget)
{
    // CloseWindow();
}

static void _resize(GtkGLArea *glarea)
{
    embedWidth = gtk_widget_get_allocated_width(GTK_WIDGET(glarea));
    embedHeight = gtk_widget_get_allocated_height(GTK_WIDGET(glarea));

    EmbedSizeCallback(embedWidth, embedHeight);
}

static void _realize(GtkGLArea *glarea)
{
    gtk_gl_area_make_current(glarea);
    gtk_gl_area_set_has_depth_buffer(glarea, TRUE);

    embedWidth = gtk_widget_get_allocated_width(GTK_WIDGET(glarea));
    embedHeight = gtk_widget_get_allocated_height(GTK_WIDGET(glarea));

    InitWindow(embedWidth, embedHeight, "Embed-GTK");

    // Get frame clock:
    GdkGLContext *glcontext = gtk_gl_area_get_context(GTK_GL_AREA(glarea));
    GdkWindow *glwindow = gdk_gl_context_get_window(glcontext);
    GdkFrameClock *frame_clock = gdk_window_get_frame_clock(glwindow);

    // Connect update signal:
    g_signal_connect_swapped(frame_clock, "update", G_CALLBACK(gtk_gl_area_queue_render), glarea);

    // Start updating:
    gdk_frame_clock_begin_updating(frame_clock);
}
static gboolean _key_press(GtkWidget *widget, GdkEventKey *event)
{
    int keyval = keymap_raylib[event->hardware_keycode];
    EmbedKeyCallback(keyval, 0, 1, 0);
    // printf("KeyDown: %d\n", event->hardware_keycode);
}
static gboolean _key_release(GtkWidget *widget, GdkEventKey *event)
{
    int keyval = keymap_raylib[event->hardware_keycode];
    EmbedKeyCallback(keyval, 0, 0, 0);
}
static gboolean _button_press(GtkWidget *widget, GdkEventButton *event)
{
    int button = -1;
    if (event->button == 1)
        button = MOUSE_LEFT_BUTTON;
    else if (event->button == 2)
        button = MOUSE_MIDDLE_BUTTON;
    else if (event->button == 3)
        button = MOUSE_RIGHT_BUTTON;
    EmbedMouseButtonCallback(button, 1, 0);
    return true;
}
static gboolean _button_release(GtkWidget *widget, GdkEventButton *event)
{
    int button = -1;
    if (event->button == 1)
        button = MOUSE_LEFT_BUTTON;
    else if (event->button == 2)
        button = MOUSE_MIDDLE_BUTTON;
    else if (event->button == 3)
        button = MOUSE_RIGHT_BUTTON;
    EmbedMouseButtonCallback(button, 0, 0);
    return false;
}
static gboolean _motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
    EmbedMouseCursorPosCallback(event->x, event->y);
    return false;
}
static gboolean _mouse_wheel(GtkWidget *widget, GdkEventScroll *event)
{
    gdouble y_scroll;
    GdkScrollDirection direct;
    if (gdk_event_get_scroll_direction(event, &direct))
    {
        const double delta = 1.0;

        switch (direct)
        {
        case GDK_SCROLL_UP:
            y_scroll = delta;
            break;
        case GDK_SCROLL_DOWN:
            y_scroll = -delta;
            break;
        }
    }
    EmbedScrollCallback(0, y_scroll);
}

// -------------------------------------------------------
// gtk_raylib_init - Se debe inicializar justo despues de la 
// la ventana para poder implementar los eventos del input.
// -------------------------------------------------------
void gtk_raylib_init(GtkWidget *window)
{
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(window, GDK_KEY_RELEASE_MASK);

    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(_key_press), NULL);
    g_signal_connect(G_OBJECT(window), "key-release-event", G_CALLBACK(_key_release), NULL);
}

// -------------------------------------------------------
// gtk_raylib_embed_new - Donde queramos un viewport.
// Gtk3 solo soporta GL3.x
// -------------------------------------------------------
GtkWidget *gtk_raylib_embed_new(void)
{
    GtkWidget *gl_area = gtk_gl_area_new();
    // gtk_gl_area_set_auto_render(gl_area, true);
    // gtk_gl_area_set_has_depth_buffer(gl_area, true);
    // gtk_gl_area_set_has_stencil_buffer(gl_area, true);
    // gtk_gl_area_set_use_es(gl_area, true);
    // gtk_gl_area_set_has_alpha(gl_area, true);
    gtk_gl_area_set_required_version(GTK_GL_AREA(gl_area), 3, 3);

    g_signal_connect(G_OBJECT(gl_area), "realize", G_CALLBACK(_realize), NULL);
    g_signal_connect(G_OBJECT(gl_area), "unrealize", G_CALLBACK(_unrealize), NULL);
    // g_signal_connect(G_OBJECT(gl_area), "size-allocate", G_CALLBACK(_resize), NULL);

    gtk_widget_add_events(gl_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON3_MASK);
    gtk_widget_add_events(gl_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(gl_area, GDK_SCROLL_MASK);

    g_signal_connect(G_OBJECT(gl_area), "button-press-event", G_CALLBACK(_button_press), NULL);
    g_signal_connect(G_OBJECT(gl_area), "button-release-event", G_CALLBACK(_button_release), NULL);
    g_signal_connect(G_OBJECT(gl_area), "motion-notify-event", G_CALLBACK(_motion_notify), NULL);
    g_signal_connect(G_OBJECT(gl_area), "scroll-event", G_CALLBACK(_mouse_wheel), NULL);

    return gl_area;
}