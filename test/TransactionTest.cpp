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

TEST_F(TransactionTest, MakeInsufficientFunds) {
    Transaction tr;
    
    EXPECT_FALSE(tr.Make(*mockFrom, *mockTo, 300));
    
    
    EXPECT_EQ(realFrom->GetBalance(), 100); 
    EXPECT_EQ(realTo->GetBalance(), 200);   
}

TEST_F(TransactionTest, MakeSuccess) {
    delete realFrom;
    delete realTo;
    delete mockFrom;
    delete mockTo;
    
    realFrom = new Account(1, 1000);
    realTo = new Account(2, 2000);
    mockFrom = new NiceMock<MockAccount>(1, 1000);
    mockTo = new NiceMock<MockAccount>(2, 2000);
    
    ON_CALL(*mockFrom, Lock()).WillByDefault(Invoke(realFrom, &Account::Lock));
    ON_CALL(*mockFrom, Unlock()).WillByDefault(Invoke(realFrom, &Account::Unlock));
    ON_CALL(*mockFrom, ChangeBalance(_)).WillByDefault(Invoke(realFrom, &Account::ChangeBalance));
    ON_CALL(*mockFrom, GetBalance()).WillByDefault(Invoke(realFrom, &Account::GetBalance));
    
    ON_CALL(*mockTo, Lock()).WillByDefault(Invoke(realTo, &Account::Lock));
    ON_CALL(*mockTo, Unlock()).WillByDefault(Invoke(realTo, &Account::Unlock));
    ON_CALL(*mockTo, ChangeBalance(_)).WillByDefault(Invoke(realTo, &Account::ChangeBalance));
    ON_CALL(*mockTo, GetBalance()).WillByDefault(Invoke(realTo, &Account::GetBalance));
    
    Transaction tr;
    EXPECT_TRUE(tr.Make(*mockFrom, *mockTo, 300));
    
    EXPECT_EQ(realFrom->GetBalance(), 1000 - 300 - tr.fee()); 
EXPECT_EQ(realTo->GetBalance(), 2000 + 300);             
}
