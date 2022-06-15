COMPILER=gcc

O=src
N=src/net

SOURCES=$(O)/main.c\
				$(O)/pc_error.c\
				$(O)/tc_window.c\
				$(O)/parse_conf.c\
				$(O)/input.c\
				$(O)/widget.c\

DEST=tc
FLAGS=-Wall \
			-lncurses

all:
	$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)
