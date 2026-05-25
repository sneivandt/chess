# ACE Test Suite

The test suite uses GoogleTest and is split into fast unit tests plus perft tests for move-generation correctness.

## Layout

| Path | Purpose |
| --- | --- |
| `unittest/` | Component-level tests for board state, move encoding, move making, move generation, search, evaluation, I/O, and UCI parsing |
| `perfttest/` | Perft runner and known positions in `perft.txt` |

## Run tests

Configure and build first:

```bash
cmake --preset dev
cmake --build --preset dev
```

Run all tests:

```bash
ctest --preset dev
```

Run unit tests only:

```bash
ctest --test-dir build-dev -R unittest --output-on-failure
```

Run perft tests only:

```bash
PERFT_MAX_DEPTH=4 ctest --test-dir build-dev -R perfttest --output-on-failure
```

List GoogleTest cases:

```bash
./build-dev/test/unittest/unittest --gtest_list_tests
```

Run a specific suite or case:

```bash
./build-dev/test/unittest/unittest --gtest_filter=BoardTest.*
./build-dev/test/unittest/unittest --gtest_filter=BoardTest.parseFenStartPos
```

## Perft depth

`perfttest` reads `PERFT_MAX_DEPTH` from the environment. Use a lower value, such as `4`, for quick validation and CI. Leave it unset when you want the default deeper run.

## Writing tests

Use FEN strings to make position setup explicit:

```cpp
#include "board/board.h"
#include "gtest/gtest.h"
#include "unittest.h"

class MyTest : public test::UnitTest
{
protected:
    board::Board pos;

    void SetUp() override
    {
        UnitTest::SetUp();
        board::Board::INIT();
    }
};

TEST_F(MyTest, HandlesPosition)
{
    pos.parseFen(board::Board::DEFAULT_FEN);

    EXPECT_EQ(pos.getSide(), board::toInt(board::Color::WHITE));
}
```

Guidelines:

- Keep each test focused on one behavior.
- Cover normal cases, edge cases, and chess-rule regressions.
- Add perft positions for move-generation changes that affect legal move counts.
- Keep tests independent; do not rely on execution order.
- Update this README when adding new test workflows or targets.
