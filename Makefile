CC=gcc
CFLAGS=-Wall -Wextra -ggdb

OBJS= src/jlexer.o src/gen.o src/jstring.o

jsgen: src/main.c $(OBJS)
	$(CC) $< -o $@ $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

.phony: clean valgrind tests/run tests/snap

tests/snap:
	@./scripts/test.py snap . jsgen

tests/run:
	@./scripts/test.py run . jsgen

valgrind/run: jsgen
	valgrind -s --track-origins=yes ./jsgen --file valgrind_test.json

clean:
	rm jsgen src/*.o