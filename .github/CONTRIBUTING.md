# Contributing to ACE

Thanks for helping improve ACE. This guide covers repository workflow, validation commands, and expectations for changes.

## Prerequisites

- C++20 compiler: GCC, Clang, or MSVC
- CMake 3.25 or newer
- Git

Optional but useful:

- VS Code with the recommended extensions from `.vscode/extensions.json`
- Visual Studio with CMake Presets support
- `clang-format` for the `clang-format` CMake target
- `clang-tidy` or `include-what-you-use` for optional static-analysis builds

## Recommended workflow

1. Create a branch from `main`.
2. Configure and build with the smallest relevant preset.
3. Add or update tests for behavior changes.
4. Update docs when commands, workflows, or user-visible behavior change.
5. Open a pull request with a short explanation of the change and validation run.

## Build and test

Use CMake presets for normal development:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Use the release preset before changes that may affect warnings or portability:

```bash
cmake --preset prod
cmake --build --preset prod
ctest --preset prod
```

Run targeted tests while iterating:

```bash
ctest --test-dir build-dev -R unittest --output-on-failure
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

## Sanitizers and static analysis

Address/undefined behavior sanitizer build:

```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset asan
```

Thread sanitizer build:

```bash
cmake --preset tsan
cmake --build --preset tsan
ctest --preset tsan
```

Optional clang-tidy configuration:

```bash
cmake -B build-tidy -S . -DCLANG_TIDY=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-tidy
```

Optional include-what-you-use configuration:

```bash
cmake -B build-iwyu -S . -DINCLUDE_WHAT_YOU_USE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-iwyu
```

## Coding style

- Use C++20 and keep code portable across GCC, Clang, and MSVC.
- Use 4 spaces for C++ indentation.
- Place public headers in `src/<module>/include/<module>/`.
- Prefer clear types, `constexpr` constants, RAII, and const-correct APIs.
- Keep engine changes small and covered by unit or perft tests.
- Do not mix unrelated refactors with behavior changes.

Naming conventions:

| Item | Style | Example |
| --- | --- | --- |
| Namespaces | `lower_snake_case` | `board` |
| Classes/structs | `PascalCase` | `Board` |
| Functions/methods | `camelCase` | `makeMove` |
| Variables | `camelCase` | `moveList` |
| Constants/enums | `UPPER_SNAKE_CASE` | `DEFAULT_FEN` |

## Test guidance

- Unit tests belong in `test/unittest/`.
- Perft coverage belongs in `test/perfttest/perft.txt` when validating move-generation positions.
- Use FEN strings to make board setup explicit.
- Keep tests independent; do not rely on execution order.
- Prefer targeted `ctest -R` runs during development, then run the relevant full preset before submitting.

## AI-assisted contributions

The repository includes `.github/copilot-instructions.md`, `AGENTS.md`, and local Agent Skills under `.github/skills/`. AI-assisted changes should follow the same rules as manual changes: scoped diffs, no unrelated rewrites, and validation with CMake/CTest commands that cover the modified behavior.

## Pull request checklist

- The change is scoped to one purpose.
- Tests or docs were updated where appropriate.
- The smallest relevant validation command passed locally.
- The PR description explains what changed and why.
