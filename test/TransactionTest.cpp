#include "gtest/gtest.h"
#include "mock/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

class TransactionTest : public ::testing::Test {
protected:
    void SetUp() override {
	realFrom = new Account(1, 100);
        realTo = new Account(2, 200);
        
        mockFrom = new NiceMock<MockAccount>(1, 100);
        mockTo = new NiceMock<MockAccount>(2, 200);
        
        ON_CALL(*mockFrom, Lock()).WillByDefault(Invoke(realFrom, &Account::Lock));
        ON_CALL(*mockFrom, Unlock()).WillByDefault(Invoke(realFrom, &Account::Unlock));
        ON_CALL(*mockFrom, ChangeBalance(_)).WillByDefault(Invoke(realFrom, &Account::ChangeBalance));
        ON_CALL(*mockFrom, GetBalance()).WillByDefault(Invoke(realFrom, &Account::GetBalance));
        
        ON_CALL(*mockTo, Lock()).WillByDefault(Invoke(realTo, &Account::Lock));
        ON_CALL(*mockTo, Unlock()).WillByDefault(Invoke(realTo, &Account::Unlock));
        ON_CALL(*mockTo, ChangeBalance(_)).WillByDefault(Invoke(realTo, &Account::ChangeBalance));
        ON_CALL(*mockTo, GetBalance()).WillByDefault(Invoke(realTo, &Account::GetBalance));
    }
    
    void TearDown() override {
        delete mockFrom;
        delete mockTo;
        delete realFrom;
        delete realTo;
    }
    
    Account* realFrom;
    Account* realTo;
    MockAccount* mockFrom;
    MockAccount* mockTo;
};

TEST_F(TransactionTest, MakeInvalidAccounts) {
    Transaction tr;
    EXPECT_THROW(tr.Make(*mockFrom, *mockFrom, 100), std::logic_error);
}

TEST_F(TransactionTest, MakeNegativeSum) {
    Transaction tr;
    EXPECT_THROW(tr.Make(*mockFrom, *mockTo, -50), std::invalid_argument);
}

TEST_F(TransactionTest, MakeSmallSum) {
    Transaction tr;
    EXPECT_THROW(tr.Make(*mockFrom, *mockTo, 99), std::logic_error);
}

TEST(TransactionTest, MakeInsufficientFunds) {
    StrictMock<MockAccount> from(1, 100);
    StrictMock<MockAccount> to(2, 200);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(100));
    ON_CALL(to, GetBalance()).WillByDefault(Return(200));

    {
        InSequence seq;
        EXPECT_CALL(from, Lock()).Times(Exactly(1));
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(from, Unlock()).Times(Exactly(1));
    }

    {
        InSequence seq;
        EXPECT_CALL(to, Lock()).Times(Exactly(1));
        EXPECT_CALL(to, ChangeBalance(300)).Times(Exactly(1));  // Зачисление
        EXPECT_CALL(to, ChangeBalance(-300)).Times(Exactly(1)); // Откат
        EXPECT_CALL(to, GetBalance()).Times(Exactly(1));        // Для SaveToDataBase
        EXPECT_CALL(to, Unlock()).Times(Exactly(1));
    }

    EXPECT_CALL(from, ChangeBalance(_)).Times(0);

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTest, MakeSuccess) {
    StrictMock<MockAccount> from(1, 1000);
    StrictMock<MockAccount> to(2, 2000);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(1000));
    ON_CALL(to, GetBalance()).WillByDefault(Return(2000));

    {
        InSequence seq;
        EXPECT_CALL(from, Lock()).Times(Exactly(1));
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));
        EXPECT_CALL(from, ChangeBalance(-301)).Times(Exactly(1)); 
        EXPECT_CALL(from, GetBalance()).Times(Exactly(1));       
        EXPECT_CALL(from, Unlock()).Times(Exactly(1));
    }

    {
        InSequence seq;
        EXPECT_CALL(to, Lock()).Times(Exactly(1));
        EXPECT_CALL(to, ChangeBalance(300)).Times(Exactly(1)); 
        EXPECT_CALL(to, GetBalance()).Times(Exactly(1));       
        EXPECT_CALL(to, Unlock()).Times(Exactly(1));
    }

    EXPECT_TRUE(tr.Make(from, to, 300));
}
