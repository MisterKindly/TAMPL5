#include "gtest/gtest.h"
#include "mock/MockAccount.h"
#include "../src/Transaction.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class LoggingMockAccount : public MockAccount {
public:
    LoggingMockAccount(int id, int balance) : MockAccount(id, balance), id_(id) {}
    
    void Lock() override {
        std::cout << "DEBUG [MockAccount " << id_ << "]: Lock()" << std::endl;
        MockAccount::Lock();
    }
    
    void Unlock() override {
        std::cout << "DEBUG [MockAccount " << id_ << "]: Unlock()" << std::endl;
        MockAccount::Unlock();
    }
    
    void ChangeBalance(int diff) override {
        std::cout << "DEBUG [MockAccount " << id_ << "]: ChangeBalance(" << diff << ")" << std::endl;
        MockAccount::ChangeBalance(diff);
    }
    
    int GetBalance() const override {
        int balance = MockAccount::GetBalance();
        std::cout << "DEBUG [MockAccount " << id_ << "]: GetBalance() = " << balance << std::endl;
        return balance;
    }

private:
    int id_;
};

TEST(TransactionTest, MakeInvalidAccounts) {
    LoggingMockAccount acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTest, MakeNegativeSum) {
    LoggingMockAccount acc1(1, 1000);
    LoggingMockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTest, MakeSmallSum) {
    LoggingMockAccount acc1(1, 1000);
    LoggingMockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTest, MakeInsufficientFunds) {
    LoggingMockAccount from(1, 100);
    LoggingMockAccount to(2, 200);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(100));
    ON_CALL(to, GetBalance()).WillByDefault(Return(200));
    
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(to, ChangeBalance(-300)).Times(1);
    EXPECT_CALL(from, ChangeBalance(_)).Times(0);

    std::cout << "===== TEST START: MakeInsufficientFunds =====" << std::endl;
    EXPECT_FALSE(tr.Make(from, to, 300));
    std::cout << "===== TEST END: MakeInsufficientFunds =====" << std::endl;
}

TEST(TransactionTest, MakeSuccess) {
    LoggingMockAccount from(1, 1000);
    LoggingMockAccount to(2, 2000);
    Transaction tr;

    ON_CALL(from, GetBalance()).WillByDefault(Return(1000));
    ON_CALL(to, GetBalance()).WillByDefault(Return(2000));
    
    EXPECT_CALL(from, ChangeBalance(-301)).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);

    std::cout << "===== TEST START: MakeSuccess =====" << std::endl;
    EXPECT_TRUE(tr.Make(from, to, 300));
    std::cout << "===== TEST END: MakeSuccess =====" << std::endl;
}
