#include "gtest/gtest.h"
#include "mock/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::InSequence;
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
    Transaction tr;

    // Для from
    {
        InSequence s;
        EXPECT_CALL(from, Lock()).Times(1);
        // GetBalance вызывается внутри Debit()
        EXPECT_CALL(from, GetBalance()).WillOnce(Return(100));
        // ChangeBalance не вызывается, так как средств недостаточно
        EXPECT_CALL(from, Unlock()).Times(1);
    }

    // Для to
    {
        InSequence s;
        EXPECT_CALL(to, Lock()).Times(1);
        EXPECT_CALL(to, ChangeBalance(300)).Times(1);  // Зачисление
        EXPECT_CALL(to, ChangeBalance(-300)).Times(1); // Откат из-за недостатка средств
        EXPECT_CALL(to, Unlock()).Times(1);
    }

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTest, MakeSuccess) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);
    Transaction tr;

    // Для from
    {
        InSequence s;
        EXPECT_CALL(from, Lock()).Times(1);
        // GetBalance вызывается внутри Debit()
        EXPECT_CALL(from, GetBalance()).WillOnce(Return(1000));
        EXPECT_CALL(from, ChangeBalance(-301)).Times(1); // 300 + комиссия 1
        EXPECT_CALL(from, Unlock()).Times(1);
    }

    // Для to
    {
        InSequence s;
        EXPECT_CALL(to, Lock()).Times(1);
        EXPECT_CALL(to, ChangeBalance(300)).Times(1); // Зачисление
        EXPECT_CALL(to, Unlock()).Times(1);
    }

    EXPECT_TRUE(tr.Make(from, to, 300));
}
