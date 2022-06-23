COMPILER=gcc
FLAGS=-Wall -g
OUTPUT=tcserver
SRC=server.c\
		error.c

all:
	$(COMPILER) $(FLAGS) -o $(OUTPUT) $(SRC)

debug:
	$(COMPILER) $(FLAGS) -D DEBUG -o $(OUTPUT) $(SRC)
