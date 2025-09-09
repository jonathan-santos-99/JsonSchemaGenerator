CC=gcc
CFLAGS=-Wall -Wextra

OBJS= src/jlexer.o src/gen.o src/jstring.o

jsgen: src/main.c $(OBJS)
	$(CC) $< -o $@ $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) $< -o $@ -c $(CFLAGS)

.phony: clean valgrin tests/run tests/snap

tests/snap:
	@./scripts/test.py snap . jsgen

tests/run:
	@./scripts/test.py run . jsgen

valgrin: jsgen
	valgrind --track-origins=yes ./jsgen '{"teste": 1}'

clean:
	rm jsgen src/*.o