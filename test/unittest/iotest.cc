#include "iotest.h"

#include "io/io.h"
#include "search/movegen.h"
#include "utils/exceptions.h"

#include <gtest/gtest.h>

using namespace test;

TEST_F(IoTest, parseMoveInvalidThrowsException)
{
    // Test that an invalid move throws AceException
    ASSERT_THROW(io::parseMove("invalid", pos), utils::AceException);
}

TEST_F(IoTest, parseMoveInvalidThrowsStdException)
{
    // Test that an invalid move can be caught as std::exception
    try {
        io::parseMove("z9z9", pos);
        FAIL() << "Expected AceException to be thrown";
    }
    catch (const std::exception& e) {
        ASSERT_STREQ(e.what(), "Invalid move");
    }
}

TEST_F(IoTest, parseMoveValid)
{
    // Test that a valid move does not throw
    ASSERT_NO_THROW(io::parseMove("e2e4", pos));
}
