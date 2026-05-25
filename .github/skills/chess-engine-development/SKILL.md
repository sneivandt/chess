---
name: chess-engine-development
description: Use when changing ACE engine behavior, including board state, move making, move generation, evaluation, search, or UCI integration.
---

# Chess Engine Development

Use this skill for ACE core engine changes. ACE is a C++20 chess engine with a 120-square mailbox board, pawn bitboards, GoogleTest tests, and CMake presets.

## Workflow

1. Identify the affected module: `board`, `search`, `io`, `main`, or `utils`.
2. Read the relevant headers and tests before editing implementation files.
3. Keep the change scoped to one behavior or bug.
4. Add or update unit tests in `test/unittest/` for localized behavior.
5. Run perft when move generation or make/undo behavior changes.

## Validation

Use the smallest relevant validation command:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --test-dir build-dev -R unittest --output-on-failure
```

For move-generation or make/undo changes:

```bash
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

For warning-sensitive or release-readiness changes:

```bash
cmake --preset prod
cmake --build --preset prod
ctest --preset prod
```

## Guardrails

- Do not change chess rules without regression coverage.
- Do not rewrite unrelated modules while fixing a localized issue.
- Preserve existing public headers and target structure unless the task requires a broader refactor.
- Prefer FEN-based tests for board positions.
