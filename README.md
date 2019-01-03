# ACE - A Chess Engine

## Build

```
cd build
cmake ..
cmake --build .
```

## Test

```
cd build
cmake ..
cmake --build . --target perft
ctest perft --verbose
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
xboard -fcp ./bin/ace -fUCI
```

Start a game using the CLI.

```
cd build
cmake ..
cmake --build .
./bin/ace
```

Enter moves in algebraic notation or an action key for special actions.

Key | Action
----|-------
n   | New Game
q   | Quit
u   | Undo
s   | Search
e   | Evaluate
