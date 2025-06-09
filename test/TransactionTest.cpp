#include "gtest/gtest.h"
#include "mock/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;

TEST(TransactionTest, MakeInvalidAccounts) {
    testing::StrictMock<MockAccount> acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTest, MakeNegativeSum) {
    testing::StrictMock<MockAccount> acc1(1, 1000);
    testing::StrictMock<MockAccount> acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTest, MakeSmallSum) {
    testing::StrictMock<MockAccount> acc1(1, 1000);
    testing::StrictMock<MockAccount> acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTest, MakeInsufficientFunds) {
    testing::StrictMock<MockAccount> from(1, 100);
    testing::StrictMock<MockAccount> to(2, 200);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(100));
    ON_CALL(to, GetBalance()).WillByDefault(Return(200));

    {
        testing::InSequence seq;
        EXPECT_CALL(from, Lock()).Times(Exactly(1));
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(from, Unlock()).Times(Exactly(1));
    }

    {
        testing::InSequence seq;
        EXPECT_CALL(to, Lock()).Times(Exactly(1));
        EXPECT_CALL(to, ChangeBalance(300)).Times(Exactly(1));
        EXPECT_CALL(to, ChangeBalance(-300)).Times(Exactly(1));
        EXPECT_CALL(to, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(to, Unlock()).Times(Exactly(1));
    }

    EXPECT_CALL(from, ChangeBalance(_)).Times(0);

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTest, MakeSuccess) {
    testing::StrictMock<MockAccount> from(1, 1000);
    testing::StrictMock<MockAccount> to(2, 2000);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(1000));
    ON_CALL(to, GetBalance()).WillByDefault(Return(2000));

    {
        testing::InSequence seq;
        EXPECT_CALL(from, Lock()).Times(Exactly(1));
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(from, ChangeBalance(-301)).Times(Exactly(1));
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(from, Unlock()).Times(Exactly(1));
    }

    {
        testing::InSequence seq;
        EXPECT_CALL(to, Lock()).Times(Exactly(1));
        EXPECT_CALL(to, ChangeBalance(300)).Times(Exactly(1));
        EXPECT_CALL(to, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(to, Unlock()).Times(Exactly(1));
    }

    EXPECT_TRUE(tr.Make(from, to, 300));
}
