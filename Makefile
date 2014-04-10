.PHONY: all clean

all: loginteractive.so

clean: loginteractive.so
	rm -f $^

loginteractive.so: loginteractive.c
	gcc -o $@ -shared $^ -std=c99
