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

SERVER_SOURCES=$(N)/error.c\
							 $(N)/server.c\

DEST=bins

FLAGS=-Wall\
			-lpanel\
			-lncurses\
			-g\

SERVER_FLAGS=-Wall\
							-g\

all: test_dir telecurse tcserver

clean:
	@if [ -d "$(DEST)" ]; then \
		rm -r "$(DEST)"
	fi

debug:
	$(COMPILER) $(SOURCES) $(FLAGS) -D DEBUG -o $(DEST)/debug_tc

telecurse:
	@$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)/tc; \

tcserver:
	@$(COMPILER) $(SERVER_SOURCES) $(SERVER_FLAGS) -o $(DEST)/tcserver

test_dir:
	@if ! [ -d "$(DEST)" ]; then \
		mkdir "$(DEST)"; \
	fi
