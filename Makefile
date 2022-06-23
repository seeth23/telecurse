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
DEBUG_DEST=$(DEST)/debug

FLAGS=-Wall\
		-lpanel\
		-lncurses\
		-g\

SERVER_FLAGS=-Wall\
		-g\

all: dest_dir telecurse tcserver

PHONY: clean
clean:
	@if [ -d "$(DEBUG_DEST)" ]; then \
		rm -r "$(DEBUG_DEST)"; \
		echo "Removed $(DEBUG_DEST)"; \
	fi
	@if [ -d "$(DEST)" ]; then \
		rm -r "$(DEST)"; \
		echo "Removed $(DEST)/"; \
	else \
		echo "Nothing to clean"; \
	fi

.PHONY: install
install:

.PHONY: debug
debug: dest_dir debug_dir
	@$(COMPILER) $(SOURCES) $(FLAGS) -D DEBUG -o $(DEBUG_DEST)/debug_tc
	@$(COMPILER) $(SERVER_SOURCES) $(SERVER_FLAGS) -D DEBUG -o $(DEBUG_DEST)/debug_tcserver

telecurse:
	@$(COMPILER) $(SOURCES) $(FLAGS) -o $(DEST)/tc

tcserver:
	@$(COMPILER) $(SERVER_SOURCES) $(SERVER_FLAGS) -o $(DEST)/tcserver

dest_dir:
	@if ! [ -d "$(DEST)" ]; then \
		mkdir "$(DEST)"; \
		echo "Created $(DEST)/"; \
	fi

debug_dir:
	@if ! [ -d "$(DEBUG_DEST)" ]; then \
		mkdir "$(DEBUG_DEST)"; \
		echo "Created $(DEBUG_DEST)"; \
	fi
