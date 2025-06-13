#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/Account.h"
#include "../src/Transaction.h"
#include "mock/MockAccount.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Ref;

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(TransactionTests, MakeInvalidAccounts) {
    MockAccount acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTests, MakeNegativeSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTests, MakeSmallSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTests, MakeSuccess) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);
    MockTransaction tr;

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(from, ChangeBalance(-301)).Times(1);
    EXPECT_CALL(from, GetBalance()).Times(1).WillOnce(Return(1000));
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(tr, SaveToDataBase(Ref(from), Ref(to), 300)).Times(1);

    EXPECT_TRUE(tr.Make(from, to, 300));
}

TEST(TransactionTests, MakeInsufficientFunds) {
    MockAccount from(1, 100);
    MockAccount to(2, 200);
    MockTransaction tr;

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(to, ChangeBalance(-300)).Times(1);
    EXPECT_CALL(from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(from, GetBalance()).Times(1).WillOnce(Return(100));
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(tr, SaveToDataBase(Ref(from), Ref(to), 300)).Times(1);

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTests, DatabaseSaving)
{
    const int val1 = 200;
    const int val2 = 250;
    MockAccount account1(1,val1);
    MockAccount account2(2,val2);
    Transaction transaction;
    const int sum = 150;

    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account2, Lock()).Times(1);
    EXPECT_CALL(account2, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, GetBalance()).Times(2).WillOnce(Return(val1)).WillOnce(Return(val1 - sum - transaction.fee()));
    EXPECT_CALL(account2, GetBalance()).Times(1).WillOnce(Return(val2 + sum));
    EXPECT_CALL(account1, Unlock()).Times(1);
    EXPECT_CALL(account2, Unlock()).Times(1);
    
    testing::internal::CaptureStdout();
    transaction.Make(account1, account2, sum);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "1 send to 2 $150\nBalance 1 is 49\nBalance 2 is 400\n");

}

TEST(TransactionTests, MakeFeeTooHigh) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);
    MockTransaction tr;
    
    tr.set_fee(80);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(to, ChangeBalance(_)).Times(0);
    EXPECT_CALL(tr, SaveToDataBase(_, _, _)).Times(0);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    EXPECT_FALSE(tr.Make(from, to, 100));
}

TEST(TransactionTests, CreditDebitCoverage) {
    MockAccount acc(3, 500);
    Transaction tr;
    
    EXPECT_CALL(acc, ChangeBalance(100)).Times(1);
    tr.Credit(acc, 100);
    
    EXPECT_CALL(acc, GetBalance()).WillOnce(Return(400));
    EXPECT_CALL(acc, ChangeBalance(-200)).Times(1);
    EXPECT_TRUE(tr.Debit(acc, 200));
    
    EXPECT_CALL(acc, GetBalance()).WillOnce(Return(300));
    EXPECT_CALL(acc, ChangeBalance(_)).Times(0);
    EXPECT_FALSE(tr.Debit(acc, 500));
}
