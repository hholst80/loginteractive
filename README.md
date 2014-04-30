loginteractive
==============

This is a small C hack using `LD_PRELOAD` to be able to log interactive
programs as if they where executed from a terminal.

Usage
=====

You can use `loginteractive` as follows:

    env STDIN=stdin-octave.txt         \
        STDOUT=stdout-octave.txt       \
        LD_PRELOAD=./loginteractive.so \
        octave -qi

The `STDIN` and `STDOUT` arguments are optional. The default values are

    STDIN=stdin.txt STDOUT=stdout.txt

Disclaimer
==========

I have not tested it with anything else than [GNU Octave] and [Python].
Please send bug reports to [Me].

[Me]: mailto:holst@matmech.com
[GNU Octave]: https://www.gnu.org/software/octave/
[Python]: https://www.python.org/
