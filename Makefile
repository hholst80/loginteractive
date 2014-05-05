.PHONY: all clean

ALL = libisatty.so ansistrip

all: $(ALL)

clean:
	rm -f $(ALL)

libisatty.so: libisatty.c
	gcc -o $@ -shared $^

ansistrip: ansistrip.c
	gcc -o $@ $^
