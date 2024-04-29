PRG = chelper

C_FILES = stats.c
H_FILES = stats.h

CFLAGS = -Wall -rdynamic -z execstack
FILES = $(C_FILES)
bin/$(PRG): $(FILES)
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG)
