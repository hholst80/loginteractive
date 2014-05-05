#!/bin/sh
HACK="env LD_PRELOAD=./libisatty.so"
$HACK octave -qi --traditional < stdin-octave.txt | ./ansistrip > stdout-octave.txt
$HACK python < stdin-python.txt | ./ansistrip > stdout-python.txt
