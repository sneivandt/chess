# ACE - A Chess Engine 

[![Build Status](https://travis-ci.org/sneivandt/chess.svg?branch=master)](https://travis-ci.org/sneivandt/chess)

## Build

```
cd build
cmake ..
cmake --build .
```

## Test

Run unit tests.

```
cd build
cmake ..
cmake --build .
ctest -R unittest --output-on-failure
```

Run a perft test.

```
cd build
cmake ..
cmake --build .
ctest -R perfttest --verbose
```

## Install

```
cd build
cmake ..
sudo cmake --build . --target install
```

## Play

Start a game using XBoard and the PolyGlot UCI adapter.

```
cd build
cmake ..
cmake --build .
xboard -fcp ./src/main/ace -fUCI
```

Start a game using the CLI.

```
cd build
cmake ..
cmake --build .
./src/main/ace
```

Enter moves in algebraic notation or an action key for special actions.

Key | Action
----|-------
n   | New Game
q   | Quit
u   | Undo
s   | Search
e   | Evaluate
