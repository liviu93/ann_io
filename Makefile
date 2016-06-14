CC = gcc
SRC = reader.c
OBJ = reader
FLAGS = -Wall -g -lm

all:
	$(CC) $(FLAGS) -o $(OBJ) $(SRC)
	./$(OBJ) -i datain
