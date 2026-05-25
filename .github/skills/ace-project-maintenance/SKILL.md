---
name: ace-project-maintenance
description: Use when improving ACE supporting files such as README, docs, CMake presets, GitHub Actions, VS Code config, Copilot instructions, or Agent Skills.
---

# ACE Project Maintenance

Use this skill for non-core project improvements. Supporting files include documentation, CMake presets, GitHub Actions workflows, editor settings, repository metadata, and AI-agent guidance.

## Workflow

1. Check the current repository facts before editing docs: CMake version, preset names, build directories, executable paths, and test target names.
2. Keep support changes consistent across `README.md`, `.github/CONTRIBUTING.md`, `docs/`, `.github/copilot-instructions.md`, and `AGENTS.md`.
3. Prefer CMake presets in documented commands.
4. Keep CI aligned with local presets.
5. Do not modify engine source files unless a supporting workflow is broken because of source layout changes.

## Validation

For docs-only changes, validate command accuracy with lightweight checks where practical:

```bash
cmake --list-presets
ctest --test-dir build-dev -N
```

For CMake preset or CI command changes:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Documentation standards

- Lead with quick-start commands.
- Use tables for command and directory references.
- Avoid stale exact test counts unless they are generated automatically.
- Document `PERFT_MAX_DEPTH=4` for quick perft runs.
