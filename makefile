
CC = gcc
FLG = -Wall -Wextra
OBJ = prng.o pso.o main.o

all: $(OBJ)
	$(CC) $(FLG) $(OBJ) -o pso

main.o: main.c
	$(CC) $(FLG) -c $^ -o $@

prng.o: prng.c
	$(CC) $(FLG) -c $^ -o $@

pso.o: pso.c
	$(CC) $(FLG) -c $^ -o $@

clean:
	rm $(OBJ) pso
