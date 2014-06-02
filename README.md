Introduction
============

`loginteractive` is a small C hacks using `LD_PRELOAD` to be able to log
interactive programs as if they where executed from a terminal.

Usage
=====

You can use `loginteractive` as follows:

	$ cat stdin.txt
	A=rand(2,2)
	v=eig(A)
	$ env LD_PRELOAD=./loginteractive.so STDOUT=stdout.txt octave -qi < stdin.txt
	>> A=rand(2,2)
	A =
	  0.86448  0.74905
	  0.30702  0.50105
	
	>> v=eig(A)
	v =
	
	  1.19560
	  0.16994
	$ cat stdout.txt 
	>> A=rand(2,2)
	A =
	
	  0.86448  0.74905
	  0.30702  0.50105
	
	>> v=eig(A)
	v =
	
	  1.19560
	  0.16994
	
	$

The script `runexamples.sh` is included for demonstration.

There is an "undocumented feature" (pre-beta) to mimic
the behavior of `tail -f`. Set the variable

~~~~
static int g_follow = 1;
~~~~

to enable this feature.

Known bugs
==========

I have not tested `loginteractive` with anything else than [GNU Octave], and
[Python]. [Ruby] already has built-in support for interactive logging. Use
`irb --inspect < input > output` and not `loginteractive`, as that will just
mess it up. Please send bug reports to [Henrik Holst].

[Henrik Holst]: mailto:holst@matmech.com
[GNU Octave]: https://www.gnu.org/software/octave/
[Python]: https://www.python.org/
[Ruby]: http://www.ruby-lang.org/

