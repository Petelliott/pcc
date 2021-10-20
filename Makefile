
CC=gcc
CFLAGS=-Isrc -Wall -Og -g

OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c src/*/*.c))

.PHONY: clean

pcc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(OBJS) pcc
