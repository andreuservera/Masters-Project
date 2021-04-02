CC = gcc
CFLAGS = -I../utils
CFLAGS += -std=c11 -Wpedantic -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wshadow -Wcast-qual -Wnested-externs
VPATH = ../utils
OBJECTS = main.o utils.o json.o

all: json

main.o: utils.h json.h
utils.o: utils.h
json.o: json.h

json: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o json -lm

clean:
	rm -f *.o json
