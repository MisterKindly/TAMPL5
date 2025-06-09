#include "gtest/gtest.h"
#include "mock/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::Return;

TEST(TransactionTest, MakeInvalidAccounts) {
    MockAccount acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTest, MakeNegativeSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTest, MakeSmallSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTest, MakeInsufficientFunds) {
    MockAccount from(1, 100);
    MockAccount to(2, 200);
    Transaction tr;

    EXPECT_CALL(from, GetBalanceMock()).WillOnce(Return(100));
    EXPECT_CALL(to, ChangeBalanceMock(300)).Times(1);
    EXPECT_CALL(to, ChangeBalanceMock(-300)).Times(1);

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTest, MakeSuccess) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);
    Transaction tr;

    EXPECT_CALL(from, GetBalanceMock()).WillOnce(Return(1000));
    EXPECT_CALL(from, ChangeBalanceMock(-301)).Times(1);
    EXPECT_CALL(to, ChangeBalanceMock(300)).Times(1);

    EXPECT_TRUE(tr.Make(from, to, 300));
}
