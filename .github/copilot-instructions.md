# GitHub Copilot Instructions for ACE (A Chess Engine)

## Project Overview
ACE is a Chess Engine written in C++17. It supports the UCI (Universal Chess Interface) protocol.

## Coding Standards
- **Language**: C++20
- **Build System**: CMake 4.2+
- **Testing Framework**: GoogleTest (gtest/gmock)

### Naming Conventions
- **Namespaces**: `lower_snake_case` (e.g., `board`, `search`, `utils`)
- **Classes/Structs**: `PascalCase` (e.g., `Board`, `MoveGen`)
- **Functions/Methods**: `camelCase` (e.g., `makeMove`, `parseFen`)
- **Variables**: `camelCase` (e.g., `side`, `moveList`)
- **Constants/Enums**: `UPPER_SNAKE_CASE` (e.g., `DEFAULT_FEN`, `PIECE_VAL`)
- **Member Variables**: No specific prefix like `m_` or `_`.

## Architecture & Data Structures

### Board Representation
- The board uses a **120-square array representation** (0-119) to handle off-board detection easily.
- The 8x8 board is centered within this array.
- Helper arrays `SQ64` and `SQ120` convert between 64-square (0-63) and 120-square indices.
- **Bitboards** are used specifically for Pawn structures (`pawns[WHITE]`, `pawns[BLACK]`, `pawns[BOTH]`).

### Move Generation
- Move generation logic is located in `src/search/movegen.cc`.
- Moves are often represented as integers or simple structs.

### Search
- Alpha-Beta pruning is used.
- Iterative Deepening.
- Transposition Tables (if implemented).

## Directory Structure
- `src/`: Source code modules.
    - `board/`: Board representation, move making, evaluation.
    - `search/`: Search algorithms, move generation.
    - `io/`: UCI protocol handling, I/O.
    - `utils/`: Common utilities.
    - `main/`: Entry point.
- `test/`: Unit tests and perft tests.
- `extern/`: Third-party libraries (GoogleTest).

## Common Tasks

### Adding a new feature
1. Implement logic in the appropriate `src/` module.
2. Add unit tests in `test/unittest/`.
3. Ensure CMake configuration is updated if new files are added.

### Running Tests
Use `ctest` in the build directory:
- Unit tests: `ctest -R unittest`
- Performance tests: `ctest -R perfttest`
