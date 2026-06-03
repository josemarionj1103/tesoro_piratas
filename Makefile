# Makefile para compilar la version visual con Raylib en MinGW/Windows

CC = g++
CFLAGS = -Wall -std=c++14 -O2
LDFLAGS = -lraylib -lgdi32 -lwinmm

all: tesoro_visual

tesoro_visual: visual_main.cpp estructuras.h
	$(CC) visual_main.cpp -o tesoro_visual.exe $(CFLAGS) $(LDFLAGS)

clean:
	rm -f tesoro_visual.exe
