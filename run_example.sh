#!/bin/sh

env LD_PRELOAD=./loginteractive.so \
    STDIN=stdin-octave.txt         \
    STDOUT=stdout-octave.txt       \
    octave -qi

env LD_PRELOAD=./loginteractive.so \
    STDIN=stdin-python.txt         \
    STDOUT=stdout-python.txt       \
    python
