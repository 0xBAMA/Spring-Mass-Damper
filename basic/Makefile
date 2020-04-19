CXX?=c++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
CXXFLAGS?=-std=c++11 #-Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
INCLUDE_SDL_TTF=-I /usr/local/lib

.PHONY: all msg clean fullclean

all: msg main

msg:
	@echo 'Compiling'

#main: main.cc
#	${CXX} ${CXXFLAGS} -O3 -lSDL2  -o $@ $< ${SDL2FLAGS}

main: main.cc graph.h includes.h
	${CXX} ${CXXFLAGS} -O3 -lSDL2 -o main main.cc ${SDL2FLAGS}

run: msg main
	time ./main

clean:
	rm -f main *.o main.asm

fullclean: clean
