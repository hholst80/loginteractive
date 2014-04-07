loginteractive
==============

LD_PRELOAD logging facility for interactive programs

Usage
=====

You can use `loginteractive` as follows:

	env STDIN=stdin-octave.txt \
            LD_PRELOAD=./loginteractive.so \
            octave -qi

or if you wish to save stdout:

	script -qc '...' | tee stdout.txt

Unfortunally, I have not had the time to fix the script dependency.
If you know why the following causes a deadlock, please send [me] a mail.

[Me]: mailto:holst@matmech.com
