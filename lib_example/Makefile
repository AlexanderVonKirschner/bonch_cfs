LIBFILE = lib.sh
TESTFILE = test.sh

.PHONY: all install test

all: install

install: $(LIBFILE)
	chmod +x $< && sh $<

test: $(TESTFILE)
	chmod +x $< && ./$< "String for test"
