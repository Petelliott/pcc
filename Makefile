
CC=gcc
CFLAGS=-Isrc -Wall -Og -g

OBJS=$(patsubst %.c,%.o,$(wildcard src/*.c src/*/*.c))
TEST_OBJS=$(patsubst %.c,%.o,$(wildcard test/*.c test/*/*.c))


pcc: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

test/test: $(filter-out src/main.o,$(OBJS)) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean check

check: test/test
	$<

clean:
	rm -rf $(OBJS) $(TEST_OBJS) pcc
