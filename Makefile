TARGET = os-project
CFLAGS = -O3 -Wall
CC = gcc

all: $(TARGET)

$(TARGET): file.o main.o
	$(CC) $(CFLAGS) -o $(TARGET).x *.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c

run: ./$(TARGET).x
	./$(TARGET).x $(ARGS)

clean:
	rm *.x *.o