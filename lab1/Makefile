CC = gcc
CFLAGS = -g -ggdb -Wall -Wextra -O2

GOALS = ui prog
LIBS = -lncurses -lcdk


.PHONY: all clean rebuild


all: $(GOALS)


ui: ui.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

prog: prog.c
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -f $(GOALS)

rebuild: clean all
