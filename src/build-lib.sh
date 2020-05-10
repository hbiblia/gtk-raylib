
LIBS=`pkg-config --libs --cflags gtk+-3.0 epoxy`
CFLAGS="-Iraylib/ -Iinclude/"

# RAYLIB
gcc -w -c raylib/core.c $CFLAGS -lm -DPLATFORM_EMBED -DBUILD_LIBTYPE_SHARED -DGRAPHICS_API_OPENGL_33 -DEPOXY
gcc -w -c raylib/shapes.c $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
gcc -w -c raylib/textures.c $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
gcc -w -c raylib/text.c $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
gcc -w -c raylib/utils.c $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
gcc -w -c raylib/models.c $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
# gcc -W -c raylib/raudio.c $LIBS $CFLAGS -lm -D -DPLATFORM_EMBED

#GTK
gcc -w -c gtkraylib.c $LIBS $CFLAGS -lm -DBUILD_LIBTYPE_SHARED

#LIB build
gcc -w  -shared -o gtkraylib.dll utils.o core.o shapes.o textures.o text.o models.o gtkraylib.o $LIBS $CFLAGS -lm -DBUILD_LIBTYPE_SHARED
cp gtkraylib.dll ../examples/ 