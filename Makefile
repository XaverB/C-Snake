PROGRAM = tetris
ARGUMENTS =

CC = gcc
CFLAGS = -pedantic -Wall -Wextra -std=c11 -ggdb3

LD = gcc
LDFLAGS = -lallegro -lallegro_font -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_dialog

CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

all: depend $(PROGRAM)
$(PROGRAM): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

.PHONY: clean run debug
clean:
	rm -f $(OFILES) $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM) $(ARGUMENTS)

debug: $(PROGRAM)
	gdb $(PROGRAM) $(ARGUMENTS)


DEPENDFILE = .depend
depend:
	$(CC) $(CCFLAGS) -MM $(CFILES) > $(DEPENDFILE)
-include $(DEPENDFILE)