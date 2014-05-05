libisatty
=========

`libisatty` is a small C hack using `LD_PRELOAD` to be able to log
interactive programs as if they where executed from a terminal.

usage
=====

You can use `libisatty` as follows:

	$ cat stdin.txt
	A=rand(2,2)
	v=eig(A)
	$ octave -qi < stdin.txt | tee stdout.txt
	>> A=rand(2,2)
	$ A =
	
	  0.86448  0.74905
	  0.30702  0.50105
	
	>> v=eig(A)
	v =
	
	  1.19560
	  0.16994
	
	>> <CTRL-D>
	$ cat stdout.txt >> A=rand(2,2)
	$ A =
	
	  0.86448  0.74905
	  0.30702  0.50105
	
	>> v=eig(A)
	v =
	
	  1.19560
	  0.16994
	
	>> 

known bugs
==========

I have not tested it with anything else than [GNU Octave] and [Python].
Please send bug reports to [Henrik Holst].

[Henrik Holst]: mailto:holst@matmech.com
[GNU Octave]: https://www.gnu.org/software/octave/
[Python]: https://www.python.org/
