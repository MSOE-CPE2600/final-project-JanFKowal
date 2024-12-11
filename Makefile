CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
SOURCES=chat1.c chat2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLES=chat1 chat2

all: $(EXECUTABLES)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

chat1: chat1.o
	$(CC) chat1.o $(LDFLAGS) -o $@

chat2: chat2.o
	$(CC) chat2.o $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES)
