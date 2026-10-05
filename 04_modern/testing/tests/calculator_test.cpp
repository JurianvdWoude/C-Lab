#include <gtest/gtest.h>
#include "calculator.h"

TEST(CalculatorTest, AddsTwoNumbers) {
  EXPECT_EQ(add(2, 3), 5);
}

TEST(CalculatorTest, DividesTwoNumbers) {
  EXPECT_EQ(divide(6, 3), 2);
}

TEST(CalculatorTest, AddsTwoNumbersWrong) {
  EXPECT_EQ(add(2, 3), 500);
}


