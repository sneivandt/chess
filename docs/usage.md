# ACE Usage Guide

This document explains how to build and run the ACE chess engine.

## Building the Project

ACE uses CMake as its build system. To build the project:

```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the executable
cmake --build .
```

## Running the Executable

After building, you can run the chess engine:

```bash
./src/main/ace
```

## Supported Commands

When running ACE in interactive mode, the following commands are available:

- **`uci`** - Switch to UCI mode for communication with chess GUIs
- **`n`** - Start a new game with the default starting position
- **`n <FEN>`** - Start a new game from a specific FEN position
- **`u`** - Undo the last move
- **`s`** - Search for the best move in the current position
- **`e`** - Evaluate the current position and display the score
- **`q`** - Quit the program
- **`<move>`** - Make a move using algebraic notation (e.g., `e2e4`, `e7e8q` for promotion)

## Move Notation

Moves should be entered in long algebraic notation:
- Standard moves: `e2e4` (from square e2 to e4)
- Promotions: `e7e8q` (pawn promotion to queen)
  - `q` = queen
  - `r` = rook
  - `b` = bishop
  - `n` = knight

## UCI Mode

To use ACE with a chess GUI that supports the UCI protocol, start the engine and enter the `uci` command. The engine will then communicate using standard UCI commands.
