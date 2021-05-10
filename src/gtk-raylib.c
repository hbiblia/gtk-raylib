// #define RLGL_IMPLEMENTATION
// #define RLGL_STANDALONE
#include <gtk-raylib.h>

// render viewport
static int embedWidth;
static int embedHeight;
// -------------------------------------------------------
// BASE CORE RAYLIB
// -------------------------------------------------------
void EmbedSizeCallback(int width, int height);
void EmbedMouseButtonCallback(int button, int action, int mods);
void EmbedMouseCursorPosCallback(double x, double y);
void EmbedKeyCallback(int key, int scancode, int action, int mods);
void EmbedScrollCallback(double xoffset, double yoffset);
void EmbedCursorEnterCallback(int enter);
static void EmbedWindowDropCallback(int count, const char **paths);

// -------------------------------------------------------
// Cerramos el context
// -------------------------------------------------------
static void _unrealize(GtkWidget *widget)
{
    gtk_gl_area_make_current(GTK_GL_AREA(widget));

    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return;

    CloseWindow();
}

// -------------------------------------------------------
// Cuando el context de inicializa se carga raylib
// -------------------------------------------------------
static void _realize(GtkGLArea *glarea)
{
    gtk_gl_area_make_current(glarea);
    if (gtk_gl_area_get_error(GTK_GL_AREA(glarea)) != NULL)
        return;

    gtk_gl_area_set_has_depth_buffer(glarea, TRUE);

    embedWidth = gtk_widget_get_allocated_width(GTK_WIDGET(glarea));
    embedHeight = gtk_widget_get_allocated_height(GTK_WIDGET(glarea));

    InitWindow(embedWidth, embedHeight, NULL);

    // Get frame clock:
    GdkGLContext *glcontext = gtk_gl_area_get_context(GTK_GL_AREA(glarea));
    GdkWindow *glwindow = gdk_gl_context_get_window(glcontext);
    GdkFrameClock *frame_clock = gdk_window_get_frame_clock(glwindow);

    // Connect update signal:
    g_signal_connect_swapped(frame_clock, "update", G_CALLBACK(gtk_gl_area_queue_render), glarea);

    // Start updating:
    gdk_frame_clock_begin_updating(frame_clock);
}

// -------------------------------------------------------
// Evento cambio de tamano.
// -------------------------------------------------------
static void _resize(GtkGLArea *glarea)
{
    embedWidth = gtk_widget_get_allocated_width(GTK_WIDGET(glarea));
    embedHeight = gtk_widget_get_allocated_height(GTK_WIDGET(glarea));
    EmbedSizeCallback(embedWidth, embedHeight);
}

// -------------------------------------------------------
// Evento KeyDown
// -------------------------------------------------------
static gboolean _key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    int keyval = keymap_raylib[event->hardware_keycode];
    // printf("KeyInfo: %s -> %d -> %d\n",gdk_keyval_name(event->keyval),event->hardware_keycode, keyval);
    EmbedKeyCallback(keyval, 0, 1, 0);
    return true;
}

// -------------------------------------------------------
// Evento KeyUp
// -------------------------------------------------------
static gboolean _key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    int keyval = keymap_raylib[event->hardware_keycode];
    EmbedKeyCallback(keyval, 0, 0, 0);
    return true;
}

// -------------------------------------------------------
// Evento Mouse Button down
// -------------------------------------------------------
static gboolean _button_press(GtkWidget *widget, GdkEventButton *event)
{
    if (!gtk_widget_has_grab(widget))
    {
        gtk_widget_set_can_focus(GTK_GL_AREA(widget), true);
        gtk_widget_grab_focus(widget);
    }

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

// -------------------------------------------------------
// Evento Mouse Button up
// -------------------------------------------------------
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

// -------------------------------------------------------
// Evento Mouse move
// -------------------------------------------------------
static gboolean _motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
    EmbedMouseCursorPosCallback(event->x, event->y);
    return false;
}

// -------------------------------------------------------
// Evento para el mouse ruedita o wheel
// -------------------------------------------------------
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
// Iniciamos el evento key, para esto buscamos el parent-window
// -------------------------------------------------------
gboolean _InfocusGlArea(GtkWidget *widget,
                        GdkEvent *event,
                        gboolean data)
{
    // Detenemos los inputs, cuando no tenemos focus.
    if (data == false)
    {
        for (int i = 0; i < 250; i++)
        {
            EmbedKeyCallback(keymap_raylib[i], 0, 0, 0);
        }
        return true;
    }
    EmbedCursorEnterCallback(1);
}
gboolean _OutfocusGlArea(GtkWidget *widget,
                         GdkEvent *event,
                         gboolean data)
{
    EmbedCursorEnterCallback(0);
}

