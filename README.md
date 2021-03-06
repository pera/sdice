# The sdice passphrase generator

This is a simple passphrase generator written in C for Unix-like systems which uses [simple1024](https://github.com/pera/simple1024): a word list with 1024 English words between 3 and 6 characters, providing exactly 10 bits of entropy per word. This project previously used a slightly modified version of [EFF's Short Wordlist #1](https://www.eff.org/dice) which is still available in case you prefer it (see *eff_short1.h*).

The default optimization level of GCC should produce the expected binary, but you should always review the assembler output generated by your compiler, and most important, do not use this program if you don't understand its potential attack vectors; using a [real dice](https://en.wikipedia.org/wiki/Diceware) is usually safer :)

