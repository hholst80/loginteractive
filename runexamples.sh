#!/bin/sh

echo
echo " *** GNU Octave *** "
echo
env LD_PRELOAD=./loginteractive.so STDOUT=stdout-octave.txt octave -qi --traditional < stdin-octave.txt

echo
echo " *** Python *** "
echo
env LD_PRELOAD=./loginteractive.so STDOUT=stdout-python.txt python < stdin-python.txt

# Ruby already has interactive support built in.
# Trying to use LD_PRELOAD=./loginteractive.so will just mess it up!
echo
echo " *** Ruby *** "
echo
irb --simple-prompt < stdin-ruby.txt | tee stdout-ruby.txt
