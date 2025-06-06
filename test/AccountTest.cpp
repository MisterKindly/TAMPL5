#include "gtest/gtest.h"
#include "mock/MockAccount.h"

using ::testing::_;

TEST(AccountTest, GetBalance) {
    Account acc(123, 1000);
    EXPECT_EQ(acc.GetBalance(), 1000);
}

TEST(AccountTest, LockUnlock) {
    Account acc(123, 1000);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(100));
    acc.Unlock();
    EXPECT_THROW(acc.ChangeBalance(100), std::runtime_error);
}

TEST(AccountTest, ChangeBalanceWithoutLock) {
    Account acc(123, 1000);
    EXPECT_THROW(acc.ChangeBalance(100), std::runtime_error);
}

TEST(AccountTest, DoubleLock) {
    Account acc(123, 1000);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}
