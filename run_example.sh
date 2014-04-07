#!/bin/sh
env LD_PRELOAD=./loginteractive.so STDIN=stdin-octave.txt octave -qi
env LD_PRELOAD=./loginteractive.so STDIN=stdin-python.txt python
