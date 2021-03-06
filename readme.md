# Wordle solver

Solve the wordle game and its many variants.

## Currently implemented variants

* The classic [wordle](https://www.nytimes.com/games/wordle/index.html)
* Variants of wordle that play multiple boards at the same time, including
    * [dordle](https://zaratustra.itch.io/dordle)
    * [quordle](https://www.quordle.com/)
    * [octordle](https://octordle.com/?mode=daily)
* Wordle with equations: [nerdle](https://nerdlegame.com/)
* Puzzle to find wordle attempts that led to a solution: [crosswordle](https://crosswordle.vercel.app/?daily=1)
* 5x5 Wordle board where all vertical and horizontal words need to be valid: [squareword](https://squareword.org/)

## Building

Assuming you are on a linux-like system

```
make
```

## Usage

**To play wordle and its clones:**

```
./wordle
```

The program will provide guesses. Play them manually on the website and enter
the response as a five-letter string such as "BGYGB" (B-Black, G-Green,
Y-Yellow)

The multi-board wordle clones can be played similarly, such as

```
./dordle
```

To play an arbitrary number of simultaneous wordle boards with an arbirary
number of attempts, you can try

```
./wordle -b 4 -a 13
```

**To play nerdle:**

Play nerdle the same way with `./nerdle`, except the colours are now B-Black,
G-Green, R-Red.

**To play crosswordle:**

To play crosswordle with `./crosswordle`, provide the responses from top to
bottom such as `BYBBB YBBGB … GGGGG` followed by the actual word. See
`data/crosswordle_sample.in` for an example

**To play squareword**

Play with `./squareword`. For each row, give the response as a five-letter
string with G-Green and B-Black, followed by another string of remaining
characters on the right (if empty, give -)
