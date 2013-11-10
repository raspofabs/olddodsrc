THE DOD SRC

This is an attempt to write a game (or at least a something) in C++ to help
illustrate the differences between Object Oriented Design and Data Oriented
Design.

This game is based on the source code hurriedly put together for the Godus
prototype, then pulled apart to make a tetris clone, then torn apart again to
start work on this project. If something has a funny name, it's bound to be
because of that.

As the project progresses, I'm going to try to keep track of what the game is
in the DESIGN.txt, hopefully remembering to actually document it as it is
developed and putting in the git hashes of pertinent blobs that reflect the
design goal being achieved (or set, I don't know) so people can see what
changes when designs change. I'm going to build the game pretty normally to
start with, then do what normally happens in games and demand new exciting
features that mean lots of refactoring, which should show up some of the
differences in DOD vs OOP in daily use as a language that includes maintenance
and bug fixing.

To start with, the game was written in as basic a way as possible, but from the
second design onwards, the game will be written out in two different ways.
There will be refactoring of code in both branches, but both implementations
will stick to the design in their own way. At each design checkpoint, the
gameplay should be completely indistinguishable.
