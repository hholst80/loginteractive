loginteractive
==============

LD_PRELOAD logging facility for interactive programs

Usage
=====

You can use `loginteractive` as follows:

	script -qc 'env LD_PRELOAD=./loginteractive.so octave -qi' > stdout.txt

Unfortunally, I have not had the time to fix the screen dependency.
If you know why the following causes a deadlock, please send [me] a mail.

[Me]: mailto:holst@matmech.com
