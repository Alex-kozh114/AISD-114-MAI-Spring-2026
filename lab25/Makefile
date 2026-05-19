CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = program
OBJS = main.o udt.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c udt.h
	$(CC) $(CFLAGS) -c main.c

udt.o: udt.c udt.h
	$(CC) $(CFLAGS) -c udt.c

clean:
	rm -f $(OBJS) $(TARGET)
