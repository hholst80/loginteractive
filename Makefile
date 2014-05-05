.PHONY: all clean

ALL = loginteractive.so libisatty.so ansistrip

all: $(ALL)

clean:
	rm -f $(ALL)

%.so: %.c
	gcc -o $@ -shared -std=c99 $^

ansistrip: ansistrip.c
	gcc -o $@ $^
