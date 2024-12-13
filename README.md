# `trn` for Modern Linux

This is `trn` for Linux. It's based on `trn-4.0-test77.tar.gz`
from Sourceforge, plus all the patches from Debian.  I've put it together
to save the world the trouble, and so that I can have `trn` on
non-Debian systems.

The `trn-4.0-test77` branch has the original code, before any patches
were applied, should anyone want to see it.

To build this on Ubuntu 22.04, I ran `Configure` without `-d`.  I then
had to edit `config.h` to define `HAVE_TERMLIB` and in the `Makefile`
add `-ltermcap` to the `libs` variable.

This no longer builds with GCC 14, although `Configure` can run to
completion.  Looking at some of it, much of the code is _really_ old.
If anyone has time to modernize it, that'd be greatly appreciated.

Arnold Robbins
arnold@skeeve.com

#### Last Modified
Fri Dec 13 10:31:35 IST 2024
