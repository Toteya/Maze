# The Maze

This project has been created and submitted as a Portfolio Project for the ALX SE Foudations program.

The entire project has been written in C. At its core, it uses the Simple DirectMedia Layer 2.0 (SDL2) development library.

## Table of Contents

- [Background](#background)
- [Install](#install)
- [Run](#run)
- [Controls](#controls)
- [Gameplay](#gameplay--rules)
- [Maps](#maps)
- [Contributors](#contributors)
- [Resources](#resources-and-references)

## Background



## Install

To compile this code the following dependacies are required:

[SDL2](https://wiki.libsdl.org/SDL2/FrontPage)

```sh
sudo apt-get install libsdl2-dev 
```

[SDL2_image](https://wiki.libsdl.org/SDL2_image/FrontPage)


```sh
# search the apt-get cache to find the latest SDL_image version
$ sudo apt-cache search libsdl2-image

# install the SDL_image package
$ sudo apt-get install libsdl2-image-dev
```

Use the Makefile to compile the code

```sh
$ make
```

Otherwise, compile manually with gcc. Be sure to include the relevant flags for SDL2, SDL_image andd Math libraries.
`` `sdl2-config --cflags --libs\` -lSDL2 -lSDL2_image -lm``

## Run

Use the Makefile to run the program:

```sh
$ make run
```

## Controls

The arrow keys to move the player `FORWARD`, `BACKWARD`, `LEFT`, and `RIGHT`.

The `A` and `D` keys rotate the player `LEFT` and `RIGHT`.

## Gameplay + Rules

The player starts at the start line.

Use the controls to navigate the player through the maze and find the finish line.

## Maps

The map is stored as a `.CSV` file.

The map is read and loaded as a 2-dimensional array (`map[HEIGHT][WIDTH]`) where the index of each element represents a coordinate on the grid.

The value of each element indicates the presence or absence of a wall. `0` means there is no wall. `1` is for a normal wall texture. Other values specify different wall textures.

## Contributors

Created by [Toteya Kamanja](https://github.com/Toteya)

## Resources and References

- Textures: [pexels.com](https://www.pexels.com/)
