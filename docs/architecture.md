# ACE Architecture

ACE is split into small CMake targets that mirror the engine's runtime responsibilities. The core executable, `ace`, links the board, search, I/O, and utility modules.

## Module map

| Module | Responsibility |
| --- | --- |
| `src/board/` | Board state, FEN parsing, move encoding, move making/unmaking, static evaluation |
| `src/search/` | Legal move generation, move lists, search state, principal variation table, alpha-beta search |
| `src/io/` | UCI command handling and move text parsing |
| `src/utils/` | Shared utility functions and exception types |
| `src/main/` | Interactive CLI loop and executable startup |

## Board representation

The board uses a 120-square mailbox representation. The playable 8x8 board is embedded in a larger array so off-board detection can be handled with sentinel squares. Helper mappings convert between 64-square and 120-square indexing when needed.

Pawn structure also uses bitboards:

- `pawns[WHITE]`
- `pawns[BLACK]`
- `pawns[BOTH]`

The board module owns position state, including side to move, castling rights, en passant state, material, piece lists, and history needed for undo.

## Move flow

1. A position is loaded from FEN or from the default starting position.
2. Move generation produces pseudo-legal and legal moves for the current side.
3. `makeMove` applies candidate moves and rejects illegal moves that leave the side in check.
4. Search evaluates legal move trees and can return the best move through UCI or interactive output.
5. `undoMove` restores prior board state for search and user undo operations.

## Search flow

Search is implemented in `src/search/` and centers on alpha-beta pruning with iterative deepening support. Search-related state is kept separate from board state so the engine can repeatedly make and undo moves while preserving timing and principal variation information.

## I/O and UCI

The I/O layer parses user commands and UCI protocol messages. Interactive mode is intentionally simple and useful for debugging positions manually. UCI mode is the integration path for chess GUIs.

## Tests

Tests are grouped by purpose:

| Test target | Purpose |
| --- | --- |
| `unittest` | Fast checks for board, move, search, evaluation, I/O, and UCI behavior |
| `perfttest` | Move-generation correctness checks against known perft node counts |

Use perft tests when changing move generation or move making. Use unit tests for localized behavior and regression coverage.
