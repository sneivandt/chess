#ifndef UNITTEST_H
#define UNITTEST_H

#include "gtest/gtest.h"

namespace test {

class UnitTest : public testing::Test
{
  protected:
    std::stringstream buffer;

    virtual void SetUp()
    {
        std::cout.rdbuf(buffer.rdbuf());
    }

    virtual void TearDown()
    {
    }
};

} // namespace test

#endif