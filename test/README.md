# ACE Test Suite

This directory contains the comprehensive test suite for the ACE chess engine.

## Test Organization

### Unit Tests (`unittest/`)
Focused, fast tests for individual components:
- **boardtest.cc**: Board representation, FEN parsing, square attacks
- **makemovetest.cc**: Move execution and undo (quiet moves, captures, castling, en passant, promotions)
- **movegentest.cc**: Move generation for all piece types and special moves
- **movetest.cc**: Move encoding/decoding and string representation
- **evaluatetest.cc**: Position evaluation, material, piece square tables
- **searchtest.cc**: Search algorithms, repetition detection, search info management
- **iotest.cc**: Move parsing and I/O operations
- **ucitest.cc**: UCI protocol parsing

### Performance Tests (`perfttest/`)
Validate move generation correctness by counting leaf nodes at various depths.

## Running Tests

### All Unit Tests
```bash
cd build
ctest -R unittest --output-on-failure
```

### Specific Test Suite
```bash
cd build
./test/unittest/unittest --gtest_filter=BoardTest.*
./test/unittest/unittest --gtest_filter=EvaluateTest.*
```

### Single Test Case
```bash
cd build
./test/unittest/unittest --gtest_filter=BoardTest.parseFenStartPos
```

### Performance Tests
```bash
cd build
ctest -R perfttest --verbose
```

## Test Coverage

Current test coverage by module:
- **Board**: 19 tests covering FEN parsing, square attacks, and move making/unmaking
- **Move**: 12 tests for move encoding, decoding, and string representation
- **Move Generation**: 9 tests for all move types including special moves
- **Evaluation**: 11 tests for position scoring and heuristics
- **Search**: 6 tests for search infrastructure
- **I/O**: 3 tests for move parsing
- **UCI**: 3 tests for UCI protocol handling

**Total: 63 unit tests**

## Writing Tests

### Test Structure
```cpp
#include "gtest/gtest.h"
#include "board/board.h"
#include "unittest.h"

using namespace test;

class MyTest : public test::UnitTest
{
protected:
    board::Board pos;
    
    virtual void SetUp()
    {
        UnitTest::SetUp();
        board::Board::INIT();
    }
};

TEST_F(MyTest, TestName)
{
    // Arrange
    pos.parseFen("fen string here");
    
    // Act
    int result = someFunction(pos);
    
    // Assert
    EXPECT_EQ(result, expectedValue);
}
```

### Best Practices
1. **One assertion per test**: Each test should focus on one specific behavior
2. **Descriptive names**: Test names should clearly describe what is being tested
3. **Arrange-Act-Assert**: Structure tests with clear setup, execution, and verification
4. **Independent tests**: Tests should not depend on each other
5. **Use FEN strings**: Set up board positions using FEN for clarity and brevity
6. **Test edge cases**: Cover boundary conditions, special moves, and error cases

### FEN Position Examples
```cpp
// Start position
pos.parseFen(board::Board::DEFAULT_FEN);

// Custom position
pos.parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

// En passant available
pos.parseFen("rnbqkbnr/ppp2ppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
```

## Continuous Integration

Tests are automatically run on every pull request and commit to ensure code quality.

## Adding New Tests

When adding new functionality:
1. Write tests first (TDD approach recommended)
2. Ensure tests cover normal cases, edge cases, and error conditions
3. Run all tests to ensure no regressions
4. Update this README if adding a new test file

## Test Dependencies

- **GoogleTest**: Modern C++ testing framework
- **CMake**: Build system integration
- All tests link against the relevant library modules (board, search, io, utils)
