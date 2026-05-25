# ACE - A Chess Engine

ACE is a C++20 chess engine with a command-line interface and UCI (Universal Chess Interface) support for chess GUIs. It is organized as a small, modular engine focused on correct board state handling, move generation, search, and testable behavior.

## Highlights

| Area | Status |
| --- | --- |
| Engine protocol | UCI plus an interactive CLI mode |
| Board model | 120-square mailbox board with pawn bitboards |
| Search | Alpha-beta search with iterative deepening support |
| Tests | GoogleTest unit tests and perft move-generation checks |
| Build | CMake presets for debug, release, and sanitizer builds |
| AI assistance | Copilot instructions, AGENTS.md, and local agent skills |

## Repository layout

| Path | Purpose |
| --- | --- |
| `src/board/` | Board representation, FEN parsing, move making/unmaking, move encoding, evaluation |
| `src/search/` | Move generation, move lists, principal variation table, search state, search loop |
| `src/io/` | UCI protocol handling and text I/O helpers |
| `src/main/` | Executable entry point and interactive command loop |
| `src/utils/` | Shared utilities and exception helpers |
| `test/unittest/` | Fast component-level GoogleTest suites |
| `test/perfttest/` | Perft positions used to validate legal move generation |
| `docs/` | Usage and architecture notes |
| `.github/skills/` | Repository-local AI agent skills for ACE workflows |

## Prerequisites

- A C++20 compiler: GCC, Clang, or MSVC
- CMake 3.25 or newer
- Git

GoogleTest is resolved by CMake through `FetchContent` when tests are enabled.

## Quick start

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
./build-dev/src/main/ace
```

The `dev` preset creates a Debug build in `build-dev/` with warnings enabled but not promoted to errors.

## Build presets

```bash
cmake --list-presets
```

| Preset | Use |
| --- | --- |
| `dev` | Debug build for local development |
| `prod` | Release build with warnings as errors |
| `asan` | Debug build with AddressSanitizer and UndefinedBehaviorSanitizer |
| `tsan` | Debug build with ThreadSanitizer |

Build with any preset:

```bash
cmake --preset prod
cmake --build --preset prod
```

## Testing

Run all tests for a configured preset:

```bash
ctest --preset dev
```

Run targeted tests:

```bash
ctest --test-dir build-dev -R unittest --output-on-failure
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

`PERFT_MAX_DEPTH` limits perft depth for quick local and CI runs. If it is not set, perft defaults to depth 6.

## Running ACE

Interactive CLI mode:

```bash
./build-dev/src/main/ace
```

Common commands:

| Command | Action |
| --- | --- |
| `n` | Reset to the starting position |
| `n <fen>` | Load a specific FEN position |
| `e2e4` | Make a move in long algebraic coordinate notation |
| `s` | Start searching the current position; press Enter to stop |
| `u` | Undo the last half-move |
| `e` | Print the static evaluation |
| `uci` | Switch to UCI protocol mode |
| `q` | Quit |

To use a GUI, configure it to launch the built `ace` executable as a UCI engine. With XBoard:

```bash
xboard -fcp ./build-dev/src/main/ace -fUCI
```

## Documentation

- [Usage guide](docs/usage.md)
- [Architecture overview](docs/architecture.md)
- [Test suite guide](test/README.md)
- [Contributing guide](.github/CONTRIBUTING.md)

## AI-assisted development

This repository includes AI guidance for coding agents:

- `.github/copilot-instructions.md` captures project conventions and validation expectations.
- `AGENTS.md` provides a concise repository operating guide for AI agents and contributors.
- `.github/skills/` contains local Agent Skills for ACE-specific workflows.

When using an AI agent, ask it to avoid unrelated engine changes, preserve chess-rule correctness, and run the smallest relevant CMake/CTest validation.

## Contributing

Contributions are welcome. Please read [.github/CONTRIBUTING.md](.github/CONTRIBUTING.md), keep changes scoped, and include tests or documentation updates when behavior or workflows change.
