# GitHub Copilot Instructions for ACE

ACE is a C++20 chess engine with an interactive CLI and UCI protocol support. Keep changes scoped, preserve chess-rule correctness, and avoid unrelated rewrites.

## Project facts

- Language: C++20
- Build system: CMake 3.25+
- Test framework: GoogleTest
- Main executable: `ace`
- Recommended local build directory: `build-dev/`

## Architecture

| Path | Responsibility |
| --- | --- |
| `src/board/` | Board representation, FEN parsing, move encoding, move making/unmaking, static evaluation |
| `src/search/` | Move generation, move lists, search state, alpha-beta search, principal variation table |
| `src/io/` | UCI protocol parsing and text I/O |
| `src/main/` | CLI entry point |
| `src/utils/` | Shared helpers and exceptions |
| `test/unittest/` | Fast unit and regression tests |
| `test/perfttest/` | Perft move-generation validation |

The board uses a 120-square mailbox representation with 64-square conversion helpers. Pawn structures also use bitboards for white, black, and both colors.

## Style conventions

| Item | Style | Example |
| --- | --- | --- |
| Namespaces | `lower_snake_case` | `board` |
| Classes/structs | `PascalCase` | `Board` |
| Functions/methods | `camelCase` | `makeMove` |
| Variables | `camelCase` | `moveList` |
| Constants/enums | `UPPER_SNAKE_CASE` | `DEFAULT_FEN` |

Use RAII, const-correctness, typed enums where practical, `constexpr` for compile-time constants, and standard library containers over raw arrays unless the existing board representation requires otherwise.

## Validation commands

Prefer the smallest command that covers the change:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Targeted runs:

```bash
ctest --test-dir build-dev -R unittest --output-on-failure
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

Sanitizers:

```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset asan
```

## Change guidance

- For board, move generation, or make/undo changes, add or update unit tests and run perft where relevant.
- For UCI or CLI changes, add parser/command tests when possible and document user-visible command changes.
- For CMake, CI, docs, or AI-support changes, do not modify engine source unless the supporting change requires it.
- Keep generated or local build artifacts out of commits.
- Use the local skills in `.github/skills/` when they match the requested task.
