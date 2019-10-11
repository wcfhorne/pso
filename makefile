
CC = gcc
FLG = -Wall -Wextra
LIB = -lm
OBJ = prng.o pso.o main.o

all: $(OBJ)
	$(CC) $(FLG) $(OBJ) $(LIB) -o pso

main.o: main.c
	$(CC) $(FLG) $(LIB) -c $^ -o $@

prng.o: prng.c
	$(CC) $(FLG) $(LIB) -c $^ -o $@

pso.o: pso.c
	$(CC) $(FLG) $(LIB) -c $^ -o $@

clean:
	rm $(OBJ) pso
