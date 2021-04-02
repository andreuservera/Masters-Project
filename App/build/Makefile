CC = gcc
VPATH = ../Client:../json:../switch
CFLAGS = -I../Client -I../json -I../switch
CFLAGS += -std=c11 -Wpedantic -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wconversion -Wshadow -Wcast-qual -Wnested-externs #-fsanitize=address -fno-omit-frame-pointer -g
CFLAGS += $(mysql_config --cflags) -MMD
#CFLAGS += $(pkg-config --cflags mysqlclient) -MMD
LDFLAGS = $(mysql_config --libs) -lmysqlclient -lm
#LDFLAGS = $(pkg-config --libs) -lmysqlclient -lm
OBJECTS = Main.o parser.o utils.o json.o switch.o

all: Main

Main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

-include $(OBJECTS:.o=.d)

clean:
	rm -f *.o *.d Main

