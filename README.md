# gtk-raylib
Es una pequeña biblioteca para crear una incrustación de raylib en gtk+3.x.

- Gtk+-3.x (OpenGL 3.3)
- LibEpoxy : https://github.com/anholt/libepoxy
- Raylib 3.x : https://github.com/raysan5/raylib

**Ahora estoy utilizando Scons para el compilado, pronto estare actualizando a Raylib 3.7.**

# Dependencias

python -m pip install scons

**Windows && dependencias Msys2(msys2-x86_64)**
- pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-libepoxy

**Linux**
- sudo apt install libgtk-3-dev libepoxy-dev

**Mac**
- Aun no probado

# Compilar gtk-raylib
- scons -Q

# Compilar ejemplos
- cd examples
- scons -Q
