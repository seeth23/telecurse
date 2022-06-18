COMPILER=gcc

O=src
N=src/net

SOURCES=$(O)/main.c\
				$(O)/pc_error.c\
				$(O)/tc_window.c\
				$(O)/parse_conf.c\
				$(O)/input.c\
				$(O)/widget.c\
				$(O)/handlers/input_handlers.c\
				$(O)/handlers/menu_handlers.c\
				$(O)/math/center.c\
				$(O)/history.c\

DEST=tc
FLAGS=-Wall\
			-lpanel\
			-lncurses\
			-g\

all:
	$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)

debug:
	$(COMPILER) $(SOURCES) $(FLAGS) -D DEBUG -o debug_$(DEST)

