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

Arnold Robbins
arnold@skeeve.com

#### Last Modified
Sat 11 Nov 2023 20:19:59 IST
