.PHONY: all clean

all: loginteractive.so

clean:
	rm -f loginteractive.so

loginteractive.so: loginteractive.c
	gcc -o $@ -shared $^
