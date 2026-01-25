# Contributing to ACE

Thank you for your interest in contributing to ACE (A Chess Engine)!

## Getting Started

### Prerequisites
- C++20 compliant compiler (GCC, Clang, MSVC)
- CMake 4.2 or higher

### Development Environment (VS Code)
We recommend using **Visual Studio Code** for development. The project includes configuration for:
- **Dev Containers**: A consistent development environment with all dependencies pre-installed. Open the project in VS Code and click "Reopen in Container" when prompted.
- **Tasks**: Pre-configured tasks for building and testing. Press `Ctrl+Shift+B` to build, or run `Task: Run Task` > `test`.
- **Debugging**: Launch configurations for debugging the engine and tests are provided in `Run and Debug` view.

### Building the Project

**Using CMake Presets (Recommended):**

We provide CMake presets for common development configurations (Debug, Release, Sanitizers).

1.  **List Presets**: `cmake --list-presets`
2.  **Configure**: `cmake --preset dev` (or `asan` for sanitizers)
3.  **Build**: `cmake --build --preset dev`
4.  **Test**: `ctest --preset dev`

**Manual Build:**

1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure with CMake:
   ```bash
   cmake ..
   ```
3. Build:
   ```bash
   cmake --build .
   ```

## Development Workflow

1.  **Fork** the repository.
2.  Create a **Feature Branch** (`git checkout -b feature/AmazingFeature`).
3.  Commit your changes.
4.  **Test** your changes using the test suite.
5.  Push to the branch.
6.  Open a **Pull Request**.

## Testing

We use [GoogleTest](https://github.com/google/googletest) for unit testing.

### Running Unit Tests
```bash
cd build
ctest -R unittest --output-on-failure
```

### Running Perft Tests
Performance tests verify the move generator's correctness.
```bash
cd build
ctest -R perfttest --verbose
```

## Coding Style
- **Indentation**: Use 4 spaces.
- **Header Files**: Place public headers in `src/<module>/include/<module>/` to ensure proper namespacing.
- **Unit Tests**: Ensure all new logic is covered by unit tests in `test/unittest/`.

### Naming Conventions
- **Namespaces**: `lower_snake_case` (e.g., `board`, `search`, `utils`)
- **Classes/Structs**: `PascalCase` (e.g., `Board`, `MoveGen`)
- **Functions/Methods**: `camelCase` (e.g., `makeMove`, `parseFen`)
- **Variables**: `camelCase` (e.g., `side`, `moveList`)
- **Constants/Enums**: `UPPER_SNAKE_CASE` (e.g., `DEFAULT_FEN`, `PIECE_VAL`)
- **Member Variables**: No specific prefix like `m_` or `_`. Use `camelCase`.

## Code of Conduct
Please be respectful and considerate of clear and constructive feedback.
