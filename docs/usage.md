# ACE Usage Guide

This guide explains how to build, test, and run ACE locally.

## Build

The recommended path is CMake presets:

```bash
cmake --preset dev
cmake --build --preset dev
```

The built executable is:

```bash
./build-dev/src/main/ace
```

For a release-style build:

```bash
cmake --preset prod
cmake --build --preset prod
```

Manual builds also work:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/src/main/ace
```

## Test

Run all tests for the development preset:

```bash
ctest --preset dev
```

Run only unit tests:

```bash
ctest --test-dir build-dev -R unittest --output-on-failure
```

Run only perft tests with a quicker local depth:

```bash
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

If `PERFT_MAX_DEPTH` is not set, the perft runner defaults to depth 6.

## Interactive CLI mode

Start ACE directly:

```bash
./build-dev/src/main/ace
```

The interactive mode acts like a manual chessboard until you ask it to search.

| Command | Description |
| --- | --- |
| `uci` | Switch to UCI protocol mode |
| `n` | Start a new game from the default position |
| `n <FEN>` | Start a new game from a specific FEN position |
| `u` | Undo the last half-move |
| `s` | Search the current position until Enter is pressed |
| `e` | Print the static evaluation score |
| `q` | Quit |
| `<move>` | Make a move in coordinate notation, such as `e2e4` or `e7e8q` |

Promotion suffixes are lowercase piece letters:

| Suffix | Promotion |
| --- | --- |
| `q` | Queen |
| `r` | Rook |
| `b` | Bishop |
| `n` | Knight |

## UCI mode and GUIs

ACE supports UCI so it can be launched by chess GUIs. Configure the GUI to start the built executable, for example:

```bash
xboard -fcp ./build-dev/src/main/ace -fUCI
```

You can also enter UCI mode manually from the CLI by running ACE and typing:

```text
uci
```

## Useful development commands

```bash
cmake --list-presets
cmake --build --preset asan
ctest --preset asan
cmake --build --preset dev --target clang-format
```
