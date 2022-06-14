COMPILER=gcc
SOURCES=src/main.c\

DEST=main
FLAGS=-Wall \
			-lncurses

all:
	$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)
