COMPILER=gcc

O=src
H=$(O)/handlers
M=$(O)/math
N=$(O)/net

SOURCES=$(O)/main.c\
				$(O)/pc_error.c\
				$(O)/tc_window.c\
				$(O)/parse_conf.c\
				$(O)/input.c\
				$(O)/widget.c\
				$(H)/input_handlers.c\
				$(H)/menu_handlers.c\
				$(M)/center.c\
				$(O)/history.c\
				$(O)/misc.c\
				$(O)/client.c\

DEST=tc
FLAGS=-Wall\
			-lpanel\
			-lncurses\
			-g\

all:
	$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)

debug:
	$(COMPILER) $(SOURCES) $(FLAGS) -D DEBUG -o debug_$(DEST)

