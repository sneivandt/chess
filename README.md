# ACE - A Chess Engine

A UCI chess engine.

## Build

    make

## XBoard

Start a game using XBoard and the PolyGlot UCI adapter.

    xboard -fcp ./ace -fUCI

## CLI Mode

Enter moves in algebraic notation or an action key for special actions.

    ./ace

Key | Action
----|-------
n   | New Game
q   | Quit
u   | Undo
s   | Search
e   | Evaluate
p   | Perft Test
