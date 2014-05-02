loginteractive
==============

`loginteractive` is a small C hack using `LD_PRELOAD` to be able to log
interactive programs as if they where executed from a terminal.

usage
=====

You can use `loginteractive` as follows:

    env STDIN=stdin-octave.txt         \
        STDOUT=stdout-octave.txt       \
        LD_PRELOAD=./loginteractive.so \
        octave -qi

The `STDIN` and `STDOUT` arguments are optional. The default values are

    STDIN=/dev/stdin STDOUT=stdout.txt

known bugs
==========

I have not tested it with anything else than [GNU Octave] and [Python].
Please send bug reports to [Henrik Holst].

The libc `select` subroutine seems to work for Python but will probably break
somewhere down the line when this is more tested.

[Henrik Holst]: mailto:holst@matmech.com
[GNU Octave]: https://www.gnu.org/software/octave/
[Python]: https://www.python.org/
