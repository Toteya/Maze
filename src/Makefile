build:
	gcc -Wall -Werror -Wextra -pedantic -std=gnu89 main.c initialise.c poll_events.c -o maze `sdl2-config --cflags --libs` -lSDL2

run:
	./maze

clean:
	rm maze
