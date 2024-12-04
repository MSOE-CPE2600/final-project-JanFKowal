CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES=Chat1.c Chat2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLES=Chat1 Chat2

all: $(EXECUTABLES)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

Chat1: Chat1.o
	$(CC) Chat1.o $(LDFLAGS) -o $@

Chat2: Chat2.o
	$(CC) Chat2.o $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES)
