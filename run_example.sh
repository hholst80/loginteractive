#!/bin/sh
env LD_PRELOAD=./loginteractive.so STDOUT=stdout-octave.txt octave -qi --traditional < stdin-octave.txt
env LD_PRELOAD=./loginteractive.so STDOUT=stdout-python.txt python < stdin-python.txt
