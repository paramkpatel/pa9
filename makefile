CC=gcc
FLAG=-Wall -Werror -std=c11
OBJ=3d.o
DEPS=3d.h

generator: generator.c $(OBJ) $(DEPS)
	$(CC) $(FLAG) generator.c $(OBJ) -o generator

$(OBJ): 3d.c $(DEPS)
	$(CC) $(FLAG) -c 3d.c -o $(OBJ)

clean:
	rm -f generator $(OBJ)