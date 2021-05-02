# gtk-raylib
Es una pequeña biblioteca para crear una incrustación de raylib en gtk+3.x.

- Gtk+-3.x (OpenGL 3.3)
- LibEpoxy : https://github.com/anholt/libepoxy
- Raylib 3.x : https://github.com/raysan5/raylib

# Compilar gtk-raylib

**Windows && dependencias Msys2(msys2-x86_64)**
- pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-libepoxy

**Linux**
- sudo apt install libgtk-3-dev libepoxy-dev

**Mac**
- No probado

1 - python -m pip install scons
2 - scons -Q

# Compilar ejemplos
- cd examples
- scons -Q
