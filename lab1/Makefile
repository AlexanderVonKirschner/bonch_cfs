CC = gcc
CFLAGS = -g -ggdb -Wall -Wextra

GOALS = ui prog
LIBS = -lncurses -lcdk


.PHONY: all clean


all: $(GOALS)


ui: ui.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

prog: prog.c
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -f $(GOALS)
