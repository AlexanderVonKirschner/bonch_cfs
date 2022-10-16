CC = gcc
CFLAGS = -g -ggdb -Wall -Wextra -O2 -fno-strict-aliasing

GOALS = gui tui prog
LIBS = -lncurses -lcdk

.PHONY: all clean rebuild


all: $(GOALS)


tui: tui.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

prog: prog.c
	$(CC) $(CFLAGS) $^ -o $@

gui: gui.cpp
	g++ -Wall -Wextra $^ -lfltk -o $@


clean:
	rm -f $(GOALS)

rebuild: clean all
