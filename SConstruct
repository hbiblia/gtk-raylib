import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})

custom_src = [
    'src/gtk-raylib.c',
    'src/raylib/core.c',
    'src/raylib/shapes.c',
    'src/raylib/textures.c',
    'src/raylib/text.c',
    'src/raylib/utils.c',
    'src/raylib/models.c',
    # 'src/raylib/raudio.c'
]

env.Append(CPPPATH = ['src/include','src/raylib/'])
env.Append(CPPDEFINES = [
    '-DBUILD_LIBTYPE_SHARED',
    '-DGRAPHICS_API_OPENGL_33',
    '-DPLATFORM_EMBED',
    '-DEPOXY'
    ])
env.Append(LIBS=['m','epoxy'])

env.ParseConfig('pkg-config --cflags --libs gtk+-3.0 epoxy')

env.Library('gtk-raylib', custom_src)
