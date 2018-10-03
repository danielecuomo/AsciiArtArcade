# ASCII_GAME
A pacman-like game that uses generic path-finding algorithms.
This abstraction allows a high independence from the data structures that encode the labyrinths.

Quick Game

Step #1 - install
apt-get install ncurses-dev

Step #2 - compile
gcc *.c -o ascii_game -lncurses -lm

Step #3 - execute
./asci_game level1 level2 level3
