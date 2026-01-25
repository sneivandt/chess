# A Chess Engine (ACE)

ACE is a robust, C++20 chess engine designed for performance and correct play. It implements the UCI (Universal Chess Interface) protocol, allowing it to be used with popular chess GUIs like Arena, Fritz, or XBoard/WinBoard (via PolyGlot).

## Project Structure

The project is organized into modular components:

*   **`src/board`**: Core board representation, piece definitions, and evaluation logic. Uses a 120-square array approach for efficient move generation and off-board detection.
*   **`src/search`**: Search algorithms (Alpha-Beta, Iterative Deepening) and move generation logic (`movegen.cc`).
*   **`src/io`**: UCI protocol implementation and console I/O handling.
*   **`src/main`**: Application entry point.
*   **`src/utils`**: General utility functions.
*   **`test/`**: Comprehensive unit tests and perft (performance) tests using GoogleTest.

## Build

### Standard

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Using CMake Presets

This project uses `CMakePresets.json` to provide common build configurations.

**List available presets:**
```bash
cmake --list-presets
```

**Common Presets:**
*   `dev` - Debug build with warnings.
*   `prod` - Release build with warnings as errors.
*   `asan` - Debug build with Address Sanitizer and Undefined Behavior Sanitizer.
*   `tsan` - Debug build with Thread Sanitizer.

**Build using a preset:**
```bash
cmake --preset [preset_name]
cmake --build --preset [preset_name]
```

**Run tests using a preset:**
```bash
ctest --preset [preset_name]
```

## Test

### Unit Tests
Run the suite of functional tests to ensure engine correctness.

```bash
cd build
ctest -R unittest --output-on-failure
```

### Performance (Perft) Tests
Verify the move generator by counting leaf nodes at various depths.

```bash
cd build
ctest -R perfttest --verbose
```

## Install

```bash
cd build
sudo cmake --build . --target install
```

## Play

### Using a GUI (XBoard)
Start a game using XBoard and the PolyGlot UCI adapter.

```bash
xboard -fcp ./build/src/main/ace -fUCI
```

### CLI Mode
The engine can be run directly in the terminal for manual analysis or debugging. Note that **by default, the engine acts as a digital chessboard**. It will not make moves automatically; you must trigger the search command manually if you want the AI to calculate a move for the current side.

```bash
./build/src/main/ace
```

**Controls:**
Enter moves in **Long Algebraic Notation** (e.g., `e2e4`, `e7e8q` for promotion).

| Command | Action | Details |
|---------|--------|---------|
| `s` | Search | Starts an infinite search for the best move. **Press Enter to stop.** *Note: The keypress stops the search but is consumed; if you typed a command to stop it, re-type it.* |
| `n` | New Game | Resets the board to the starting position. |
| `n [fen]`| Set FEN | Sets the board to a specific position defined by a FEN string. |
| `u` | Undo | Undoes the last half-move. |
| `e` | Evaluate | Prints the static evaluation score of the current position (centipawns). |
| `uci` | UCI Mode | Switches to UCI protocol mode (for connecting to GUIs). |
| `q` | Quit | Exits the application. |

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to build, test, and submit changes to ACE.

