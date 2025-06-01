#include "gtest/gtest.h"
#include "mocks/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

TEST(TransactionTest, MakeInvalidAccounts) {
    NiceMock<MockAccount> acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTest, MakeNegativeSum) {
    NiceMock<MockAccount> acc1(1, 1000);
    NiceMock<MockAccount> acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTest, MakeSmallSum) {
    NiceMock<MockAccount> acc1(1, 1000);
    NiceMock<MockAccount> acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTest, MakeInsufficientFunds) {
    MockAccount from(1, 100);
    MockAccount to(2, 200);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(from, GetBalance()).WillOnce(Return(100));
    EXPECT_CALL(from, ChangeBalance(302)).Times(0);
    EXPECT_CALL(to, ChangeBalance(-300)).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    Transaction tr;
    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTest, MakeSuccess) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(from, GetBalance()).WillOnce(Return(1000));
    EXPECT_CALL(from, ChangeBalance(-302)).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);

    Transaction tr;
    EXPECT_TRUE(tr.Make(from, to, 300));
}
