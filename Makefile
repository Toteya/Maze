build:
	gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -g src/*.c -o maze.out `sdl2-config --cflags --libs` -lSDL2 -lm -lSDL2_image

run:
	./maze.out

clean:
	rm ./maze.out
