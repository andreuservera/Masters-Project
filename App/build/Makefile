CC = gcc
VPATH = ../Client
CFLAGS = -I../Client
CFLAGS += -std=c11 -Wpedantic -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wshadow -Wcast-qual -Wnested-externs #-fsanitize=address -fno-omit-frame-pointer -O2
CFLAGS += $(mysql_config --cflags) -MMD
#CFLAGS += $(pkg-config --cflags mysqlclient) -MMD
LDFLAGS = $(mysql_config --libs) -lmysqlclient -lm
#LDFLAGS = $(pkg-config --libs) -lmysqlclient -lm
OBJECTS = ReadFile.o parser.o utils.o 

all: ReadFile

ReadFile: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

-include $(OBJECTS:.o=.d)

clean:
	rm -f *.o *.d ReadFile