void _droppable(GtkWidget *widget,
                GdkDragContext *context,
                gint x,
                gint y,
                GtkSelectionData *data,
                guint info,
                guint time)
{
    guchar *text_result = gtk_selection_data_get_data(data);
    gchar **result = g_strsplit(text_result, "\n", -1);
    int length = g_strv_length(result) - 1;

    EmbedWindowDropCallback(length, result);

    g_strfreev(result);
    // g_free(text_result);
}

GtkWidget *_get_window(GtkWidget *widget)
{
    GtkWidget *child = widget;
    for (;;)
    {
        child = gtk_widget_get_parent(child);
        if (strcmp(gtk_widget_get_name(child), "GtkWindow") == 0)
        {
            break;
        }
    }

    return child;
}

static void _parentSet(GtkWidget *widget,
                       GtkWidget *old_parent,
                       gpointer user_data)
{

    GtkWidget *win_parent = _get_window(widget);
    {
        gtk_widget_add_events(win_parent, GDK_FOCUS_CHANGE_MASK);
        g_signal_connect(G_OBJECT(win_parent), "focus-in-event", G_CALLBACK(_InfocusGlArea), true);
        g_signal_connect(G_OBJECT(win_parent), "focus-out-event", G_CALLBACK(_OutfocusGlArea), false);
        // g_signal_connect(G_OBJECT(win_parent), "set-focus", G_CALLBACK(_InfocusGlArea), false);
    }
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
    gtk_gl_area_set_required_version(GTK_GL_AREA(gl_area), 3, 2);
    // gtk_widget_set_can_focus(GTK_GL_AREA(gl_area), true);
    gtk_widget_set_focus_on_click(GTK_GL_AREA(gl_area), true);

    g_signal_connect(G_OBJECT(gl_area), "realize", G_CALLBACK(_realize), NULL);
    g_signal_connect(G_OBJECT(gl_area), "unrealize", G_CALLBACK(_unrealize), NULL);
    g_signal_connect(G_OBJECT(gl_area), "size-allocate", G_CALLBACK(_resize), NULL);

    gtk_widget_add_events(gl_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(gl_area, GDK_BUTTON3_MASK);
    gtk_widget_add_events(gl_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(gl_area, GDK_SCROLL_MASK);
    gtk_widget_add_events(gl_area, GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(gl_area, GDK_KEY_RELEASE_MASK);
    // gtk_widget_add_events(gl_area, GDK_LEAVE_NOTIFY_MASK);

    g_signal_connect(G_OBJECT(gl_area), "key-press-event", G_CALLBACK(_key_press), NULL);
    g_signal_connect(G_OBJECT(gl_area), "key-release-event", G_CALLBACK(_key_release), NULL);
    g_signal_connect(G_OBJECT(gl_area), "button-press-event", G_CALLBACK(_button_press), NULL);
    g_signal_connect(G_OBJECT(gl_area), "button-release-event", G_CALLBACK(_button_release), NULL);
    g_signal_connect(G_OBJECT(gl_area), "motion-notify-event", G_CALLBACK(_motion_notify), NULL);
    g_signal_connect(G_OBJECT(gl_area), "scroll-event", G_CALLBACK(_mouse_wheel), NULL);

    g_signal_connect(G_OBJECT(gl_area), "show", G_CALLBACK(_parentSet), NULL);
    // g_signal_connect(G_OBJECT(gl_area), "grab-focus", G_CALLBACK(_InfocusGlArea), true);

    enum
    {
        TARGET_STRING,
        TARGET_URL
    };
    static GtkTargetEntry targetentries[] =
        {
            {"STRING", 0, TARGET_STRING},
            {"text/plain", 0, TARGET_STRING},
            {"text/uri-list", 0, TARGET_URL},
        };

    gtk_drag_dest_set(gl_area, GTK_DEST_DEFAULT_ALL, targetentries, 3,
                      GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
    g_signal_connect(G_OBJECT(gl_area), "drag-data-received", G_CALLBACK(_droppable), NULL);

    return gl_area;
}