// #define RLGL_IMPLEMENTATION
// #define RLGL_STANDALONE
#include "gtkraylib.h"

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
    embedWidth = gtk_widget_get_allocated_width(glarea);
    embedHeight = gtk_widget_get_allocated_height(glarea);

    EmbedSizeCallback(embedWidth, embedHeight);
}

static void _realize(GtkGLArea *glarea)
{
    gtk_gl_area_make_current(glarea);
    gtk_gl_area_set_has_depth_buffer(glarea, TRUE);

    embedWidth = gtk_widget_get_allocated_width(glarea);
    embedHeight = gtk_widget_get_allocated_height(glarea);

    InitWindow(embedWidth, embedHeight, "Embed-GTK");

    // Get frame clock:
    GdkGLContext *glcontext = gtk_gl_area_get_context(glarea);
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
    EmbedKeyCallback(keyval, 0, 1);
    // printf("KeyDown: %d\n", event->hardware_keycode);
}
static gboolean _key_release(GtkWidget *widget, GdkEventKey *event)
{
    int keyval = keymap_raylib[event->hardware_keycode];
    EmbedKeyCallback(keyval, 0, 0);
}
static gboolean _button_press(GtkWidget *widget, GdkEventButton *event)
{
    int button = -1;
    if (event->button == 1)button = MOUSE_LEFT_BUTTON;
    else if (event->button == 2)button = MOUSE_MIDDLE_BUTTON;
    else if (event->button == 3)button = MOUSE_RIGHT_BUTTON;
    EmbedMouseButtonCallback(button, 1, 0);
    return true;
}
static gboolean _button_release(GtkWidget *widget, GdkEventButton *event)
{
    int button = -1;
    if (event->button == 1)button = MOUSE_LEFT_BUTTON;
    else if (event->button == 2)button = MOUSE_MIDDLE_BUTTON;
    else if (event->button == 3)button = MOUSE_RIGHT_BUTTON;
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

        switch(direct){
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

void gtk_raylib_init(GtkWidget *window)
{
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(window, GDK_KEY_RELEASE_MASK);

    g_signal_connect(window, "key-press-event", _key_press, NULL);
    g_signal_connect(window, "key-release-event", _key_release, NULL);
}

GtkWidget *gtk_raylib_embed_new(void)
{
    GtkWidget *gl_area = gtk_gl_area_new();
    gtk_gl_area_set_auto_render(gl_area, true);
    // gtk_gl_area_set_has_depth_buffer(gl_area, true);
    // gtk_gl_area_set_has_stencil_buffer(gl_area, true);
    // gtk_gl_area_set_use_es(gl_area, true);
    // gtk_gl_area_set_has_alpha(gl_area, true);
    gtk_gl_area_set_required_version(gl_area, 3, 3);

    gtk_widget_add_events(gl_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON3_MASK);
    gtk_widget_add_events(gl_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(gl_area, GDK_SCROLL_MASK);

    g_signal_connect(gl_area, "realize", _realize, NULL);
    g_signal_connect(gl_area, "unrealize", _unrealize, NULL);
    g_signal_connect(gl_area, "size-allocate", _resize, NULL);

    g_signal_connect(gl_area, "button-press-event", _button_press, NULL);
    g_signal_connect(gl_area, "button-release-event", _button_release, NULL);
    g_signal_connect(gl_area, "motion-notify-event", _motion_notify, NULL);
    g_signal_connect(gl_area, "scroll-event", _mouse_wheel, NULL);

    return gl_area;
}